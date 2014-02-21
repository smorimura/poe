/* pmem.h
 * definitions for memory management and object creation. In general, pmem
 * functions increase the amount of memory controlled by Poe, while it is
 * the job of pgc to decrease it.
 */

#include <stdio.h>
#include "pdata.h"
#include "puniverse.h"

#ifndef PMEM_H_
#define PMEM_H_

enum POE_STACKS {
  POE_MAIN_STACK, POE_LOCALS_STACK, POE_ARGV_STACK, POE_ARGC_STACK, 
  POE_WHILE_STACK, POE_FOR_STACK, POE_OBJ_STACK, POE_BUF_STACK,
  POE_LOOP_STACK, POE_INFO_STACK
};

enum POE_ENLARGE_MODES {
  POE_TINY = 0, POE_BIG = 1
};

poe_pfunc *poe_make_pfunc(poe_universe *, poe_table *, int, char *);
poe_code *poe_make_code(poe_universe *, int, char *);
poe_code *poe_make_code_from_buf(poe_universe *, int, char *);
poe_str *poe_make_str(poe_universe *);
poe_arr *poe_make_arr(poe_universe *);
poe_table *poe_make_table(poe_universe *);

poe_obj_list *poe_obj_list_cons(poe_universe *, char, poe_obj, poe_obj_list **,
				char *);
void poe_obj_list_attach(poe_obj_list *, poe_obj_list **);
void poe_obj_list_remove(poe_universe *, poe_obj_list *);

unsigned char *resize_uchar_stack(poe_universe *, unsigned char *, int, int, 
				  int);
char *resize_char_stack(poe_universe *, char *, int, int, int);
int *resize_int_stack(poe_universe *, int *, int, int, int);
while_obj *resize_while_stack(poe_universe *, while_obj *, int, int, int);
tagged_poe_obj *resize_tpo_stack(poe_universe *, tagged_poe_obj *, int, int, 
				 int);
for_obj *resize_for_stack(poe_universe *, for_obj *, int, int, int);
poe_arr **resize_arr_stack(poe_universe *, poe_arr **, int, int, int);
poe_table **resize_table_stack(poe_universe *, poe_table **, int, int, int);
int resize_stack(poe_universe *, int, int, int);
int enlarge_stack(poe_universe *, int, int);
int ensmall_stack(poe_universe *, int, int);

#define STR_MIN_SIZE 32
#define ARR_MIN_SIZE 32
#define TABLE_MIN_SIZE 43

#define snap(i) ( (i)<0 ? 0 : i)
#define min(a,b) (((a)<(b))?a:b)

#endif
