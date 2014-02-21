/* pgc.c
 * Functions for use by the garbage collector, PGC
 */

#include "pdata.h"
#include "pmem.h"
#include "puniverse.h"
#include "pgc.h"

/* THE GARBAGE COLLECTION MODUS
Poe utilizes a very simple, lean garbage collection algorithm. Poe's compound
data structures (tables, arrays, strings, pfuncs, and codes) are all
created with the functions in pmem.h through dynamic heap allocation;
the garbage collector gets the right to free these structures when the
object is added to the global object list through a call to
poe_gc_control.

A GC cycle is split into 5 distinct phases:
1) Mark all objects in the object list white.
2) Mark all reachable objects black.
3) Free all white objects in the object list.
4) Re-orient the object list.
5) Transfer all items from the buffer list to the object list.

Phase 2 can only be performed sequentially, but all the other steps can
be performed completely incrementally. During steps 2 through 5, objects put
into the garbage collection lists through poe_gc_control are added to the 
buffer list, and are moved to the object list through step 5.

Further, each phase is divided into concrete steps. A GC "step" is a period
of execution time in which a small fraction of the work of a single phase is
completed. The behavior of the GC is governed primarily by a datum called
the "step factor", which provides a rough guideline for how many objects
the collector should touch in a given step relative to how many objects
were added to a GC list over the course of the execution time since the
last step. For example, a step factor of 5.0 denotes that if, for example,
N objects were added since the last step, 5N objects should be touched in the
next GC step. (This is just a guideline; more or fewer objects may be touched
in the cycle. The GC may opt for less if "touching" an object may take longer
in this phase, as is the case in phase 3.) In this sense, the GC's behavior
is dictated by memory allocation; the GC will be more aggressive if a memory-
intensive application is being run. Higher step factors make the GC act more
aggressively, at the cost of extra time being spent in GC cycles. Meanwhile,
a step factor that is too low may cause the garbage collector to run too slowly
when compared to memory allocation. The GC can change the step factor if it
is not doing an adequate job of handling the memory (for instance, if it is
asked to perform an emergency garbage collection). If it is never asked to
perform an emergency collection, it may decide after some time to reduce the 
step factor.

Another datum pertinent to the behavior of the GC is the timer, which controls
how many Poe bytecode instructions can be executed in between each GC step.
A timer value of 100 mandates that there be 100 instructions in between each
step; a timer value of 0 will never allow an instruction to be executed,
instead launching the application into an infinite loop of garbage collection.
(Obviously, 0 is not a legal timer value.)

The GC is somewhat generational, which means that on many cycles, only the 
newest objects in the object list will be swept and freed in phase 3.
Though this will not necessarily decrease the execution time of a 
program, it will increase its *efficiency*; that is, the time that would 
otherwise be spent scanning old objects (which, by the generational hypothesis,
are probably still live anyway) can be put to doing other useful work. 
A cycle in which the entire object list is swept is called a "major cycle"; 
otherwise, it is called a "minor cycle". 

The number of cycles between major cycles is called the "major offset", and it 
is another important datum which controls the behavior of the GC. Set the major
offset to 6 to put 6 minor cycles in between each major cycle; set it to 0 to 
have each cycle be a major cycle.

Additionally, while the memory-allocation functions are responsible for
resizing structures if they need to be larger, it is the job of the garbage
collector to ensure that objects are never much larger than they need to be.
During each major cycle, a sixth phase is performed in which each old structure
is scanned for waste. If a structure is found to be much larger than it needs to
be, then it is resized.

Finally, the GC controls the factories. Generally, the GC will prefer to put
dead objects in factories rather than free them to the OS, as it eliminates
the need for a system call. If the GC is called to perform an emergency 
garbage collection, however, it will "empty" the factories by freeing all
of their structures.

Though the algorithm is smart enough to deal with catastrophic cases, great
GC performance requires fine-tuning of the step factor, major offset, and timer.
If your application requires that little time be spent on garbage collection
and if space is not at a premium, you want a low step factor and a high major
offset and timer; if space is at a premium, then you want a high step factor
and a low major offset and timer (though this will be at the cost of a higher
execution time). The ideal case for each application will be somewhere in
between, so experiment with different values for each parameter and check
your gains.

*/

void pgc_stop(poe_universe *U)
{
  U->gc_hub.running = 0;
}

void pgc_resume(poe_universe *U)
{
  U->gc_hub.running = 1;
}

void pgc_set_stepfactor(poe_universe *U, double d)
{
  U->gc_hub.step_factor = d;
}

int pgc_set_timer(poe_universe *U, int i)
{
  if (i<=0) return 0;
  U->gc_hub.timer = i;
  return 1;
}

void pgc_set_offset(poe_universe *U, int i)
{
  U->gc_hub.major_offset = i;
}

/* Puts an object under gc control. Do not call this function on an object if
you are still using this object in the PCL without already being certain that
it exists somewhere in the reference tree of the poe_universe. */
int poe_gc_control(poe_universe *U, poe_obj o, char tag)
{
  poe_gc_hub *gc = &(U->gc_hub);
  tagged_poe_obj tpo = { tag, o };
  if (!((tag==POE_PFUNC_TAG)||(tag==POE_STR_TAG)||(tag==POE_TABLE_TAG)||
	(tag==POE_ARR_TAG)||(tag==POE_CODE_TAG)))
    return 1;
  switch (tag) {
  case POE_TABLE_TAG:
    if (o.poe_table->control)
      return 1;
    o.poe_table->control = 1;
    break;
  case POE_STR_TAG:
    if (o.poe_str->control)
      return 1;
    o.poe_str->control = 1;
    break;
  case POE_ARR_TAG:
    if (o.poe_arr->control)
      return 1;
    o.poe_arr->control = 1;
    break;
  case POE_PFUNC_TAG:
    if (o.poe_pfunc->control)
      return 1;
    o.poe_pfunc->control = 1;
    break;
  case POE_CODE_TAG:
    if (o.poe_code->control)
      return 1;
    o.poe_code->control = 1;
    break;
  }

  // if the phase is 0 or 1, we can put it right in the object list. We
  // will have to increment the new_count.
  if (gc->phase<=POE_MARK_WHITE_PHASE) {
    if (gc->object_stack_index + 1 >= gc->object_stack_size)
      if (!enlarge_stack(U,POE_OBJ_STACK,1))
	return 0;
    gc->object_stack_index ++;
    gc->object_stack_base[gc->object_stack_index] = tpo;
    gc->new_count ++;
  } else { // we have to put it in the buffer list
    if (gc->buffer_stack_index + 1 >= gc->buffer_stack_size)
      if (!enlarge_stack(U,POE_BUF_STACK,1))
	return 0;
    gc->buffer_stack_index ++;
    gc->buffer_stack_base[gc->buffer_stack_index] = tpo;
  }

  return 1;
}

