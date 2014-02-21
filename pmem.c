/* pmem.c
 * Functions for basic memory management (object allocation)
 */ 

#include "pmem.h"
#include "pdata.h"
#include "puniverse.h"
#include "pgc.h"
#include <stdio.h>

/* The basic memory management modus is as follows:
1) If applicable, try to get a structure from the factory.
2) If there is no factory/nothing available in the factory, attempt to allocate
space for a new structure.
3) If the memory allocation fails, force a complete garbage collection cycle.
4) If the memory allocation still fails, return a NULL pointer, which signals
that the application must be halted. (Since we have already tried a complete
GC cycle, Poe cannot itself do anything to rectify the situation.)
*/

poe_pfunc *poe_make_pfunc(poe_universe *U, poe_table *parent, int size, 
			  char *code)
{
  poe_pfunc *ret;
  int i;
  char *s;
  ret = malloc(sizeof(poe_pfunc));
  if (!ret) {
    if (!U) return NULL;
    poe_emergency_gc(U);
    ret = malloc(sizeof(poe_pfunc));
    if (!ret) return NULL;
  }
  ret->control = 0;
  ret->size = size;
  ret->parent = parent;
  s = malloc(size);
  if (!s) {
    if (!U) return NULL;
    poe_emergency_gc(U);
    s = malloc(size);
    if (!s) {
      free(ret);
      return NULL;
    }
  }
  ret->code = s;
  for (i=0; i<size; i++)
    s[i] = code[i];

  U->gc_hub.debt ++; // register debt
    
  return ret;
}

poe_code *poe_make_code(poe_universe *U, int size, char *code)
{
  poe_code *ret;
  int i;
  char *s;
  ret = malloc(sizeof(poe_code));
  if (!ret) {
    if (!U) return NULL;
    poe_emergency_gc(U);
    ret = malloc(sizeof(poe_code));
    if (!ret) return NULL;
  }
  ret->control = 0;
  ret->size = size;
  s = malloc(size);
  if (!s) {
    if (!U) return NULL;
    poe_emergency_gc(U);
    s = malloc(size);
    if (!s) {
      free(ret);
      return NULL;
    }
  }
  ret->code = s;
  for (i=0; i<size; i++)
    s[i] = code[i];
  U->gc_hub.debt ++; // register debt 
  return ret;
}

poe_code *poe_make_code_from_buf(poe_universe *U, int sz, char *code)
{
  poe_code *ret;
  ret = malloc(sizeof(poe_code));
  if (!ret) {
    if (!U) return NULL;
    poe_emergency_gc(U);
    ret = malloc(sizeof(poe_code));
    if (!ret) return NULL;
  }
  ret->control = 0;
  ret->code = code;
  ret->size = sz;
  U->gc_hub.debt ++; // register debt 
  return ret;
}

poe_str *poe_make_str(poe_universe *U)
{
  poe_str *ret, **stack;
  int index;
  char *s;
  if (U) {
    stack = U->gc_hub.str_factory;
    index = U->gc_hub.str_factory_index;
    if (index>=0) {
      U->gc_hub.str_factory_index --;
      return stack[index];
    } 
  } 
  ret = malloc(sizeof(poe_str));
  if (!ret) {
    if (!U) return NULL;
    poe_emergency_gc(U);
    ret = malloc(sizeof(poe_str));
    if (!ret) return NULL;
  }
  ret->control = 0;
  ret->super_tag = POE_UNDEF_TAG;
  ret->len = 0;
  ret->size = STR_MIN_SIZE;
  ret->meta = NULL;
  s = malloc(STR_MIN_SIZE);
  if (!s) {
    if (!U) return NULL;
    poe_emergency_gc(U);
    s = malloc(STR_MIN_SIZE);
    if (!s) {
      free(ret);
      return NULL;
    }
  }
  s[0] = '\0';
  ret->s = s;

  if (U) U->gc_hub.debt ++; // register debt
  return ret;
}

