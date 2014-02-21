/* pmath.c: math std library functions
 *
 */

#include "pmath.h"
#include "pdata.h"
#include "puniverse.h"
#include "pgc.h"
#include "pexec.h"
#include <math.h>
#include <stdlib.h>

int poe_loadmath(poe_universe *U)
{
  poe_table *math = poe_make_table(U);
  if (!math) return 0;
  if (!poe_table_rawset(U,math,"abs",POE_CFUNC_TAG,(poe_obj)&pmath_abs))
    return 0;
  if (!poe_table_rawset(U,math,"arccos",POE_CFUNC_TAG,(poe_obj)&pmath_arccos))
    return 0;
  if (!poe_table_rawset(U,math,"arcsin",POE_CFUNC_TAG,(poe_obj)&pmath_arcsin))
    return 0;
  if (!poe_table_rawset(U,math,"arctan",POE_CFUNC_TAG,(poe_obj)&pmath_arctan))
    return 0;
  if (!poe_table_rawset(U,math,"ceiling",POE_CFUNC_TAG,(poe_obj)&pmath_ceiling))
    return 0;
  if (!poe_table_rawset(U,math,"cos",POE_CFUNC_TAG,(poe_obj)&pmath_cos))
    return 0;
  if (!poe_table_rawset(U,math,"cosh",POE_CFUNC_TAG,(poe_obj)&pmath_cosh))
    return 0;
  if (!poe_table_rawset(U,math,"deg",POE_CFUNC_TAG,(poe_obj)&pmath_deg))
    return 0;
  if (!poe_table_rawset(U,math,"exp",POE_CFUNC_TAG,(poe_obj)&pmath_exp))
    return 0;
  if (!poe_table_rawset(U,math,"floor",POE_CFUNC_TAG,(poe_obj)&pmath_floor))
    return 0;
  if (!poe_table_rawset(U,math,"fmod",POE_CFUNC_TAG,(poe_obj)&pmath_fmod))
    return 0;
  if (!poe_table_rawset(U,math,"log",POE_CFUNC_TAG,(poe_obj)&pmath_log))
    return 0;
  if (!poe_table_rawset(U,math,"log10",POE_CFUNC_TAG,(poe_obj)&pmath_log10))
    return 0;
  if (!poe_table_rawset(U,math,"pow",POE_CFUNC_TAG,(poe_obj)&pmath_pow))
    return 0;
  if (!poe_table_rawset(U,math,"pi",POE_FLOAT_TAG,(poe_obj)M_PI)) return 0;
  if (!poe_table_rawset(U,math,"rad",POE_CFUNC_TAG,(poe_obj)&pmath_rad))
    return 0;
  if (!poe_table_rawset(U,math,"rand",POE_CFUNC_TAG,(poe_obj)&pmath_rand))
    return 0;
  if (!poe_table_rawset(U,math,"sin",POE_CFUNC_TAG,(poe_obj)&pmath_sin))
    return 0;
  if (!poe_table_rawset(U,math,"sinh",POE_CFUNC_TAG,(poe_obj)&pmath_sinh))
    return 0;
  if (!poe_table_rawset(U,math,"sqrt",POE_CFUNC_TAG,(poe_obj)&pmath_sqrt))
    return 0;
  if (!poe_table_rawset(U,math,"srand",POE_CFUNC_TAG,(poe_obj)&pmath_srand))
    return 0;
  if (!poe_table_rawset(U,math,"tan",POE_CFUNC_TAG,(poe_obj)&pmath_tan))
    return 0;
  if (!poe_table_rawset(U,math,"tanh",POE_CFUNC_TAG,(poe_obj)&pmath_tanh))
    return 0;
  if (!poe_pushobj(U,(poe_obj)math,POE_TABLE_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"math")) return 0;
  if (!poe_gc_control(U,(poe_obj)math,POE_TABLE_TAG)) return 0;
  return 1;
}