/* phase 1: mark all objects in object list white. if step is 0, performs the
entire phase. */
void pgc_phase1_step(poe_universe *U, int step)
{
  tagged_poe_obj *stack = U->gc_hub.object_stack_base;
  int i, n = U->gc_hub.object_stack_index, forever = !step;
  tagged_poe_obj o;
  for (i = U->gc_hub.phase_index; (i<=n)&&(step||forever); i++, step--) {
    o = stack[i];
    switch (o.tag) {
    case POE_TABLE_TAG:
      o.data.poe_table->gc_info = POE_WHITE;
      break;
    case POE_ARR_TAG:
      o.data.poe_arr->gc_info = POE_WHITE;
      break;
    case POE_STR_TAG:
      o.data.poe_str->gc_info = POE_WHITE;
      break;
    case POE_PFUNC_TAG:
      o.data.poe_pfunc->gc_info = POE_WHITE;
      break;
    case POE_CODE_TAG:
      o.data.poe_code->gc_info = POE_WHITE;
      break;
    }
  }
  // how did we break out of the loop?
  if (i>n) { // case: ran out of objects to mark
             // action: switch to phase 2
    U->gc_hub.phase = POE_MARK_BLACK_PHASE;
    U->gc_hub.phase_index = 0;
  } else { // case: reached our step limit
           // action: stay in phase 1
    U->gc_hub.phase_index = i;
  }
  U->gc_hub.debt = 0; // reset debt 
  return;
}

/* phase 2: mark all reachable objects black. we must scan the following
structures:
1) global table
2) all current local tables
3) execution stack
4) all current argv's
5) current for stack
6) the current retv
As implemented, this phase cannot be performed incrementally, so the step
value is being ignored. I may change it so that it can be performed
incrementally in the future.
The algorithm is as follows:
1) If applicable, gray the root set (global table, retv)
2) Blacken the root set (if applicable) by graying all the objects the root set
references directly.
3) Blacken all gray objects.
4) When no gray objects are left, all items referenced by that set have been
marked. 
In code, the logical flow is as follows:
1) Gray the object (you can do this to more or less any object).
2) Blacken the object with a pgc_blacken function.
3) Traverse the object with a pgc_traverse function.
Then, all the objects to which that object points indirectly will be PITCH.*/
void pgc_phase2_step(poe_universe *U, int step)
{
  int i, n;
  poe_table *table, **locs = U->locals_stack_base;
  poe_arr *arr, **argvs = U->argv_stack_base;
  tagged_poe_obj tpo, *main = U->main_stack_base;
  for_obj f, *fors;
  /* 1) SCAN GLOBAL TABLE */
  U->global_table->gc_info = POE_GRAY;
  pgc_blacken_table(U->global_table);
  pgc_traverse_table(U->global_table);
  /* 2) SCAN LOCAL TABLES */
  for (i=0, n=U->locals_stack_index; i<=n; i++) {
    table = locs[i]; 
    if ((table->gc_info)<POE_BLACK) {
      table->gc_info = POE_GRAY;
      pgc_blacken_table(table);
    }
    pgc_traverse_table(table);
  }
  /* 3) SCAN EXECUTION STACK */
  for (i=0, n=U->main_stack_index; i<=n; i++) {
    tpo = main[i];
    switch (tpo.tag) {
    case POE_TABLE_TAG:
      if ((tpo.data.poe_table->gc_info)<POE_BLACK) {
	tpo.data.poe_table->gc_info = POE_GRAY;
	pgc_blacken_table(tpo.data.poe_table);
      }
      pgc_traverse_table(tpo.data.poe_table);
      break;
    case POE_ARR_TAG:
      if ((tpo.data.poe_arr->gc_info)<POE_BLACK) {
	tpo.data.poe_arr->gc_info = POE_GRAY;
	pgc_blacken_arr(tpo.data.poe_arr);
      }
      pgc_traverse_arr(tpo.data.poe_arr);
      break;
    case POE_STR_TAG:
      if ((tpo.data.poe_str->gc_info)<POE_BLACK) {
	tpo.data.poe_str->gc_info = POE_GRAY;
	pgc_blacken_str(tpo.data.poe_str);
      }
      pgc_traverse_str(tpo.data.poe_str);
      break;
    case POE_PFUNC_TAG:
      if ((tpo.data.poe_pfunc->gc_info)<POE_BLACK) {
	tpo.data.poe_pfunc->gc_info = POE_GRAY;
	pgc_blacken_pfunc(tpo.data.poe_pfunc);
      }
      pgc_traverse_pfunc(tpo.data.poe_pfunc);
      break;
    case POE_CODE_TAG:
      if ((tpo.data.poe_code->gc_info)<POE_BLACK) {
	tpo.data.poe_code->gc_info = POE_GRAY;
	pgc_blacken_code(tpo.data.poe_code);
      }
      break;
    default:
      break;
    }
  }
  /* 4) SCAN ARGV'S */
  for (i=0, n=U->argv_stack_index; i<=n; i++) {
    arr = argvs[i];
    if ((arr->gc_info)<POE_BLACK) {
      arr->gc_info = POE_GRAY;
      pgc_blacken_arr(arr);
    }
    pgc_traverse_arr(arr);
  }
  /* 5) SCAN FOR STACK */
  fors = U->for_stack_base;
  for (i=0, n=U->for_stack_index; i<=n; i++) {
    f = fors[i];

    if ((f.key_sname->gc_info)<POE_BLACK) {
      f.key_sname->gc_info = POE_GRAY;
      pgc_blacken_str(f.key_sname);
    }
    pgc_traverse_str(f.key_sname);

    if ((f.val_sname->gc_info)<POE_BLACK) {
      f.val_sname->gc_info = POE_GRAY;
      pgc_blacken_str(f.val_sname);
    }
    pgc_traverse_str(f.val_sname);

    switch (f.tag) {
    case POE_TABLE_TAG:
      if ((f.iterable.poe_table->gc_info)<POE_BLACK) {
	f.iterable.poe_table->gc_info = POE_GRAY;
	pgc_blacken_table(f.iterable.poe_table);
      }
      pgc_traverse_table(f.iterable.poe_table);
      break;
    case POE_ARR_TAG:
      if ((f.iterable.poe_arr->gc_info)<POE_BLACK) {
	f.iterable.poe_arr->gc_info = POE_GRAY;
	pgc_blacken_arr(f.iterable.poe_arr);
      }
      pgc_traverse_arr(f.iterable.poe_arr);
      break;
    case POE_STR_TAG:
      if ((f.iterable.poe_str->gc_info)<POE_BLACK) {
	f.iterable.poe_str->gc_info = POE_GRAY;
	pgc_blacken_str(f.iterable.poe_str);
      }
      pgc_traverse_str(f.iterable.poe_str);
      break;
    default:
      break;
    }
  }
  /* 6) SCAN RETV */
  arr = U->retv;
  if (arr) {
    if ((arr->gc_info)<POE_BLACK) {
      arr->gc_info = POE_GRAY;
      pgc_blacken_arr(arr);
    }
    pgc_traverse_arr(arr);
  }

  U->gc_hub.phase = POE_FREE_WHITE_PHASE;
  U->gc_hub.phase_index = U->gc_hub.object_stack_index;
  U->gc_hub.debt = 0;

  return;
}