poe_arr *poe_make_arr(poe_universe *U)
{
  poe_arr *ret, **stack;
  int index, x;
  tagged_poe_obj *a;
  if (U) {
    stack = U->gc_hub.arr_factory;
    index = U->gc_hub.arr_factory_index;
    if (index>=0) {
      U->gc_hub.arr_factory_index --;
      return stack[index];
    }
  } 
  ret = malloc(sizeof(poe_arr));
  if (!ret) {
    if (!U) return NULL;
    poe_emergency_gc(U);
    ret = malloc(sizeof(poe_arr));
    if (!ret) return NULL;
  }
  ret->control = 0;
  ret->super_tag = POE_UNDEF_TAG;
  ret->max = -1;
  ret->size = ARR_MIN_SIZE;
  ret->meta = NULL;
  a = malloc(sizeof(tagged_poe_obj) * ARR_MIN_SIZE);
  if (!a) {
    if (!U) return NULL;
    poe_emergency_gc(U);
    a = malloc(sizeof(tagged_poe_obj) * ARR_MIN_SIZE);
    if (!a) {
      free(ret);
      return NULL;
    }
  }
  for (x=0; x<ARR_MIN_SIZE; x++)
    a[x].tag = POE_UNDEF_TAG;
  ret->a = a;
  if (U) U->gc_hub.debt ++; // register debt
  return ret;
}

poe_table *poe_make_table(poe_universe *U)
{
  poe_table *ret, **stack;
  int index, x;
  poe_obj_list **t;
  if (U) {
    stack = U->gc_hub.table_factory;
    index = U->gc_hub.table_factory_index;
    if (index>=0) {
      U->gc_hub.table_factory_index --;
      return stack[index];
    }
  }
  ret = malloc(sizeof(poe_table));
  if (!ret) { 
    if (!U) return NULL;
    poe_emergency_gc(U);
    ret = malloc(sizeof(poe_table));
    if (!ret) return NULL;
  }
  ret->control = 0;
  ret->super_tag = POE_UNDEF_TAG;
  ret->n = 0;
  ret->size = TABLE_MIN_SIZE;
  ret->meta = NULL;
  t = malloc(sizeof(poe_obj_list *) * TABLE_MIN_SIZE);
  if (!t) {
    if (!U) return NULL;
    poe_emergency_gc(U);
    t = malloc(sizeof(poe_obj_list *) * TABLE_MIN_SIZE);
    if (!t) {
      free(ret);
      return NULL;
    }
  }
  for (x=0; x<TABLE_MIN_SIZE; x++)
    t[x] = NULL;
  ret->t = t;
  if (U) U->gc_hub.debt ++; // register debt
  return ret;
}

poe_obj_list *poe_obj_list_cons(poe_universe *U, char tag, poe_obj first,
				poe_obj_list **head, char *s)
{
  poe_obj_list *ret, **stack = U->gc_hub.list_factory, *next = *head;
  int index = U->gc_hub.list_factory_index;
  if (index>=0) {
    U->gc_hub.list_factory_index --;
    ret = stack[index];
  } else {
    ret = malloc(sizeof(poe_obj_list));
    if (!ret) {
      if (!U) return NULL;
      poe_emergency_gc(U);
      ret = malloc(sizeof(poe_obj_list));
      if (!ret) return NULL;
    }
  }
  ret->tag = tag;
  ret->first = first;
  ret->next = next;
  ret->prev = NULL;
  ret->head = head;
  strncpy(ret->s,s,32);
  *head = ret;
  if (next) next->prev = ret;
  if (U) U->gc_hub.debt ++; // register debt
  return ret;
}

void poe_obj_list_attach(poe_obj_list *node, poe_obj_list **head)
{
  poe_obj_list *next = *head;
  *head = node;
  node->prev = NULL;
  node->head = head;
  node->next = next;
  if (next)  // case: there is a node here
    next->prev = node;
  return;
}

void poe_obj_list_remove(poe_universe *U, poe_obj_list *l)
{
  // if item is not first in list, change next field of prev
  if (l->prev)
    l->prev->next = l->next;
  // if item is first in list, change head to point to next
  else
    *(l->head) = l->next;
  /* if item is not last in list, change prev. field of next */
  if (l->next)
    l->next->prev = l->prev;
  free(l);
  return;
}


/* FREES GIVEN POINTER AS A SIDE EFFECT */

unsigned char *resize_uchar_stack(poe_universe * U, unsigned char *old, 
				  int oldsize, int mode, int gc)
{
  unsigned char *new;
  int newsize = mode?oldsize*2:oldsize/2, i;
  new = malloc(newsize);
  if (!new) {
    if (!gc) return NULL;
    if (!U) return NULL;
    poe_emergency_gc(U);
    new = malloc(newsize);
    if (!new) return NULL;
  }
  for (i=0; i<min(oldsize,newsize); i++)
    new[i] = old[i];
  free(old);
  return new;
}