int pmath_abs(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.abs: argument %d is not a number\n",i);
      return 0;
    }
    f = fabs(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_arccos(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.arccos: argument %d is not a number\n",i);
      return 0;
    }
    f = acos(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_arcsin(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.arcsin: argument %d is not a number\n",i);
      return 0;
    }
    f = asin(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_arctan(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.arctan: argument %d is not a number\n",i);
      return 0;
    }
    f = atan(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_ceiling(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.ceiling: argument %d is not a number\n",i);
      return 0;
    }
    f = ceil(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_cos(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.cos: argument %d is not a number\n",i);
      return 0;
    }
    f = cos(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_cosh(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.cosh: argument %d is not a number\n",i);
      return 0;
    }
    f = cosh(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_deg(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.deg: argument %d is not a number\n",i);
      return 0;
    }
    f *= 57.2957795;
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_exp(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.exp: argument %d is not a number\n",i);
      return 0;
    }
    f = exp(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_floor(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.floor: argument %d is not a number\n",i);
      return 0;
    }
    f = floor(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_fmod(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index];
  poe_obj arg1, arg2;
  char tag1, tag2;
  double f1, f2;
  if (argc!=2) {
    printf("math.fmod: did not receive 2 arguments\n");
    return 0;
  }
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  arg1 = poe_pop(U,&tag1);
  switch (tag1) {
  case POE_FLOAT_TAG:
    f1 = arg1.poe_float;
    break;
  case POE_INT_TAG:
    f1 = (double)arg1.poe_int;
    break;
  case POE_CHAR_TAG:
    f1 = (double)arg1.poe_char;
    break;
  default:
    printf("math.fmod: arg 1 is not a number\n");
    return 0;
  }
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)1L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  arg2 = poe_pop(U,&tag2);
  switch (tag2) {
  case POE_FLOAT_TAG:
    f2 = arg2.poe_float;
    break;
  case POE_INT_TAG:
    f2 = (double)arg2.poe_int;
    break;
  case POE_CHAR_TAG:
    f2 = (double)arg2.poe_char;
    break;
  default:
    printf("math.fmod: arg 2 is not a number\n");
    return 0;
  }
  f1 = fmod(f1,f2);
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (!poe_arr_set(U,retv,0,POE_FLOAT_TAG,(poe_obj)f1)) return 0;
  U->retv = retv;
  U->retc = 1;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}

int pmath_log(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.log: argument %d is not a number\n",i);
      return 0;
    }
    f = log(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_log10(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.log10: argument %d is not a number\n",i);
      return 0;
    }
    f = log10(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_pow(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index];
  poe_obj arg1, arg2;
  char tag1, tag2;
  double f1, f2;
  if (argc!=2) {
    printf("math.pow: did not receive 2 arguments\n");
    return 0;
  }
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  arg1 = poe_pop(U,&tag1);
  switch (tag1) {
  case POE_FLOAT_TAG:
    f1 = arg1.poe_float;
    break;
  case POE_INT_TAG:
    f1 = (double)arg1.poe_int;
    break;
  case POE_CHAR_TAG:
    f1 = (double)arg1.poe_char;
    break;
  default:
    printf("math.pow: arg 1 is not a number\n");
    return 0;
  }
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)1L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  arg2 = poe_pop(U,&tag2);
  switch (tag2) {
  case POE_FLOAT_TAG:
    f2 = arg2.poe_float;
    break;
  case POE_INT_TAG:
    f2 = (double)arg2.poe_int;
    break;
  case POE_CHAR_TAG:
    f2 = (double)arg2.poe_char;
    break;
  default:
    printf("math.pow: arg 2 is not a number\n");
    return 0;
  }
  f1 = pow(f1,f2);
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (!poe_arr_set(U,retv,0,POE_FLOAT_TAG,(poe_obj)f1)) return 0;
  U->retv = retv;
  U->retc = 1;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}

int pmath_rad(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.rad: argument %d is not a number\n",i);
      return 0;
    }
    f *= 0.0174532925;
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_rand(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i, nobase = 0;
  poe_obj o;
  char tag;
  if (argc<1) nobase = 1;
  else {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_INT_TAG:
      i = o.poe_int;
      break;
    case POE_CHAR_TAG:
      i = o.poe_char;
      break;
    case POE_FLOAT_TAG:
      i = (int)o.poe_float;
      break;
    default:
      printf("math.srand: arg 1 is not a number\n");
      return 0;
    }
    if (i==0) nobase = 1;
  }
  if (nobase) i = rand();
  else i = rand()%i;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arrset(U)) return 0;
  U->retv = retv;
  U->retc = 1;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}

int pmath_srand(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  if (argc<1) {
    printf("math.srand: 0 arguments received\n");
    return 0;
  }
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  o = poe_pop(U,&tag);
  switch (tag) {
  case POE_INT_TAG:
    i = o.poe_int;
    break;
  case POE_CHAR_TAG:
    i = o.poe_char;
    break;
  case POE_FLOAT_TAG:
    i = (int)o.poe_float;
    break;
  default:
    printf("math.srand: arg 1 is not a number\n");
    return 0;
  }
  srand((unsigned)i);
  retv = poe_make_arr(U);
  if (!retv) return 0;
  U->retv = retv;
  U->retc = 0;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}

int pmath_sin(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.sin: argument %d is not a number\n",i);
      return 0;
    }
    f = sin(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_sinh(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.sinh: argument %d is not a number\n",i);
      return 0;
    }
    f = sinh(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_sqrt(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.sqrt: argument %d is not a number\n",i);
      return 0;
    }
    f = sqrt(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_tan(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.tan: argument %d is not a number\n",i);
      return 0;
    }
    f = tan(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pmath_tanh(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  double f;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_FLOAT_TAG:
      f = o.poe_float;
      break;
    case POE_INT_TAG:
      f = (double)(o.poe_int);
      break;
    case POE_CHAR_TAG:
      f = (double)(o.poe_char);
      break;
    default:
      printf("math.tanh: argument %d is not a number\n",i);
      return 0;
    }
    f = tanh(f);
    if (!poe_pushobj(U,(poe_obj)f,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}