/* phase 3: free all white objects in the object-list. Start from the top
   of the stack and move downward. If step is 0, performs entire phase.
Does not deposit any items in factories if emergeny is POE_EMERGENCY (1). */
void pgc_phase3_step(poe_universe *U, int step, int emergency)
{
  // we only need to check the new objects if this is not a major phase
  int i, stop = !(U->gc_hub.cycle_count) ? 0 : U->gc_hub.object_stack_index
    - U->gc_hub.new_count + 1, forever = !step, j;
  tagged_poe_obj *stack = U->gc_hub.object_stack_base, tpo;
  poe_obj_list *l, *next;
  for (i=U->gc_hub.phase_index; (i>=stop)&&(step||forever); i--, step--) {
    tpo = stack[i];
    switch (tpo.tag) {
    case POE_TABLE_TAG:
      if ((tpo.data.poe_table->gc_info)<POE_BLACK) {
	/* FIRST SCAN ALL OBJECT LISTS */
	for (j=0; j<tpo.data.poe_table->size; j++) {
	  for (l=tpo.data.poe_table->t[j]; l; l = next) {
	    next = l->next;
	    if ((U->gc_hub.list_factory_index+1 < FACTORY_SIZE)&&!emergency) {
	      // case: we can add it to the factory
	      U->gc_hub.list_factory_index ++;
	      U->gc_hub.list_factory[U->gc_hub.list_factory_index] = l;
	    } else { 
	      // case: it has to go away :(
	      free(l);
	    }
	  }
	}
	/* THEN SCAN THE TABLE */
	if ((U->gc_hub.table_factory_index+1 < FACTORY_SIZE)&&!emergency) {
	  // case: we can add it to the factory
	  U->gc_hub.table_factory_index ++;
	  for (j=0; j<tpo.data.poe_table->size; j++)
	    tpo.data.poe_table->t[j] = NULL;
	  tpo.data.poe_table->n = 0;
	  tpo.data.poe_table->control = 0;
	  tpo.data.poe_table->super_tag = POE_UNDEF_TAG;
	  tpo.data.poe_table->meta = NULL;
	  U->gc_hub.table_factory[U->gc_hub.table_factory_index] = 
	    tpo.data.poe_table;
	} else { // case: it has to go away :(
	  free(tpo.data.poe_table->t);
	  free(tpo.data.poe_table);
	}
	stack[i].tag = POE_UNDEF_TAG;
      } 
      break;
    case POE_ARR_TAG:
      if ((tpo.data.poe_arr->gc_info)<POE_BLACK) { // we can free it :)
	if ((U->gc_hub.arr_factory_index+1 < FACTORY_SIZE)&&!emergency) {
	  // case: we can add it to the factory
	  U->gc_hub.arr_factory_index ++;
	  for (j=0; j<tpo.data.poe_arr->size; j++)
	    tpo.data.poe_arr->a[j].tag = POE_UNDEF_TAG;
	  tpo.data.poe_arr->control = 0;
	  tpo.data.poe_arr->super_tag = POE_UNDEF_TAG;
	  tpo.data.poe_arr->max = -1;
	  tpo.data.poe_arr->meta = NULL;
	  U->gc_hub.arr_factory[U->gc_hub.arr_factory_index] = tpo.data.poe_arr;
	} else { // case: it has to go away :(
	  free(tpo.data.poe_arr->a);
	  free(tpo.data.poe_arr);
	}
	stack[i].tag = POE_UNDEF_TAG;
      }
      break;
    case POE_STR_TAG:
      if ((tpo.data.poe_str->gc_info)<POE_BLACK) { // we can free it :)
	if ((U->gc_hub.str_factory_index+1 < FACTORY_SIZE)&&!emergency) {
	  // case : we can add it to the factory
	  U->gc_hub.str_factory_index ++;
	  tpo.data.poe_str->s[0] = '\0';
	  tpo.data.poe_str->meta = NULL;
	  tpo.data.poe_str->super_tag = POE_UNDEF_TAG;
	  tpo.data.poe_str->control = 0;
	  tpo.data.poe_str->len = 0;
	  U->gc_hub.str_factory[U->gc_hub.str_factory_index] = tpo.data.poe_str;
	} else { // case: it has to go away :(
	  free(tpo.data.poe_str->s);
	  free(tpo.data.poe_str);
	}
	stack[i].tag = POE_UNDEF_TAG;
      }
      break;
    case POE_PFUNC_TAG:
      if ((tpo.data.poe_pfunc->gc_info)<POE_BLACK) { // we can free it :)
	// there is no pfunc factory
	free(tpo.data.poe_pfunc->code);
	free(tpo.data.poe_pfunc);
	stack[i].tag = POE_UNDEF_TAG;
      }
      break;
    case POE_CODE_TAG:
      if ((tpo.data.poe_code->gc_info)<POE_BLACK) { // we can free it :)
	// there is no code factory
	free(tpo.data.poe_code->code);
	free(tpo.data.poe_code);
	stack[i].tag = POE_UNDEF_TAG;
      }
      break;
    }
  }
  // how did we exit the loop?
  if (i<stop) { // case: we scanned the whole stack
    U->gc_hub.phase = POE_ORIENT_PHASE;
    U->gc_hub.phase_index = 0;
    U->gc_hub.phase_sub = 0;
  } else {
    U->gc_hub.phase_index = i;
  }
  U->gc_hub.debt = 0; // reset the debt
  return;
}