char *resize_char_stack(poe_universe * U, char *old, int oldsize, int mode, 
			int gc)
{
  char *new;
  int newsize = mode?oldsize*2:oldsize/2, i;
  new = malloc(newsize);
  if (!new) {
    if (!gc) return NULL;
    if (!U) return NULL;
    poe_emergency_gc(U);
    new = malloc(newsize);
    if (!new) return NULL;
  }
  for (i=0; i<min(oldsize,newsize); i++)
    new[i] = old[i];
  free(old);
  return new;
}

int *resize_int_stack(poe_universe *U, int *old, int oldsize, int mode, int gc)
{
  int *new;
  int newsize = mode?oldsize*2:oldsize/2, i;
  new = malloc(newsize * sizeof(int));
  if (!new) {
    if (!gc) return NULL;
    if (!U) return NULL;
    poe_emergency_gc(U);
    new = malloc(newsize * sizeof(int));
    if (!new) return NULL;
  }
  for (i=0; i<min(oldsize,newsize); i++)
    new[i] = old[i];
  free(old);
  return new;
}

while_obj *resize_while_stack(poe_universe * U, while_obj *old, int oldsize, 
			int mode, int gc)
{
  while_obj *new;
  int newsize = mode?oldsize*2:oldsize/2, i;
  new = malloc(sizeof(while_obj) * newsize);
  if (!new) {
    if (!gc) return NULL;
    if (!U) return NULL;
    poe_emergency_gc(U);
    new = malloc(sizeof(int) * newsize);
    if (!new) return NULL;
  }
  for (i=0; i<min(oldsize,newsize); i++)
    new[i] = old[i];
  free(old);
  return new;
}

/* FREES GIVEN TAGGED_POE_OBJ * AS A SIDE EFFECT */
tagged_poe_obj *resize_tpo_stack(poe_universe *U, tagged_poe_obj *old, 
				 int oldsize, int mode, int gc)
{
  tagged_poe_obj *new;
  int newsize = mode?oldsize*2:oldsize/2, i;
  new = malloc(sizeof(tagged_poe_obj) * newsize);
  if (!new) {
    if (!gc) return NULL;
    if (!U) return NULL;
    poe_emergency_gc(U);
    new = malloc(sizeof(tagged_poe_obj) * newsize);
    if (!new) return NULL;
  }
  for (i=0; i<min(oldsize,newsize); i++)
    new[i] = old[i];
  free(old);
  return new;
}

for_obj *resize_for_stack(poe_universe *U, for_obj *old, int oldsize, int mode,
			  int gc)
{
  for_obj *new;
  int newsize = mode?oldsize*2:oldsize/2, i;
  new = malloc(sizeof(for_obj) * newsize);
  if (!new) {
    if (!U) return NULL;
    if (!gc) return NULL;
    poe_emergency_gc(U);
    new = malloc(sizeof(for_obj) * newsize);
    if (!new) return NULL;
  }
  for (i=0; i<min(oldsize,newsize); i++)
    new[i] = old[i];
  free(old);
  return new;
}

poe_arr **resize_arr_stack(poe_universe *U, poe_arr **old, int oldsize,
			   int mode, int gc)
{
  poe_arr **new;
  int newsize = mode?oldsize*2:oldsize/2, i;
  new = malloc(sizeof(poe_arr) * newsize);
  if (!new) {
    if (!gc) return NULL;
    if (!U) return NULL;
    poe_emergency_gc(U);
    new = malloc(sizeof(poe_arr) * newsize);
    if (!new) return NULL;
  }
  for (i=0; i<min(oldsize,newsize); i++)
    new[i] = old[i];
  free(old);
  return new;
}

poe_table **resize_table_stack(poe_universe *U, poe_table **old, int oldsize,
			       int mode, int gc)
{
  poe_table **new;
  int newsize = mode?oldsize*2:oldsize/2, i;
  new = malloc(sizeof(poe_arr) * newsize);
  if (!new) {
    if (!gc) return NULL;
    if (!U) return NULL;
    poe_emergency_gc(U);
    new = malloc(sizeof(poe_arr) * newsize);
    if (!new) return NULL;
  }
  for (i=0; i<min(oldsize,newsize); i++)
    new[i] = old[i];
  free(old);
  return new;
}

