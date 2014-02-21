/* puniverse.h
 * Definitions for creating, maintaining basic Poe environments
 */

#ifndef PUNIVERSE_H_
#define PUNIVERSE_H_

#include <stdio.h>
#include "pdata.h"

#define FACTORY_SIZE 32
#define ITER_STACK_SIZE 32
#define OBJ_STACK_SIZE 256
#define BUF_STACK_SIZE 128
#define EXEC_STACK_SIZE 128
#define LOCALS_STACK_SIZE 32

enum POE_LOOPS {
  FOR_LOOP, WHILE_LOOP
};

typedef struct for_obj {
  unsigned int tag : 6; /* type information for the iterable */
  poe_obj iterable;
  poe_str *key_sname;
  poe_str *val_sname;
  int index; // current index of iteration, up to size of iterable
  poe_obj_list *node; // current list node of iteration (for tables)
  int offset;
  int break_offset;
} for_obj;

typedef struct while_obj {
  char *expr;
  int offset;
  int break_offset;
} while_obj;

typedef struct poe_gc_hub {

  tagged_poe_obj *object_stack_base; // stack of all compound objects in program
  int object_stack_index;
  int object_stack_size;

  tagged_poe_obj *buffer_stack_base; /* more compound objects; the gc moves
					objects from here to the object stack 
					as needed */
  int buffer_stack_index;
  int buffer_stack_size;

  /* FACTORIES: allocated data structures may be retained here rather than
being freed back to the OS; essentially, the flow of object allocation here is
1) Create an object.
2) When it is no longer in use, the garbage collector will collect it.
3) The GC may decide to free the object or push it into the factory stack.
4) When you need an object of the same type later, check the factory. If there
is not a NULL at the top of the stack, the object was pushed into the factory
and you may take it again.
Poe tables, strings, and arrays, as well as PCL poe_obj_lists are candidates
for placement in a factory. During an emergency collection, all the factories
are cleared.
  */ 

  poe_table *table_factory[FACTORY_SIZE];
  int table_factory_index;
  poe_str *str_factory[FACTORY_SIZE];
  int str_factory_index;
  poe_arr *arr_factory[FACTORY_SIZE];
  int arr_factory_index;
  poe_obj_list *list_factory[FACTORY_SIZE];
  int list_factory_index;

  int debt; /* the number of allocated objects since the last step; used so
	       that, on garbage collection steps, the GC knows to touch 
	       step_factor*debt objects. The debt is incremented with each
	       allocated object. */
  int new_count; /* the number of objects added to the object list since the 
		    last phase. This number is incremented every time an
		    object is added to the buffer list. */
  int phase; // current phase of execution; could be 0 on major cycles
  int phase_index; /* phase index of execution (this number has different
		      meanings for different phases) */
  int phase_sub;   /* current sub-phase of execution */
  int major_offset; /* number of minor cycles between major cycles; set to 0
		       for no minor cycles. */
  int cycle_count; // number of cycles until the next major cycle. if this is
  // 0, we are curretly executing a major cycle.
  int timer; // number of instructions between gc steps.
  int timer_count; // number of instructions left until a gc step.
  double step_factor; /* this, multiplied by the debt, is the approximate number
			 of items to touch in a gc step. */
  int min_step; /* the minimum number of items to touch during a gc step (useful
		   if unusually few objects were created between steps. */
  int bufsize; /* saved buffer size for use by the gc in phase 0 */

  int running;
} poe_gc_hub;

struct poe_universe {
  poe_table *global_table; // GLOBAL TABLE

  poe_table **locals_stack_base; /* stack of local tables; each function call 
				    adds a table. We need these for garbage
				    collection. */
  int locals_stack_index;
  int locals_stack_size;

  tagged_poe_obj *main_stack_base; /* EXECUTION STACK; e.g., when you "push an
				      int onto the stack, this is where it ends
				      up */
  int main_stack_index;
  int main_stack_size;

  poe_arr **argv_stack_base; /* argv stack holds argv's for function calls; 
				the current argv is at the top of the stack */
  int argv_stack_index;
  int argv_stack_size;

  int *argc_stack_base; /* the current argc is at the top of the stack */
  int argc_stack_index;
  int argc_stack_size;

  while_obj *while_stack_base; 
  int while_stack_index;
  int while_stack_size;

  for_obj *for_stack_base; // current for info is at top of stack
  int for_stack_index;
  int for_stack_size;

  char *loop_stack_base;
  int loop_stack_index;
  int loop_stack_size;

  unsigned char *info_stack_base;
  int info_stack_index;
  int info_stack_size;

  poe_arr *retv;
  long retc;

  poe_gc_hub gc_hub;
};

poe_universe *poe_big_bang(void);
void poe_heat_death(poe_universe *);
void poe_destroy_my_objects(poe_universe *);
#endif