/* phase 4: reorient stack. start from bottom and move upward. if step is 0,
   performs entire phase. */
void pgc_phase4_step(poe_universe *U, int step)
{
  int i, stop = U->gc_hub.object_stack_index, forever = !step, 
    sub = U->gc_hub.phase_sub;
  tagged_poe_obj *stack = U->gc_hub.object_stack_base, tpo;
  for (i=U->gc_hub.phase_index; (i<=stop)&&(step||forever); step--, i++) {
    tpo = stack[i];
    if (tpo.tag != POE_UNDEF_TAG) { // case: regular structure
      if (i==sub) { sub++; continue; }
      stack[sub++] = tpo;
      stack[i].tag = POE_UNDEF_TAG;
    }
  }
  /* how did we exit the loop? */
  if (i>stop) { // case: we reoriented the whole stack
    U->gc_hub.phase = POE_BUFFER_PHASE;
    U->gc_hub.phase_index = 0;
    U->gc_hub.phase_sub = 0;
    U->gc_hub.object_stack_index = sub-1;
    U->gc_hub.new_count = 0;
  } else {
    U->gc_hub.phase_index = i;
    U->gc_hub.phase_sub = sub;
  }
  U->gc_hub.debt = 0; // reset the debt
}

void pgc_fix_buf_stack(poe_universe *U)
{
  int i = 0, j = 0, size = U->gc_hub.buffer_stack_index;
  tagged_poe_obj *buf = U->gc_hub.buffer_stack_base, tpo;
  for (; i<=size; i++) {
    tpo = buf[i];
    if (tpo.tag != POE_UNDEF_TAG) 
      buf[j++] = tpo;
  }
  U->gc_hub.buffer_stack_index = j-1;
}

/* phase 5: transfer items from buffer to object list. if step is 0, 
   performs entire phase. */
int pgc_phase5_step(poe_universe *U, int step)
{
  int i = U->gc_hub.phase_index, j = U->gc_hub.object_stack_index+1,  
    stop = U->gc_hub.buffer_stack_index, forever = !step,
    size = U->gc_hub.object_stack_size, new = 0;
  tagged_poe_obj *buf = U->gc_hub.buffer_stack_base, 
    *main = U->gc_hub.object_stack_base;
  for (; (i<=stop)&&(step||forever); i++, step--, j++, new++) {
    if (j+1>=size) {
      if (!enlarge_stack(U,POE_OBJ_STACK,0)) {
	pgc_fix_buf_stack(U);
	return 0;
      }
      else 
	size *= 2;
    }
    main[j] = buf[i];
    buf[i].tag = POE_UNDEF_TAG;
  }
  // how did we exit the loop?
  if (i>stop) { // case: we scanned the whole stack
    if (!U->gc_hub.cycle_count) { // case: this is a major cycle
      U->gc_hub.phase = POE_RESIZE_PHASE;
      U->gc_hub.phase_sub = 0;
      U->gc_hub.bufsize = stop;
    } else {
      U->gc_hub.phase = POE_MARK_WHITE_PHASE;
      U->gc_hub.cycle_count --;
    }
    U->gc_hub.phase_index = 0;
    U->gc_hub.buffer_stack_index = -1;
  } else { // case: we haven't scanned the whole stack
    U->gc_hub.phase_index = i;
  }
  U->gc_hub.debt = 0;
  U->gc_hub.new_count += new;
  U->gc_hub.object_stack_index = j-1;
  return 1;
}

int pgc_resize_objects(poe_universe *U, int step)
{
  int i = U->gc_hub.phase_index, stop = U->gc_hub.object_stack_index -
    U->gc_hub.new_count, forever = !step, oldsize, newsize;
  tagged_poe_obj *stack = U->gc_hub.object_stack_base, tpo;
  poe_obj_list **t;
  tagged_poe_obj *a;
  char *s;
  for (; (i<=stop)&&(step||forever); i++, step--) {
    tpo = stack[i];
    switch (tpo.tag) {
    case POE_TABLE_TAG:
      if ((((float)tpo.data.poe_table->n)/(oldsize = tpo.data.poe_table->size))
	  < POE_MIN_LOAD_FACTOR) 
	if (((newsize = poe_lowprime(oldsize))>=TABLE_MIN_SIZE)&&
	    (newsize!=oldsize)) {
	  // case: we can resize the table
	  t = malloc(newsize*sizeof(poe_obj_list *));
	  if (!t) return 0;
	  for (i=0; i<newsize; i++)
	    t[i] = NULL;
	  rehash_everything(tpo.data.poe_table,t,newsize);
	  free(tpo.data.poe_table->t);
	  tpo.data.poe_table->t = t;
	  tpo.data.poe_table->size = newsize;
	}
      break;
    case POE_ARR_TAG:
      if ((((float)tpo.data.poe_arr->max)/(oldsize = tpo.data.poe_arr->size))
	  < POE_MIN_LOAD_FACTOR)
	if ((newsize = oldsize/2) >= ARR_MIN_SIZE) {
	  // case: we can resize the table
	  a = malloc(newsize*sizeof(tagged_poe_obj));
	  if (!a) return 0;
	  for (i=0; i<newsize; i++)
	    a[i] = tpo.data.poe_arr->a[i];
	  free(tpo.data.poe_arr->a);
	  tpo.data.poe_arr->a = a;
	  tpo.data.poe_arr->size = newsize;
	}
      break;
    case POE_STR_TAG:
      if ((((float)tpo.data.poe_str->len)/(oldsize = tpo.data.poe_str->size))
	  < POE_MIN_LOAD_FACTOR)
	if ((newsize = oldsize/2) >= STR_MIN_SIZE) {
	  // case: we can resize the table
	  s = malloc(newsize);
	  if (!s) return 0;
	  for (i=0; i<newsize; i++)
	    s[i] = tpo.data.poe_str->s[i];
	  free(tpo.data.poe_str->s);
	  tpo.data.poe_str->s = s;
	  tpo.data.poe_str->size = newsize;
	}
      break;
    default:
      break;
    }
  }
  /* how did we exit the loop? */
  if (i>stop) { // case: we scanned the entire stack
    U->gc_hub.phase = POE_MARK_WHITE_PHASE;
    U->gc_hub.phase_index = 0;
    U->gc_hub.cycle_count = U->gc_hub.major_offset;
  } else { // case: we ran out of time
    U->gc_hub.phase_index = i;
  }
  U->gc_hub.debt = 0;
  return 1;
}

