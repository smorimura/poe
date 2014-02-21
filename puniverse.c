#include "pdata.h"
#include "puniverse.h"
#include "pgc.h"

#include <stdio.h>

poe_universe *poe_big_bang(void)
{
  poe_universe *U = malloc(sizeof(poe_universe));
  if (!U) return NULL;

  U->global_table = poe_make_table(NULL);
  if (!U->global_table) return NULL;

  U->locals_stack_base = malloc(sizeof(poe_table *) * LOCALS_STACK_SIZE);
  if (!U->locals_stack_base) return NULL;
  U->locals_stack_index = 0;
  U->locals_stack_base[0] = U->global_table;
  U->locals_stack_size = LOCALS_STACK_SIZE;

  U->main_stack_base = malloc(sizeof(tagged_poe_obj) * EXEC_STACK_SIZE);
  if (!U->main_stack_base) return NULL;
  U->main_stack_index = -1;
  U->main_stack_size = EXEC_STACK_SIZE;

  U->argv_stack_base = malloc(sizeof(poe_arr *) * ITER_STACK_SIZE);
  if (!U->argv_stack_base) return NULL;
  U->argv_stack_index = -1;
  U->argv_stack_size = ITER_STACK_SIZE;

  U->argc_stack_base = malloc(sizeof(int) * ITER_STACK_SIZE);
  if (!U->argc_stack_base) return NULL;
  U->argc_stack_index = -1;
  U->argc_stack_size = ITER_STACK_SIZE;

  U->while_stack_base = malloc(sizeof(while_obj) * ITER_STACK_SIZE);
  if (!U->while_stack_base) return NULL;
  U->while_stack_index = -1;
  U->while_stack_size = ITER_STACK_SIZE;

  U->for_stack_base = malloc(sizeof(for_obj) * ITER_STACK_SIZE);
  if (!U->for_stack_base) return NULL;
  U->for_stack_index = -1;
  U->for_stack_size = ITER_STACK_SIZE;

  U->loop_stack_base = malloc(ITER_STACK_SIZE);
  if (!U->loop_stack_base) return NULL;
  U->loop_stack_index = -1;
  U->loop_stack_size = ITER_STACK_SIZE;

  U->info_stack_base = malloc(ITER_STACK_SIZE);
  if (!U->info_stack_base) return NULL;
  U->info_stack_index = 0;
  U->info_stack_base[0] = 0;
  U->info_stack_size = ITER_STACK_SIZE;

  U->retv = NULL;
  U->retc = 0;

  U->gc_hub.object_stack_base = malloc(sizeof(tagged_poe_obj) * OBJ_STACK_SIZE);
  if (!U->gc_hub.object_stack_base) return NULL;  
  U->gc_hub.object_stack_index = -1;
  U->gc_hub.object_stack_size = OBJ_STACK_SIZE;

  U->gc_hub.buffer_stack_base = malloc(sizeof(tagged_poe_obj) * BUF_STACK_SIZE);
  if (!U->gc_hub.buffer_stack_base) return NULL;
  U->gc_hub.buffer_stack_index = -1;
  U->gc_hub.buffer_stack_size = BUF_STACK_SIZE;

  U->gc_hub.table_factory_index = -1;
  U->gc_hub.str_factory_index = -1;
  U->gc_hub.arr_factory_index = -1;
  U->gc_hub.list_factory_index = -1;

  U->gc_hub.debt = 0;
  U->gc_hub.new_count = 0;
  U->gc_hub.phase = 1;
  U->gc_hub.phase_index = 0;
  U->gc_hub.phase_sub = 0;
  U->gc_hub.major_offset = POE_DEFAULT_MAJ_OFFSET;
  U->gc_hub.cycle_count = POE_DEFAULT_MAJ_OFFSET;
  U->gc_hub.timer = POE_DEFAULT_TIMER;
  U->gc_hub.timer_count = POE_DEFAULT_TIMER;
  U->gc_hub.step_factor = POE_DEFAULT_STEP_FACTOR;
  U->gc_hub.min_step = POE_DEFAULT_MIN_STEP;
  U->gc_hub.bufsize = 0;
  U->gc_hub.running = 1;

  if (!poe_gc_control(U,(poe_obj)U->global_table,POE_TABLE_TAG)) return NULL;

  return U;
}

void poe_destroy_my_objects(poe_universe *U)
{
  int index, i, j;
  tagged_poe_obj *stack, tpo;
  poe_obj_list *l, *next;
  index = U->gc_hub.object_stack_index;
  stack = U->gc_hub.object_stack_base;
  for (i=0; i<=index; i++) {
    tpo = stack[i];
    switch (tpo.tag) {
    case POE_TABLE_TAG:
      for (j=0; j<tpo.data.poe_table->size; j++) 
	for (l=tpo.data.poe_table->t[j]; l; l = next) {
	  next = l->next;
	  free(l);
	}
      free(tpo.data.poe_table->t);
      free(tpo.data.poe_table);
      break;
    case POE_ARR_TAG:
      free(tpo.data.poe_arr->a);
      free(tpo.data.poe_arr);
      break;
    case POE_STR_TAG:
      free(tpo.data.poe_str->s);
      free(tpo.data.poe_str);
      break;
    case POE_PFUNC_TAG:
      free(tpo.data.poe_pfunc->code);
      free(tpo.data.poe_pfunc);
      break;
    case POE_CODE_TAG:
      free(tpo.data.poe_code->code);
      free(tpo.data.poe_code);
      break;
    default:
      continue;
    }
  }
  index = U->gc_hub.buffer_stack_index;
  stack = U->gc_hub.buffer_stack_base;
  for (i=0; i<=index; i++) {
    tpo = stack[i];
    switch (tpo.tag) {
    case POE_TABLE_TAG:
      for (j=0; j<tpo.data.poe_table->size; j++) 
	for (l=tpo.data.poe_table->t[j]; l; l = next) {
	  next = l->next;
	  free(l);
	}
      free(tpo.data.poe_table->t);
      free(tpo.data.poe_table);
      break;
    case POE_ARR_TAG:
      free(tpo.data.poe_arr->a);
      free(tpo.data.poe_arr);
      break;
    case POE_STR_TAG:
      free(tpo.data.poe_str->s);
      free(tpo.data.poe_str);
      break;
    case POE_PFUNC_TAG:
      free(tpo.data.poe_pfunc->code);
      free(tpo.data.poe_pfunc);
      break;
    case POE_CODE_TAG:
      free(tpo.data.poe_code->code);
      free(tpo.data.poe_code);
      break;
    default:
      continue;
    }
  }
  pgc_cleanout_factories(U);
  return;
}

void poe_heat_death(poe_universe *U)
{
  poe_destroy_my_objects(U);
  free(U->locals_stack_base);
  free(U->main_stack_base);
  free(U->argv_stack_base);
  free(U->argc_stack_base);
  free(U->while_stack_base);
  free(U->for_stack_base);
  free(U->loop_stack_base);
  free(U->info_stack_base);
  free(U->gc_hub.object_stack_base);
  free(U->gc_hub.buffer_stack_base);
  free(U);
  return;
}
