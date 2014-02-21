/* pstring.c
 *
 */

#include "pstring.h"
#include "pdata.h"
#include "puniverse.h"
#include "pexec.h"
#include "pdo1.h"
#include "pdo2.h"
#include <string.h>
#include <stdio.h>

int poe_loadstrlib(poe_universe *U)
{
  poe_table *string = poe_make_table(U);
  if (!string) return 0;
  if (!poe_table_rawset(U,string,"cmp",POE_CFUNC_TAG,(poe_obj)&pstring_cmp))
    return 0;
  if (!poe_table_rawset(U,string,"ncmp",POE_CFUNC_TAG,(poe_obj)&pstring_ncmp))
    return 0;
  if (!poe_pushobj(U,(poe_obj)string,POE_TABLE_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"string")) return 0;
  if (!poe_gc_control(U,(poe_obj)string,POE_TABLE_TAG)) return 0;
  return 1;
}

int pstring_cmp(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], res;
  poe_obj arg1, arg2;
  char tag1, tag2;
  if (argc<2) {
    printf("string.cmp: did not receive 2 arguments\n");
    return 0;
  }
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  arg1 = poe_pop(U,&tag1);
  if (tag1!=POE_STR_TAG) {
    printf("string.cmp: argument 1 is not a string\n");
    return 0;
  }
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)1L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  arg2 = poe_pop(U,&tag2);
  if (tag2!=POE_STR_TAG) {
    printf("string.cmp: argument 2 is not a string\n");
  }
  res = strcmp(arg1.poe_str->s,arg2.poe_str->s);
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (!poe_arr_set(U,retv,0,POE_INT_TAG,(poe_obj)(long)res)) return 0;
  U->retv = retv;
  U->retc = 1;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}

int pstring_ncmp(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], res, i;
  poe_obj arg1, arg2, arg3;
  char tag1, tag2, tag3;
  if (argc>3) {
    printf("string.ncmp: did not receive 3 arguments\n");
    return 0;
  }
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  arg1 = poe_pop(U,&tag1);
  if (tag1!=POE_STR_TAG) {
    printf("string.ncmp: argument 1 is not a string\n");
    return 0;
  }
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)1L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  arg2 = poe_pop(U,&tag2);
  if (tag2!=POE_STR_TAG) {
    printf("string.ncmp: argument 2 is not a string\n");
  }
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)2L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  arg3 = poe_pop(U,&tag3);
  switch (tag3) {
  case POE_INT_TAG:
    i = arg3.poe_int;
    break;
  case POE_FLOAT_TAG:
    i = (int)arg3.poe_float;
    break;
  case POE_CHAR_TAG:
    i = arg3.poe_char;
    break;
  default:
    printf("string.ncmp: argument 3 is not a number\n");
  }
  res = strncmp(arg1.poe_str->s,arg2.poe_str->s,i);
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (!poe_arr_set(U,retv,0,POE_INT_TAG,(poe_obj)(long)res)) return 0;
  U->retv = retv;
  U->retc = 1;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}