/* phase 0: resize all objects that are deemed to be too large. 
   In particular, the following objects are candidates for resizing:
   0) object stack
   1) buffer stack
   2) execution stack
   3) argv stack
   4) argc stack
   5) while stack
   6) for stack
   7) locals stack
   8) all the "old" objects in the object stack 
   The subphase of execution determines what objects are analyzed in each
   step. The minimum ratio of resident items to object size is defined
   in pgc.h as MIN_LOAD_FACTOR; as given, it is 0.3. */
int pgc_phase0_step(poe_universe *U, int step)
{
  int i;
  switch (U->gc_hub.phase_sub) {
  case 0: // object stack
    i = (int)(POE_MIN_LOAD_FACTOR*(U->gc_hub.object_stack_size));
    if (U->gc_hub.object_stack_index<i) 
      if ((U->gc_hub.object_stack_size/2)>=OBJ_STACK_SIZE)
	if (!ensmall_stack(U,POE_OBJ_STACK,0))
	  return 0;
    U->gc_hub.phase_sub ++;
    break;
  case 1: // buffer stack
    i = (int)(POE_MIN_LOAD_FACTOR*(U->gc_hub.buffer_stack_size));
    if (U->gc_hub.bufsize<i) 
      if ((U->gc_hub.buffer_stack_size/2)>=BUF_STACK_SIZE)
	if (!ensmall_stack(U,POE_BUF_STACK,0))
	  return 0;
    U->gc_hub.phase_sub ++;
    break;
  case 2: // execution stack
    i = (int)(POE_MIN_LOAD_FACTOR*(U->main_stack_size));
    if (U->main_stack_index<i) 
      if ((U->main_stack_size/2)>=EXEC_STACK_SIZE)
	if (!ensmall_stack(U,POE_MAIN_STACK,0))
	  return 0;
    U->gc_hub.phase_sub ++;
    break;
  case 3: // argv stack
    i = (int)(POE_MIN_LOAD_FACTOR*(U->argv_stack_size));
    if (U->argv_stack_index<i)
      if ((U->argv_stack_size/2)>=ITER_STACK_SIZE)
	if (!ensmall_stack(U,POE_ARGV_STACK,0))
	  return 0;
    U->gc_hub.phase_sub ++;
    break;
  case 4: // argc stack
    i = (int)(POE_MIN_LOAD_FACTOR*(U->argc_stack_size));
    if (U->argc_stack_index<i)
      if ((U->argc_stack_size/2)>=ITER_STACK_SIZE)
	if (!ensmall_stack(U,POE_ARGC_STACK,0))
	  return 0;
    U->gc_hub.phase_sub ++;
    break;
  case 5: // while stack
    i = (int)(POE_MIN_LOAD_FACTOR*(U->while_stack_size));
    if (U->while_stack_index<i)
      if ((U->argc_stack_size/2)>=ITER_STACK_SIZE)
	if (!ensmall_stack(U,POE_WHILE_STACK,0))
	  return 0;
    U->gc_hub.phase_sub ++;
    break;
  case 6: // for stack
    i = (int)(POE_MIN_LOAD_FACTOR*(U->for_stack_size));
    if (U->for_stack_index<i)
      if ((U->for_stack_size/2)>=ITER_STACK_SIZE)
	if (!ensmall_stack(U,POE_FOR_STACK,0))
	  return 0;
    U->gc_hub.phase_sub ++;
    break;
  case 7: // locals stack
    i = (int)(POE_MIN_LOAD_FACTOR*(U->locals_stack_size));
    if (U->locals_stack_index<i)
      if ((U->locals_stack_size/2)>=LOCALS_STACK_SIZE)
	if (!ensmall_stack(U,POE_LOCALS_STACK,0))
	  return 0;
    U->gc_hub.phase_sub ++;
    break;
  case 8: // all old objects
    return pgc_resize_objects(U,step);
    break;
  }
  U->gc_hub.debt = 0;
  return 1;
}

/* perform a single step of garbage collection */
void pgc_step(poe_universe *U)
{
  poe_gc_hub *gc = &(U->gc_hub);
  int stp_amount = (int)((gc->step_factor)*(gc->debt));

  if (!U->gc_hub.running) return;

  stp_amount = poe_max(stp_amount,gc->min_step); /* rough minimum number of objs
						   to touch on this step */
  switch (gc->phase) {
  case POE_MARK_WHITE_PHASE:
    pgc_phase1_step(U,stp_amount);
    break;
  case POE_MARK_BLACK_PHASE:
    pgc_phase2_step(U,stp_amount);
    break;
  case POE_FREE_WHITE_PHASE:
    pgc_phase3_step(U,stp_amount,POE_NORMAL);
    break;
  case POE_ORIENT_PHASE:
    pgc_phase4_step(U,stp_amount);
    break;
  case POE_BUFFER_PHASE:
    if (!pgc_phase5_step(U,stp_amount)) {
      pgc_raw_begin(U);
      poe_emergency_gc(U);
    }
    break;
  case POE_RESIZE_PHASE:
    if (!pgc_phase0_step(U,stp_amount)) {
      pgc_raw_begin(U);
      poe_emergency_gc(U);
    }
    break;
  }
  gc->debt = 0;
  return;
}

/* decrements the counter and triggers a garbage collection step if necessary */
void pgc_countdown(poe_universe *U)
{
  if (!U->gc_hub.timer_count) {
    pgc_step(U);
    U->gc_hub.timer_count = U->gc_hub.timer;
  } else {
    U->gc_hub.timer_count --;
  }
  return;
}

/* does a raw set of the GC's state to phase 1-0. this is a VERY UNSAFE
function to use unless you are sure the GC will take the correct action
after the set. */
void pgc_raw_begin(poe_universe *U)
{
  U->gc_hub.phase = POE_MARK_WHITE_PHASE;
  U->gc_hub.phase_index = 0;
}

/* Runs a complete garbage collection cycle. If mode is nonzero, exits without
   performing any collection if we are already at the beginning of a cycle.
   Either way, the GC will be in phase 1-0 when this function exits. */