/* double or halve the size of the given stack in the given Poe universe.
Return 1 on success and 0 on failure. The job of this function, regardless
of stack, is always the same: 1) Locate the stack being requested,
2) Allocate double/half the space for a new stack, 3) copy the elements of the
stack from the old to the new, 4) free the old stack, and 5) update
the universe with the new size. The int stack argument expects an enum
POE_STACKS as given in pexec.h. Whether the function allocates half or double
as much space for the new stack depends on the mode argument: POE_BIG (1)
to double, POE_TINY (2) to halve. */
int resize_stack(poe_universe *U, int stack, int mode, int gc)
{
  int *int_stack;
  char *char_stack;
  unsigned char *uchar_stack;
  while_obj *while_stack;
  tagged_poe_obj *tpo_stack;
  for_obj *for_stack;
  poe_arr **arr_stack;
  poe_table **table_stack;
  switch (stack) {
  case POE_ARGC_STACK:
    int_stack = resize_int_stack(U, U->argc_stack_base, U->argc_stack_size,
				  mode, gc);
    if (!int_stack) return 0;
    U->argc_stack_base = int_stack;
    if (mode) U->argc_stack_size *= 2;
    else U->argc_stack_size /= 2;
    break;
  case POE_LOCALS_STACK:
    table_stack = resize_table_stack(U, U->locals_stack_base,
				      U->locals_stack_size, mode, gc);
    if (!table_stack) return 0;
    U->locals_stack_base = table_stack;
    if (mode) U->while_stack_size *= 2;
    else U->while_stack_size /= 2;
    break;
  case POE_WHILE_STACK: 
    while_stack = resize_while_stack(U, U->while_stack_base, U->while_stack_size,
				  mode, gc);
    if (!while_stack) return 0;
    U->while_stack_base = while_stack;
    if (mode) U->while_stack_size *= 2;
    else U->while_stack_size /= 2;
    break;
  case POE_FOR_STACK:
    for_stack = resize_for_stack(U, U->for_stack_base, U->for_stack_size,
				  mode, gc);
    if (!for_stack) return 0;
    U->for_stack_base = for_stack;
    if (mode) U->for_stack_size *= 2;
    else U->for_stack_size /= 2;
    break;
  case POE_ARGV_STACK:
    arr_stack = resize_arr_stack(U, U->argv_stack_base, U->argv_stack_size,
				  mode, gc);
    if (!arr_stack) return 0;
    U->argv_stack_base = arr_stack;
    if (mode) U->argv_stack_size *= 2;
    else U->argv_stack_size /= 2;
    break;
  case POE_MAIN_STACK: 
    tpo_stack = resize_tpo_stack(U, U->main_stack_base, U->main_stack_size,
				  mode, gc);
    if (!tpo_stack) return 0;
    U->main_stack_base = tpo_stack;
    if (mode) U->main_stack_size *= 2;
    else U->main_stack_size /= 2;
    break;
  case POE_OBJ_STACK: 
    tpo_stack = resize_tpo_stack(U, U->gc_hub.object_stack_base,
				  U->gc_hub.object_stack_size, mode, gc);
    if (!tpo_stack) return 0;
    U->gc_hub.object_stack_base = tpo_stack;
    if (mode) U->gc_hub.object_stack_size *= 2;
    else U->gc_hub.object_stack_size /= 2;
    break;
  case POE_BUF_STACK:
    tpo_stack = resize_tpo_stack(U, U->gc_hub.buffer_stack_base,
				  U->gc_hub.buffer_stack_size, mode, gc);
    if (!tpo_stack) return 0;
    U->gc_hub.buffer_stack_base = tpo_stack;
    if (mode) U->gc_hub.buffer_stack_size *= 2;
    else U->gc_hub.buffer_stack_size /= 2;
    break;
  case POE_LOOP_STACK:
    char_stack = resize_char_stack(U, U->loop_stack_base, U->loop_stack_size,
				   mode, gc);
    if (!char_stack) return 0;
    U->loop_stack_base = char_stack;
    if (mode) U->loop_stack_size *= 2;
    else U->loop_stack_size /= 2;
    break;
  case POE_INFO_STACK:
    uchar_stack = resize_uchar_stack(U, U->info_stack_base, U->info_stack_size,
				   mode, gc);
    if (!uchar_stack) return 0;
    U->info_stack_base = uchar_stack;
    if (mode) U->info_stack_size *= 2;
    else U->info_stack_size /= 2;
    break;
  }
  return 1;
}

int enlarge_stack(poe_universe *U, int stack, int gc)
{
  return resize_stack(U,stack,POE_BIG,gc);
}

int ensmall_stack(poe_universe *U, int stack, int gc)
{
  return resize_stack(U,stack,POE_TINY,gc);
}
