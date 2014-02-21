/* pdo.c
 * functions for bytecode instruction execution
 * All the PDO functions have pexec- prefixes, and each of them executes a
 * particular PBC instruction. As such, they tend to get their arguments
 * from the stack rather than from arguments to the C function (though there
 * exceptions to this rule). Further, as each of these functions executes a 
 * bytecode instruction, each call decreases the timer, and is liable to
 * trigger a garbage collection step at any time. 
 */

#include "pdata.h"
#include "pmem.h"
#include "puniverse.h"
#include "pexec.h"
#include "pdo1.h"
#include "pdo2.h"
#include "pgc.h"


int pexec_pushint(poe_universe *U, long i)
{
  poe_obj o;
  o.poe_int = i;
  if (!poe_pushobj(U,o,POE_INT_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_pushfloat(poe_universe *U, double f)
{
  poe_obj o;
  o.poe_float = f;
  if (!poe_pushobj(U,o,POE_FLOAT_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_pushchar(poe_universe *U, char c)
{
  poe_obj o;
  o.poe_char = c;
  if (!poe_pushobj(U,o,POE_CHAR_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_pushcfunc(poe_universe *U, int (*cfunc) (poe_universe *) )
{
  poe_obj o;
  o.poe_cfunc = cfunc;
  if (!poe_pushobj(U,o,POE_CFUNC_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_pushtrue(poe_universe *U)
{
  poe_obj o;
  o.poe_int = 1L;
  if (!poe_pushobj(U,o,POE_TRUE_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_pushfalse(poe_universe *U)
{
  poe_obj o;
  o.poe_int = 0L;
  if (!poe_pushobj(U,o,POE_FALSE_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_pushundef(poe_universe *U)
{
  poe_obj o;
  o.poe_int = 0L;
  if (!poe_pushobj(U,o,POE_UNDEF_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_pushnull(poe_universe *U)
{
  poe_obj o;
  o.poe_int = 0L;
  if (!poe_pushobj(U,o,POE_NULL_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_newtab(poe_universe *U)
{
  poe_table *new_table;
  poe_obj o;
  new_table = poe_make_table(U);
  if (!new_table) 
    return POE_FATAL_ERROR;
  o.poe_table = new_table;
  if (!poe_pushobj(U,o,POE_TABLE_TAG)) 
    return POE_FATAL_ERROR;
  if (!poe_gc_control(U,o,POE_TABLE_TAG))
    return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_newarr(poe_universe *U)
{
  poe_arr *new_arr;
  poe_obj o;
  new_arr = poe_make_arr(U);
  if (!new_arr) 
    return POE_FATAL_ERROR;
  o.poe_arr = new_arr;
  if (!poe_pushobj(U,o,POE_ARR_TAG))
    return POE_FATAL_ERROR;
  if (!poe_gc_control(U,(poe_obj)new_arr,POE_ARR_TAG)) 
    return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_newstr(poe_universe *U)
{
  poe_obj o;
  poe_str *new_str;
  new_str = poe_make_str(U);
  if (!new_str) 
    return POE_FATAL_ERROR;
  o.poe_str = new_str;
  if (!poe_pushobj(U,o,POE_STR_TAG))
    return POE_FATAL_ERROR;
  if (!poe_gc_control(U,(poe_obj)new_str,POE_STR_TAG))
    return POE_FATAL_ERROR; 
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_writestr(poe_universe *U, char *stuff, int *offset)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index, ret, o;
  if (stack[index].tag!=POE_STR_TAG)
    return POE_FATAL_ERROR;
  o = strlen(stuff);
  if (offset) *offset += o+2;
  ret = poe_str_write(U,stack[index].data.poe_str,stuff,o);
  pgc_countdown(U);
  return ret;
}

int pexec_pushfunc(poe_universe *U, char *code, int *offset)
{
  int o;
  poe_table *parent;
  poe_pfunc *new_pfunc;
  poe_obj x;
  parent = U->locals_stack_base[U->locals_stack_index];
  new_pfunc = poe_make_pfunc(U,parent,o = (size_code(code,CODEEND)+1),
			     code);
  if (!new_pfunc) 
    return POE_FATAL_ERROR;
  x.poe_pfunc = new_pfunc;
  if (!poe_pushobj(U,x,POE_PFUNC_TAG))
    return POE_FATAL_ERROR;
  if (offset) *offset += o+1;
  if (!poe_gc_control(U,(poe_obj)new_pfunc,POE_PFUNC_TAG)) 
    return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_pushcode(poe_universe *U, char *code, int *offset)
{
  int o;
  poe_code *new_code;
  poe_obj x;
  new_code = poe_make_code(U,o = (size_code(code,CODEEND)+1),code);
  if (!new_code) 
    return POE_FATAL_ERROR;
  x.poe_code = new_code;
  if (!poe_pushobj(U,x,POE_CODE_TAG))
    return POE_FATAL_ERROR;
  if (offset) *offset += o+1;
  if (!poe_gc_control(U,(poe_obj)new_code,POE_CODE_TAG)) 
    return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_argenum(poe_universe *U, char *code, int *offset)
{
  int c;
  poe_obj io, argo;
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index];
  io.poe_int = 0L;
  argo.poe_arr = argv;
  for (c = 0; code[c]!='\0'; io.poe_int ++)  {
    if (!poe_pushobj(U,argo,POE_ARR_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,io,POE_INT_TAG)) return POE_FATAL_ERROR;
    if (!pexec_arracc(U)) return POE_FATAL_ERROR;
    if (!pexec_ass(U,LOCAL,code+c)) return POE_FATAL_ERROR;
    c += strlen(code+c) + 1;
  }
  *offset += 2+c;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_add(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int + b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = (float)a.data.poe_int + 
	b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int + b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_add,a.data,a.tag,b.data,b.tag)) 
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_FLOAT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_float + (float)b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_float + b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_float + (float)b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_add,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_CHAR_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_char + b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_char + b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_CHAR_TAG;
      stack[index-1].data.poe_char = a.data.poe_char + b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_add,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  default:
    U->main_stack_index -= 2;
    if (!poe_binmeta(U,meta_add,a.data,a.tag,b.data,b.tag))
      return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_sub(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int - b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = (float)a.data.poe_int -
	b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int - b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_sub,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_FLOAT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_float - (float)b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_float - b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_float - (float)b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_sub,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_CHAR_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_char - b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_char - b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_CHAR_TAG;
      stack[index-1].data.poe_char = a.data.poe_char - b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_sub,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  default:
    U->main_stack_index -= 2;
    if (!poe_binmeta(U,meta_sub,a.data,a.tag,b.data,b.tag))
      return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_mult(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int * b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = (float)a.data.poe_int * 
	b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int * b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_mult,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_FLOAT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_float * (float)b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_float * b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_float * (float)b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_mult,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_CHAR_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_char * b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      stack[index-1].data.poe_float = a.data.poe_char * b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_CHAR_TAG;
      stack[index-1].data.poe_char = a.data.poe_char * b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_mult,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  default:
    U->main_stack_index -= 2;
    if (!poe_binmeta(U,meta_mult,a.data,a.tag,b.data,b.tag))
      return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_div(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      if (b.data.poe_int==0) return 0;
      stack[index-1].data.poe_int = a.data.poe_int / b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      if ((int)b.data.poe_float==0) return 0;
      stack[index-1].data.poe_float = (float)a.data.poe_int / 
	b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_INT_TAG;
      if (b.data.poe_char==0) return 0;
      stack[index-1].data.poe_int = a.data.poe_int / b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_div,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_FLOAT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      if (b.data.poe_int==0) return 0;
      stack[index-1].data.poe_float = a.data.poe_float / (float)b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      if (b.data.poe_float==0.0) return 0;
      stack[index-1].data.poe_float = a.data.poe_float / b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      if (b.data.poe_char==0) return 0;
      stack[index-1].data.poe_float = a.data.poe_float / (float)b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_div,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_CHAR_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      if (b.data.poe_int==0) return 0;
      stack[index-1].data.poe_int = a.data.poe_char / b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_FLOAT_TAG:
      stack[index-1].tag = POE_FLOAT_TAG;
      if (b.data.poe_float==0.0) return 0;
      stack[index-1].data.poe_float = a.data.poe_char / b.data.poe_float;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_CHAR_TAG;
      if (b.data.poe_char==0) return 0;
      stack[index-1].data.poe_char = a.data.poe_char / b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_div,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  default:
    U->main_stack_index -= 2;
    if (!poe_binmeta(U,meta_div,a.data,a.tag,b.data,b.tag))
      return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_mod(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      if (b.data.poe_int==0) return 0;
      stack[index-1].data.poe_int = a.data.poe_int % b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_INT_TAG;
      if (b.data.poe_char==0) return 0;
      stack[index-1].data.poe_int = a.data.poe_int % b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_mod,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_CHAR_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      if (b.data.poe_char==0) return 0;
      stack[index-1].data.poe_int = a.data.poe_char % b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_CHAR_TAG;
      if (b.data.poe_char==0) return 0;
      stack[index-1].data.poe_char = a.data.poe_char % b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_mod,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  default:
    U->main_stack_index -= 2;
    if (!poe_binmeta(U,meta_mod,a.data,a.tag,b.data,b.tag))
      return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_unm(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    stack[index].data.poe_int = -(a.data.poe_int);
    break;
  case POE_FLOAT_TAG:
    stack[index].data.poe_float = -(a.data.poe_float);
    break;
  case POE_CHAR_TAG:
    stack[index].data.poe_char = -(a.data.poe_char);
    break;
  default:
    U->main_stack_index --;
    if (!poe_unmeta(U,meta_unm,a.data,a.tag))
      return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_and(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  if (poe_truth_val(a.tag)&&poe_truth_val(b.tag)) {
    stack[index-1].tag = POE_TRUE_TAG;
    U->main_stack_index --;
  } else {
    stack[index-1].tag = POE_FALSE_TAG;
    U->main_stack_index --;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_or(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  if (poe_truth_val(a.tag)||poe_truth_val(b.tag)) {
    stack[index-1].tag = POE_TRUE_TAG;
    U->main_stack_index --;
  } else {
    stack[index-1].tag = POE_FALSE_TAG;
    U->main_stack_index --;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_not(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index];
  if (poe_truth_val(a.tag)) 
    stack[index].tag = POE_FALSE_TAG;
  else 
    stack[index].tag = POE_TRUE_TAG;
  pgc_countdown(U);
  return 1;
}


int pexec_band(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int & b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int & b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_band,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_CHAR_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_char & b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_CHAR_TAG;
      stack[index-1].data.poe_char = a.data.poe_char & b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_band,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  default:
    U->main_stack_index -= 2;
    if (!poe_binmeta(U,meta_band,a.data,a.tag,b.data,b.tag))
      return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}


int pexec_bor(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int | b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int | b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_bor,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_CHAR_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_char | b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_CHAR_TAG;
      stack[index-1].data.poe_char = a.data.poe_char | b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_bor,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  default:
    U->main_stack_index -= 2;
    if (!poe_binmeta(U,meta_bor,a.data,a.tag,b.data,b.tag))
      return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}


int pexec_bxor(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int ^ b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int ^ b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_bxor,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_CHAR_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_char ^ b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_CHAR_TAG;
      stack[index-1].data.poe_char = a.data.poe_char ^ b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      if (!poe_binmeta(U,meta_bxor,a.data,a.tag,b.data,b.tag))
	return POE_FATAL_ERROR;
      break;
    }
    break;
  default:
    U->main_stack_index -= 2;
    if (!poe_binmeta(U,meta_bxor,a.data,a.tag,b.data,b.tag))
      return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_bnot(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    stack[index].data.poe_int = ~(a.data.poe_int);
    break;
  case POE_CHAR_TAG:
    stack[index].data.poe_char = ~(a.data.poe_char);
    break;
  default:
    U->main_stack_index --;
    if (!poe_unmeta(U,meta_bnot,a.data,a.tag))
      return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_shr(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int >> b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int >> b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_CHAR_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_char >> b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_CHAR_TAG;
      stack[index-1].data.poe_char = a.data.poe_char >> b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      return POE_FATAL_ERROR;
      break;
    }
    break;
  default:
    return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}


int pexec_shl(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  switch (a.tag) {
  case POE_INT_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int << b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_int << b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      return POE_FATAL_ERROR;
      break;
    }
    break;
  case POE_CHAR_TAG:
    switch (b.tag) {
    case POE_INT_TAG:
      stack[index-1].tag = POE_INT_TAG;
      stack[index-1].data.poe_int = a.data.poe_char << b.data.poe_int;
      U->main_stack_index --;
      break;
    case POE_CHAR_TAG:
      stack[index-1].tag = POE_CHAR_TAG;
      stack[index-1].data.poe_char = a.data.poe_char << b.data.poe_char;
      U->main_stack_index --;
      break;
    default:
      U->main_stack_index -= 2;
      return POE_FATAL_ERROR;
      break;
    }
    break;
  default:
    return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_strlen(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index];
  char methodtag;
  get_metamethod(meta_strlen,a.data,a.tag,&methodtag);
  if (methodtag==POE_UNDEF_TAG) {
    switch (a.tag) {
    case POE_STR_TAG:
      stack[index].tag = POE_INT_TAG;
      stack[index].data.poe_int = a.data.poe_str->len;
      break;
    default:
      return POE_FATAL_ERROR;
      break;
    }
  } else {
    U->main_stack_index --;
    if (!poe_unmeta(U,meta_strlen,a.data,a.tag))
      return POE_FATAL_ERROR;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_arrmax(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index];
  char methodtag;
  get_metamethod(meta_arrmax,a.data,a.tag,&methodtag);
  if (methodtag==POE_UNDEF_TAG) {
    switch (a.tag){
    case POE_ARR_TAG:
      stack[index].tag = POE_INT_TAG;
      stack[index].data.poe_int = a.data.poe_arr->max;
      break;
    case POE_STR_TAG:
      stack[index].tag = POE_INT_TAG;
      stack[index].data.poe_int = a.data.poe_str->len + 1;
      break;
    default:
      return POE_FATAL_ERROR;
      break;
    }
  } else {
    U->main_stack_index --;
    if (!poe_unmeta(U,meta_arrmax,a.data,a.tag))
      return POE_FATAL_ERROR;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_eq(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  char methodtag;
  poe_cmphandler(meta_lt, a.data, a.tag, b.data, b.tag, &methodtag);
  if (methodtag==POE_UNDEF_TAG) {
    switch (a.tag) {
    case POE_INT_TAG:
      switch (b.tag) {
      case POE_INT_TAG:
	if (a.data.poe_int==b.data.poe_int)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else 
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_FLOAT_TAG:
	if (a.data.poe_int==b.data.poe_float)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_CHAR_TAG:
	if (a.data.poe_int==b.data.poe_char)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      default:
	return POE_FATAL_ERROR;
	break;
      }
      break;
    case POE_FLOAT_TAG:
      switch (b.tag) {
      case POE_INT_TAG:
	if (a.data.poe_float==b.data.poe_int)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_FLOAT_TAG:
	if (a.data.poe_float==b.data.poe_float)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_CHAR_TAG:
	if (a.data.poe_float==b.data.poe_char)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      default:
	return POE_FATAL_ERROR;
	break;
      }
      break;
    case POE_CHAR_TAG:
      switch (b.tag) {
      case POE_INT_TAG:
	if (a.data.poe_char==b.data.poe_int)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_FLOAT_TAG:
	if (a.data.poe_char==b.data.poe_float)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_CHAR_TAG:
	if (a.data.poe_char==b.data.poe_char)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      default:
	return POE_FATAL_ERROR;
	break;
      }
      break;
    case POE_TRUE_TAG:
      if (b.tag==POE_TRUE_TAG)
	U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
      else
	U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
      break;
    case POE_FALSE_TAG:
      if (b.tag==POE_FALSE_TAG)
	U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
      else
	U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
      break;
    case POE_NULL_TAG:
      if (b.tag==POE_NULL_TAG)
	U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
      else
	U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
      break;
    case POE_UNDEF_TAG:
      if (b.tag==POE_UNDEF_TAG)
	U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
      else
	U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
      break;
    case POE_FILE_TAG:
      if ((b.tag==POE_FILE_TAG)&&(a.data.poe_file==b.data.poe_file)) 
	U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
      else
	U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
      break;
    case POE_PFUNC_TAG:
      if ((b.tag==POE_PFUNC_TAG)&&(a.data.poe_pfunc==b.data.poe_pfunc)) 
	U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
      else
	U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
      break;
    case POE_CFUNC_TAG:
      if ((b.tag==POE_CFUNC_TAG)&&(a.data.poe_cfunc==b.data.poe_cfunc)) 
	U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
      else
	U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
      break;
    case POE_ARR_TAG:
      if ((b.tag==POE_ARR_TAG)&&(a.data.poe_arr==b.data.poe_arr)) 
	U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
      else
	U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
      break;
    case POE_STR_TAG:
      if ((b.tag==POE_STR_TAG)&&(a.data.poe_str==b.data.poe_str)) 
	U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
      else
	U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
      break;
    case POE_TABLE_TAG:
      if ((b.tag==POE_TABLE_TAG)&&(a.data.poe_table==b.data.poe_table)) 
	U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
      else
	U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
      break;
    }
  } else {
    U->main_stack_index -= 2;
    if (!poe_binmeta(U,meta_eq,a.data,a.tag,b.data,b.tag))
      return POE_FATAL_ERROR;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_lt(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  char methodtag;
  poe_cmphandler(meta_lt, a.data, a.tag, b.data, b.tag, &methodtag);
  if (methodtag==POE_UNDEF_TAG) {
    switch (a.tag) {
    case POE_INT_TAG:
      switch (b.tag) {
      case POE_INT_TAG:
	if (a.data.poe_int<b.data.poe_int)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else 
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_FLOAT_TAG:
	if (a.data.poe_int<b.data.poe_float)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_CHAR_TAG:
	if (a.data.poe_int<b.data.poe_char)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      default:
	return POE_FATAL_ERROR;
	break;
      }
      break;
    case POE_FLOAT_TAG:
      switch (b.tag) {
      case POE_INT_TAG:
	if (a.data.poe_float<b.data.poe_int)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_FLOAT_TAG:
	if (a.data.poe_float<b.data.poe_float)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_CHAR_TAG:
	if (a.data.poe_float<b.data.poe_char)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      default:
	return POE_FATAL_ERROR;
	break;
      }
      break;
    case POE_CHAR_TAG:
      switch (b.tag) {
      case POE_INT_TAG:
	if (a.data.poe_char<b.data.poe_int)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_FLOAT_TAG:
	if (a.data.poe_char<b.data.poe_float)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_CHAR_TAG:
	if (a.data.poe_char<b.data.poe_char)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      default:
	return POE_FATAL_ERROR;
	break;
      }
      break;
    default:
      return POE_FATAL_ERROR;
      break;
    }
  } else {
    U->main_stack_index -= 2;
    if (!poe_binmeta(U,meta_lt,a.data,a.tag,b.data,b.tag))
      return POE_FATAL_ERROR;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_le(poe_universe *U)
{
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tagged_poe_obj a = stack[index-1], b = stack[index];
  char methodtag;
  poe_cmphandler(meta_le, a.data, a.tag, b.data, b.tag, &methodtag);
  if (methodtag==POE_UNDEF_TAG) {
    switch (a.tag) {
    case POE_INT_TAG:
      switch (b.tag) {
      case POE_INT_TAG:
	if (a.data.poe_int<=b.data.poe_int)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else 
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_FLOAT_TAG:
	if (a.data.poe_int<=b.data.poe_float)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_CHAR_TAG:
	if (a.data.poe_int<=b.data.poe_char)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      default:
	return POE_FATAL_ERROR;
	break;
      }
      break;
    case POE_FLOAT_TAG:
      switch (b.tag) {
      case POE_INT_TAG:
	if (a.data.poe_float<=b.data.poe_int)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_FLOAT_TAG:
	if (a.data.poe_float<=b.data.poe_float)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_CHAR_TAG:
	if (a.data.poe_float<=b.data.poe_char)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      default:
	return POE_FATAL_ERROR;
	break;
      }
      break;
    case POE_CHAR_TAG:
      switch (b.tag) {
      case POE_INT_TAG:
	if (a.data.poe_char<=b.data.poe_int)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_FLOAT_TAG:
	if (a.data.poe_char<=b.data.poe_float)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      case POE_CHAR_TAG:
	if (a.data.poe_char<=b.data.poe_char)
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_TRUE_TAG;
	else
	  U->main_stack_base[--(U->main_stack_index)].tag = POE_FALSE_TAG;
	break;
      default:
	return POE_FATAL_ERROR;
	break;
      }
      break;
    default:
      return POE_FATAL_ERROR;
      break;
    }
  } else {
    U->main_stack_index -= 2;
    if (!poe_binmeta(U,meta_le,a.data,a.tag,b.data,b.tag))
      return POE_FATAL_ERROR;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

/* if a pfunc is called, we need to generate a new local symbol table and push
it onto the local stack. (The supertable of that table will be the parent 
symbol table of the pfunc; when the interpreter needs to search for a symbol,
if will traverse the supertables until it reaches the global table.) A RETURN
statement will pop the argv, argc, and locals stacks, so if we hit a codeend,
we'll need to RETURN ourselves. HOWEVER, a cfunc does not need to clear
the argv/argc stacks, so this function needs to take care of them. */
int pexec_call(poe_universe *U)
{  
  tagged_poe_obj *stack = U->main_stack_base;
  int index = U->main_stack_index, result, status;
  tagged_poe_obj a = stack[index];
  poe_table *new_local;
  poe_obj metamethod;
  poe_arr *retv;
  char methodtag, retvtag;
  poe_obj retv_obj;
  switch (a.tag) {
  case POE_PFUNC_TAG:
    new_local = poe_make_table(U);
    if (!new_local) return POE_FATAL_ERROR;
    new_local->super_tag = POE_TABLE_TAG;
    new_local->super = (poe_obj)a.data.poe_pfunc->parent;
    index = U->locals_stack_index;
    if (index+1>=U->locals_stack_size) 
      if (!enlarge_stack(U, POE_LOCALS_STACK, 1))
	return POE_FATAL_ERROR;
    if (U->info_stack_index+1>=U->info_stack_size)
      if (!enlarge_stack(U, POE_INFO_STACK, 1))
	return POE_FATAL_ERROR;
    U->locals_stack_index++;
    U->locals_stack_base[index+1] = new_local;
    U->info_stack_index++;
    U->info_stack_base[U->info_stack_index] = 0;
    if (!poe_gc_control(U,(poe_obj)new_local,POE_TABLE_TAG)) 
      return POE_FATAL_ERROR;
    U->main_stack_index--; // remove the function from the stack
    result = poe_exec_code(U, a.data.poe_pfunc->code,NULL);
    if (result==POE_RETURN) { /* we need to push the first returned value
				 onto the stack */
      retv_obj = poe_arr_get(U->retv, 0L, &retvtag);
      if (!poe_pushobj(U,retv_obj,retvtag)) return POE_FATAL_ERROR;
      pgc_countdown(U);
      return POE_CONTINUE;
    } else if (result==POE_CODEEND) { /* we need to execute our own return
				       statement and return an empty retv */
      pexec_return(U);
      retv = poe_make_arr(U);
      if (!retv) return POE_FATAL_ERROR;
      U->retv = retv;
      retv_obj.poe_arr = retv;
      if (!poe_gc_control(U,retv_obj,POE_ARR_TAG)) return POE_FATAL_ERROR;
      U->retc = 0L;
      retv_obj.poe_int = 0;
      if (!poe_pushobj(U,retv_obj,POE_UNDEF_TAG)) return POE_FATAL_ERROR;
      pgc_countdown(U);
      return POE_CONTINUE;
    }
    return POE_FATAL_ERROR;
    break;
  case POE_CFUNC_TAG:
    /* no new local table */
    U->main_stack_index --;
    result = (a.data.poe_cfunc)(U);
    U->argv_stack_base[U->argv_stack_index--] = NULL;
    U->argc_stack_base[U->argc_stack_index--] = -1;
    retv_obj = poe_arr_get(U->retv, 0L, &retvtag);
    if (!poe_pushobj(U,retv_obj,retvtag)) return POE_FATAL_ERROR;
    pgc_countdown(U);
    return result;
  default:
    metamethod = get_metamethod(meta_call,a.data,a.tag,&methodtag);
    if (methodtag!=POE_UNDEF_TAG) {
      U->main_stack_index --;
      if (!poe_pushobj(U,metamethod,methodtag)) return POE_FATAL_ERROR;
      if (!(status = pexec_call(U))) return POE_FATAL_ERROR;
      if (status==POE_HALT) return POE_HALT;
    } else {
      U->argv_stack_base[U->argv_stack_index--] = NULL;
      U->argc_stack_base[U->argc_stack_index--] = -1;
    }
    pgc_countdown(U);
    return POE_CONTINUE;
    break;
  }
}

int pexec_return(poe_universe *U)
{
  while (U->info_stack_base[U->info_stack_index]>0)
    pexec_break(U,NULL);
  U->info_stack_index --;
  U->locals_stack_base[U->locals_stack_index] = NULL;
  U->argv_stack_base[U->argv_stack_index] = NULL;
  U->argc_stack_base[U->argc_stack_index] = -1;
  U->argv_stack_index --;
  U->locals_stack_index --;
  U->argc_stack_index --;
  pgc_countdown(U);
  return POE_RETURN;
}

int pexec_do(poe_universe *U)
{
  int index = U->main_stack_index, result, status;
  tagged_poe_obj *stack = U->main_stack_base, a = stack[index];
  poe_obj meta, argo;
  char metatag;
  poe_arr *argv;
  U->main_stack_index --;
  switch (a.tag) {
  case POE_CODE_TAG:
    result = poe_exec_code(U,a.data.poe_code->code,NULL);
    pgc_countdown(U);
    if (result==POE_CODEEND) return POE_CONTINUE;
    else return result;
    break;
  default:
    meta = get_metamethod(meta_do, a.data, a.tag, &metatag);
    if (metatag==POE_UNDEF_TAG) return POE_FATAL_ERROR;
    argv = poe_make_arr(U);
    if (!argv) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,0,a.tag,a.data)) return POE_FATAL_ERROR;
    if (!poe_pushargv(U,argv)) return POE_FATAL_ERROR;
    argo.poe_arr = argv;
    if (!poe_gc_control(U,argo,POE_ARR_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushargc(U,1)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,meta,metatag)) return POE_FATAL_ERROR;
    if (!(status = pexec_call(U))) return POE_FATAL_ERROR;
    if (status==POE_HALT) return POE_HALT;
    pgc_countdown(U);
    return POE_CONTINUE;
  }
}

int pexec_swap(poe_universe *U)
{
  tagged_poe_obj tmp, *stack = U->main_stack_base;
  int index = U->main_stack_index;
  tmp = stack[index];
  stack[index] = stack[index-1];
  stack[index-1] = tmp;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_del(poe_universe *U)
{
  U->main_stack_index --;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_copy(poe_universe *U)
{
  if (!poe_pushobj(U,U->main_stack_base[U->main_stack_index].data,
		   U->main_stack_base[U->main_stack_index].tag))
    return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}