void pgc_cycle(poe_universe *U, int mode, int emergency)
{
  if (mode)
    if ((U->gc_hub.phase==1)&&(U->gc_hub.phase_index==0))
      return;
  switch (U->gc_hub.phase) {
  case POE_RESIZE_PHASE:
    if (!pgc_phase0_step(U,0)) {
      pgc_raw_begin(U);
      poe_emergency_gc(U);
    }
    break;
  case POE_MARK_WHITE_PHASE:
    pgc_phase1_step(U,0);
  case POE_MARK_BLACK_PHASE:
    pgc_phase2_step(U,0);
  case POE_FREE_WHITE_PHASE:
    pgc_phase3_step(U,0,emergency);
  case POE_ORIENT_PHASE:
    pgc_phase4_step(U,0);
  case POE_BUFFER_PHASE:
    if (!pgc_phase5_step(U,0)) {
      pgc_raw_begin(U);
      poe_emergency_gc(U);
      break;
    }
    if (!U->gc_hub.cycle_count)
      if (!pgc_phase0_step(U,0)) {
	pgc_raw_begin(U);
	poe_emergency_gc(U);
	break;
      }
  }
  return;
}

/* completes the current cycle. Does not run another cycle if we are at the
   very beginning of a new one. */
void pgc_complete_cycle(poe_universe *U, int emergency)
{
  pgc_cycle(U,1,emergency);
}

/* runs a complete cycle. DOES run another cycle if we are at the very 
   very beginning of a new one. */
void pgc_do_cycle(poe_universe *U, int emergency)
{
  pgc_cycle(U,0,emergency);
}

/* completes the current phase and sets up a new major cycle, regardless 
   of whether the cycle was major beforehand. */
void pgc_force_major_cycle(poe_universe *U)
{
  pgc_complete_cycle(U,POE_NORMAL);
  U->gc_hub.cycle_count = 0;
  return;
}

void pgc_cleanout_factories(poe_universe *U)
{
  int i;
  for (i=0; i<=U->gc_hub.table_factory_index; i++) {
    free(U->gc_hub.table_factory[i]->t);
    free(U->gc_hub.table_factory[i]);
  }
  for (i=0; i<=U->gc_hub.str_factory_index; i++) {
    free(U->gc_hub.str_factory[i]->s);
    free(U->gc_hub.str_factory[i]);
  }
  for (i=0; i<=U->gc_hub.arr_factory_index; i++) {
    free(U->gc_hub.arr_factory[i]->a);
    free(U->gc_hub.arr_factory[i]);
  }
  for (i=0; i<=U->gc_hub.list_factory_index; i++) {
    free(U->gc_hub.list_factory[i]);
  }
  U->gc_hub.table_factory_index = -1;
  U->gc_hub.str_factory_index = -1;
  U->gc_hub.arr_factory_index = -1;
  U->gc_hub.list_factory_index = -1;
  return;
}

/* an emergency garbage collection consists of two parts:
1) a major cycle
2) cleaning out the factories
By doing this, Poe should get rid of all unnecessary memory.
*/
void poe_emergency_gc(poe_universe *U)
{
  pgc_cleanout_factories(U);
  pgc_force_major_cycle(U); // sets up a major cycle
  pgc_do_cycle(U,POE_EMERGENCY); // forces the complete cycle
  /* there are no items in the factory after the cycle because an emergency
     GC puts no items in factories */
}

/* Blacken functions -- these functions expect gray objects. They will return
without change if the object is already black, but they will not function
on white objects.
Further, these functions don't actually traverse a series of references; they
simple blacken a single object by graying all the direct references that
object contains. Use the pgc_traverse functions to traverse a structure
(that is, blacken an object and all the objects that object references
directly or indirectly). */
void pgc_blacken_table(poe_table *table)
{
  int i, size = table->size;
  poe_obj_list *l, **t = table->t;
  if (table->gc_info>=POE_BLACK) return;
  /* MARK ALL REFERENCED OBJECTS GRAY */
  if (table->meta) // mark metatable
    if ((table->meta->gc_info)==POE_WHITE)
      table->meta->gc_info = POE_GRAY;
  switch (table->super_tag) { // mark supertable
  case POE_TABLE_TAG:
    if ((table->super.poe_table->gc_info)==POE_WHITE)
      table->super.poe_table->gc_info = POE_GRAY;
    break;
  case POE_ARR_TAG:
    if ((table->super.poe_arr->gc_info)==POE_WHITE)
      table->super.poe_arr->gc_info = POE_GRAY;
    break;
  case POE_PFUNC_TAG:
    if ((table->super.poe_pfunc->gc_info)==POE_WHITE)
      table->super.poe_pfunc->gc_info = POE_GRAY;
    break;
  case POE_CODE_TAG:
    if ((table->super.poe_code->gc_info)==POE_WHITE)
      table->super.poe_code->gc_info = POE_GRAY;
    break;
  case POE_STR_TAG:
    if ((table->super.poe_str->gc_info)==POE_WHITE)
      table->super.poe_code->gc_info = POE_GRAY;
    break;
  default:
    break;
  }
  for (i=0; i<size; i++) { // mark list
    for (l = t[i]; l; l = l->next) {
      switch (l->tag) { // gray object list element references
      case POE_TABLE_TAG:
	if ((l->first.poe_table->gc_info)==POE_WHITE)
	  l->first.poe_table->gc_info = POE_GRAY;
	break;
      case POE_ARR_TAG:
	if ((l->first.poe_arr->gc_info)==POE_WHITE)
	  l->first.poe_arr->gc_info = POE_GRAY;
	break;
      case POE_PFUNC_TAG:
	if ((l->first.poe_pfunc->gc_info)==POE_WHITE)
	  l->first.poe_pfunc->gc_info = POE_GRAY;
	break;
      case POE_STR_TAG:
	if ((l->first.poe_str->gc_info)==POE_WHITE)
	  l->first.poe_str->gc_info = POE_GRAY;
	break;
      case POE_CODE_TAG:
	if ((l->first.poe_code->gc_info)==POE_WHITE)
	  l->first.poe_code->gc_info = POE_GRAY;
	break;
      default:
	break;
      }
    }
  }
  /* we've ensured the table's metatable, superobject, and referenced objects
     are all gray, so we can blacken */
  table->gc_info = POE_BLACK;
  return;
}

void pgc_blacken_arr(poe_arr *arr)
{
  int i, size = arr->max + 1;
  tagged_poe_obj *a = arr->a, tpo;
  if ((arr->gc_info)>=POE_BLACK) return;
  /* MARK ALL REFERENCED OBJECTS GRAY */
  if (arr->meta) // mark metatable
    if ((arr->meta->gc_info)==POE_WHITE)
      arr->meta->gc_info = POE_GRAY;
  switch (arr->super_tag) { // mark supertable
  case POE_TABLE_TAG:
    if ((arr->super.poe_table->gc_info)==POE_WHITE)
      arr->super.poe_table->gc_info = POE_GRAY;
    break;
  case POE_ARR_TAG:
    if ((arr->super.poe_arr->gc_info)==POE_WHITE)
      arr->super.poe_arr->gc_info = POE_GRAY;
    break;
  case POE_PFUNC_TAG:
    if ((arr->super.poe_pfunc->gc_info)==POE_WHITE)
      arr->super.poe_pfunc->gc_info = POE_GRAY;
    break;
  case POE_CODE_TAG:
    if ((arr->super.poe_code->gc_info)==POE_WHITE)
      arr->super.poe_code->gc_info = POE_GRAY;
    break;
  case POE_STR_TAG:
    if ((arr->super.poe_str->gc_info)==POE_WHITE)
      arr->super.poe_code->gc_info = POE_GRAY;
    break;
  default:
    break;
  }
  for (i=0; i<size; i++) {
    tpo = a[i];
    switch (tpo.tag) {
    case POE_TABLE_TAG:
      if ((tpo.data.poe_table->gc_info)==POE_WHITE)
	tpo.data.poe_table->gc_info = POE_GRAY;
      break;
    case POE_PFUNC_TAG:
      if ((tpo.data.poe_pfunc->gc_info)==POE_WHITE)
	tpo.data.poe_pfunc->gc_info = POE_GRAY;
      break;
    case POE_CODE_TAG:
      if ((tpo.data.poe_code->gc_info)==POE_WHITE)
	tpo.data.poe_code->gc_info = POE_GRAY;
      break;
    case POE_STR_TAG:
      if ((tpo.data.poe_str->gc_info)==POE_WHITE)
	tpo.data.poe_str->gc_info = POE_GRAY;
      break;
    case POE_ARR_TAG:
      if ((tpo.data.poe_arr->gc_info)==POE_WHITE)
	tpo.data.poe_arr->gc_info = POE_GRAY;
      break;
    default:
      break;
    }
  }
    /* we've ensured the metatable, superobject, and all objects in the
       array are gray, so we can blacken */
  arr->gc_info = POE_BLACK;
  return;
}

void pgc_blacken_str(poe_str *str)
{
  if ((str->gc_info)>=POE_BLACK) return;
  /* a string only has a superobject and a metatable, so we just need to gray
     those */
  if (str->meta)
    if ((str->meta->gc_info) == POE_WHITE)
      str->meta->gc_info = POE_GRAY;
  switch (str->super_tag) {
  case POE_STR_TAG:
    if ((str->super.poe_str->gc_info)==POE_WHITE)
      str->super.poe_str->gc_info = POE_GRAY;
    break;
  case POE_ARR_TAG:
    if ((str->super.poe_arr->gc_info)==POE_WHITE)
      str->super.poe_arr->gc_info = POE_GRAY;
    break;
  case POE_TABLE_TAG:
    if ((str->super.poe_table->gc_info)==POE_WHITE)
      str->super.poe_table->gc_info = POE_GRAY;
    break;
  case POE_PFUNC_TAG:
    if ((str->super.poe_pfunc->gc_info)==POE_WHITE)
      str->super.poe_pfunc->gc_info = POE_GRAY;
    break;
  case POE_CODE_TAG:
    if ((str->super.poe_code->gc_info)==POE_WHITE)
      str->super.poe_code->gc_info = POE_GRAY;
    break;
  default:
    break;
  }
  str->gc_info = POE_BLACK;
  return;
}

void pgc_blacken_pfunc(poe_pfunc *pfunc)
{
  if ((pfunc->gc_info)>=POE_BLACK) return;
  /* a pfunc only has a parent table, so we'll just need to gray that */
  if (pfunc->parent)
    if ((pfunc->parent->gc_info)==POE_WHITE)
      pfunc->parent->gc_info = POE_GRAY;
  pfunc->gc_info = POE_BLACK;
  return;
}
void pgc_blacken_code(poe_code *code)
{
  /* a code object references no other objects, so we can gray it immediately
   */
  code->gc_info = POE_PITCH;
  return;
}

/* the pgc_traverse functions always expect BLACK objects. To traverse an
object, first blacken it with the correct pgc_blacken function and then
send it to a pgc_traverse function.
the pgc_traverse functions, given a BLACK object, blacken all the objects
the object points to directly and indirectly. */
void pgc_traverse_table(poe_table *t)
{
  int i, size = t->size;
  poe_obj_list *l, **a = t->t;
  if ((t->gc_info)==POE_PITCH) return; // case: already been traversed
  t->gc_info = POE_PITCH;
  if (t->meta) { // traverse metatable: first blacken then traverse
    if ((t->meta->gc_info)<POE_BLACK)
      pgc_blacken_table(t->meta);
    pgc_traverse_table(t->meta);
  }
  switch (t->super_tag) { // traverse super: first blacken then traverse
  case POE_TABLE_TAG:
    if ((t->super.poe_table->gc_info)<POE_BLACK)
      pgc_blacken_table(t->super.poe_table);
    pgc_traverse_table(t->super.poe_table);
    break;
  case POE_ARR_TAG:
    if ((t->super.poe_arr->gc_info)<POE_BLACK)
      pgc_blacken_arr(t->super.poe_arr);
    pgc_traverse_arr(t->super.poe_arr);
    break;
  case POE_STR_TAG:
    if ((t->super.poe_str->gc_info)<POE_BLACK)
      pgc_blacken_str(t->super.poe_str);
    pgc_traverse_str(t->super.poe_str);
    break;
  case POE_PFUNC_TAG:
    if ((t->super.poe_pfunc->gc_info)<POE_BLACK)
      pgc_blacken_pfunc(t->super.poe_pfunc);
    pgc_traverse_pfunc(t->super.poe_pfunc);
    break;
  case POE_CODE_TAG:
    if ((t->super.poe_code->gc_info)<POE_BLACK)
      pgc_blacken_code(t->super.poe_code);
    break;
  default: 
    break;
  }
  for (i=0; i<size; i++) { // traverse lists: first blacken then traverse
    for (l = a[i]; l; l = l->next) {
      switch (l->tag) {
      case POE_TABLE_TAG:
	if ((l->first.poe_table->gc_info)<POE_BLACK)
	  pgc_blacken_table(l->first.poe_table);
	if ((l->first.poe_table->gc_info)<POE_PITCH)
	  pgc_traverse_table(l->first.poe_table);
	break;
      case POE_ARR_TAG:
	if ((l->first.poe_arr->gc_info)<POE_BLACK)
	  pgc_blacken_arr(l->first.poe_arr);
	if ((l->first.poe_arr->gc_info)<POE_PITCH)
	  pgc_traverse_arr(l->first.poe_arr);
	break;
      case POE_STR_TAG:
	if ((l->first.poe_str->gc_info)<POE_BLACK)
	  pgc_blacken_str(l->first.poe_str);
	if ((l->first.poe_str->gc_info)<POE_PITCH)
	  pgc_traverse_str(l->first.poe_str);
	break;
      case POE_PFUNC_TAG:
	if ((l->first.poe_pfunc->gc_info)<POE_BLACK)
	  pgc_blacken_pfunc(l->first.poe_pfunc);
	if ((l->first.poe_pfunc->gc_info)<POE_PITCH)
	  pgc_traverse_pfunc(l->first.poe_pfunc);
	break;
      case POE_CODE_TAG:
	if ((l->first.poe_code->gc_info)<POE_BLACK)
	  pgc_blacken_code(l->first.poe_code);
	break;
      default:
	break;
      }
    }
  }
  return;
}

void pgc_traverse_arr(poe_arr *arr)
{
  int i, size = arr->max + 1;
  tagged_poe_obj *a = arr->a, o;
  if ((arr->gc_info)==POE_PITCH) return;
  arr->gc_info = POE_PITCH;
  if (arr->meta) { // metatable 
    if ((arr->meta->gc_info)<POE_BLACK) 
      pgc_blacken_table(arr->meta);
    pgc_traverse_table(arr->meta);
  }
  switch (arr->super_tag) { // superobject
  case POE_TABLE_TAG:
    if ((arr->super.poe_table->gc_info)<POE_BLACK)
      pgc_blacken_table(arr->super.poe_table);
    pgc_traverse_table(arr->super.poe_table);
    break;
  case POE_ARR_TAG:
    if ((arr->super.poe_arr->gc_info)<POE_BLACK)
      pgc_blacken_arr(arr->super.poe_arr);
    pgc_traverse_arr(arr->super.poe_arr);
    break;
  case POE_STR_TAG:
    if ((arr->super.poe_str->gc_info)<POE_BLACK)
      pgc_blacken_str(arr->super.poe_str);
    pgc_traverse_str(arr->super.poe_str);
    break;
  case POE_PFUNC_TAG:
    if ((arr->super.poe_pfunc->gc_info)<POE_BLACK)
      pgc_blacken_pfunc(arr->super.poe_pfunc);
    pgc_traverse_pfunc(arr->super.poe_pfunc);
    break;
  case POE_CODE_TAG:
    if ((arr->super.poe_code->gc_info)<POE_BLACK)
      pgc_blacken_code(arr->super.poe_code);
    break;
  default:
    break;
  }
  for (i=0; i<size; i++) { // traverse array: first blacken then traverse
    o = a[i];
    switch (o.tag) {
    case POE_TABLE_TAG:
      if ((o.data.poe_table->gc_info)<POE_BLACK)
	pgc_blacken_table(o.data.poe_table);
      if ((o.data.poe_table->gc_info)<POE_PITCH)
	pgc_traverse_table(o.data.poe_table);
      break;
    case POE_ARR_TAG:
      if ((o.data.poe_arr->gc_info)<POE_BLACK)
	pgc_blacken_arr(o.data.poe_arr);
      if ((o.data.poe_arr->gc_info)<POE_PITCH)
	pgc_traverse_arr(o.data.poe_arr);
      break;
    case POE_STR_TAG:
      if ((o.data.poe_str->gc_info)<POE_BLACK)
	pgc_blacken_str(o.data.poe_str);
      if ((o.data.poe_str->gc_info)<POE_PITCH)
	pgc_traverse_str(o.data.poe_str);
      break;
    case POE_PFUNC_TAG:
      if ((o.data.poe_pfunc->gc_info)<POE_BLACK)
	pgc_blacken_pfunc(o.data.poe_pfunc);
      if ((o.data.poe_pfunc->gc_info)<POE_PITCH)
	pgc_traverse_pfunc(o.data.poe_pfunc);
      break;
    case POE_CODE_TAG:
      if ((o.data.poe_code->gc_info)<POE_BLACK)
	pgc_blacken_code(o.data.poe_code);
      break;
    default:
      break;
    }
  }
  return;
}

void pgc_traverse_str(poe_str *str)
{
  /* a str only has a metatable and a super table */
  if ((str->gc_info)==POE_PITCH)
    return;
  str->gc_info = POE_PITCH;
  if (str->meta) {
    if ((str->meta->gc_info)<POE_BLACK)
      pgc_blacken_table(str->meta);
    pgc_traverse_table(str->meta);
  }
  switch (str->super_tag) {
  case POE_TABLE_TAG:
    if ((str->super.poe_table->gc_info)<POE_BLACK)
      pgc_blacken_table(str->super.poe_table);
    pgc_traverse_table(str->super.poe_table);
    break;
  case POE_ARR_TAG:
    if ((str->super.poe_arr->gc_info)<POE_BLACK)
      pgc_blacken_arr(str->super.poe_arr);
    pgc_traverse_arr(str->super.poe_arr);
    break;
  case POE_STR_TAG:
    if ((str->super.poe_str->gc_info)<POE_BLACK)
      pgc_blacken_str(str->super.poe_str);
    pgc_traverse_str(str->super.poe_str);
    break;
  case POE_PFUNC_TAG:
    if ((str->super.poe_pfunc->gc_info)<POE_BLACK)
      pgc_blacken_pfunc(str->super.poe_pfunc);
    pgc_traverse_pfunc(str->super.poe_pfunc);
    break;
  case POE_CODE_TAG:
    if ((str->super.poe_code->gc_info)<POE_BLACK)
      pgc_blacken_code(str->super.poe_code);
    break;
  default:
    break;
  }
  return;
}

void pgc_traverse_pfunc(poe_pfunc *pf)
{
  /* a pfunc only has a parent table */
  if ((pf->gc_info)==POE_PITCH) return;
  pf->gc_info = POE_PITCH;
  if (pf->parent) {
    if ((pf->parent->gc_info)<POE_BLACK)
      pgc_blacken_table(pf->parent);
    if ((pf->parent->gc_info)<POE_PITCH)
      pgc_traverse_table(pf->parent);
  }
  return;
}
