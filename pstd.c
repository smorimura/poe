/* pstd.c
 * standard library files
 */

#include "pstd.h"
#include "pdata.h"
#include "puniverse.h"
#include "pmem.h"
#include "pexec.h"
#include "pdo1.h"
#include "pdo2.h"
#include "pbc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int poe_loadglobs(poe_universe *U)
{
  if (!poe_pushobj(U,(poe_obj)&pstd_compile,POE_CFUNC_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"compile")) return 0;
  if (!poe_pushobj(U,(poe_obj)&pstd_print,POE_CFUNC_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"print")) return 0;
  if (!poe_pushobj(U,(poe_obj)&pstd_I,POE_CFUNC_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"I")) return 0;
  if (!poe_pushobj(U,(poe_obj)&pstd_size,POE_CFUNC_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"size")) return 0;
  if (!poe_pushobj(U,(poe_obj)&pstd_time,POE_CFUNC_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"time")) return 0;
  if (!poe_pushobj(U,(poe_obj)&pstd_tochar,POE_CFUNC_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"tochar")) return 0;
  if (!poe_pushobj(U,(poe_obj)&pstd_toint,POE_CFUNC_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"toint")) return 0;
  if (!poe_pushobj(U,(poe_obj)&pstd_tofloat,POE_CFUNC_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"tofloat")) return 0;
  if (!poe_pushobj(U,(poe_obj)&pstd_tostring,POE_CFUNC_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"tostring")) return 0;
  if (!poe_pushobj(U,(poe_obj)&pstd_type,POE_CFUNC_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"type")) return 0;
  return 1;
}

/* Compile source code into temporary file, read that temporary file into a 
   code object */
int pstd_compile(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i, sz;
  poe_code *code;
  poe_obj o;
  char tag;
  FILE *tmp;
  char *src;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_STR_TAG:
      tmp = pbc_compile_from_string(o.poe_str->s);
      if (!tmp) return 0;
      src = pint_load_filef(tmp, &sz);
      if (!src) return 0;
      code = poe_make_code_from_buf(U, sz, src);
      fclose(tmp);
      break;
    case POE_FILE_TAG:
      tmp = tmpfile();
      if (!tmp) return 0;
      if (!pbc_compile(o.poe_file, tmp)) return 0;
      rewind(tmp);
      src = pint_load_filef(tmp, &sz);
      if (!src) return 0;
      code = poe_make_code_from_buf(U, sz, src);
      fclose(tmp);
      break;
    default:
      printf("compile: argument %d is not a string or file\n",i);
      return 0;
    }
    if (!poe_pushobj(U,(poe_obj)code,POE_CODE_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  U->retc = argc;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}

int pstd_I(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index];
  int argc = U->argc_stack_base[U->argc_stack_index];
  U->retv = argv;
  U->retc = argc;
  return 1;
}

int pstd_print(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o, meta;
  char tag, metatag;
  poe_arr *a;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_NULL_TAG:
      printf("null");
      break;
    case POE_TRUE_TAG:
      printf("true");
      break;
    case POE_FALSE_TAG:
      printf("false");
      break;
    case POE_UNDEF_TAG:
      printf("undef");
      break;
    case POE_PFUNC_TAG:
      printf("pfunc@<%p>",o.poe_pfunc);
      break;
    case POE_CFUNC_TAG:
      printf("cfunc@<%p>",o.poe_cfunc);
      break;
    case POE_CODE_TAG:
      printf("code@<%p>",o.poe_code);
      break;
    case POE_FILE_TAG:
      printf("file@<%p>",o.poe_file);
      break;
    case POE_INT_TAG:
      printf("%ld",o.poe_int);
      break;
    case POE_FLOAT_TAG:
      printf("%lf",o.poe_float);
      break;
    case POE_CHAR_TAG:
      printf("%c",o.poe_char);
      break;
    case POE_ARR_TAG:
      meta = get_metamethod(meta_tostr,o,tag,&metatag);
      if (metatag!=POE_UNDEF_TAG) {
	a = poe_make_arr(U);
	if (!a) return 0;
	if (!poe_arr_set(U,a,0,tag,o)) return 0;
	if (!poe_pushargv(U,a)) return 0;
	if (!poe_gc_control(U,(poe_obj)a,POE_ARR_TAG)) return 0;
	if (!poe_pushargc(U,1)) return 0;
	if (!poe_pushobj(U,meta,metatag)) return 0;
	if (!pexec_call(U)) return 0;
	o = poe_pop(U,&tag);
	if (tag!=POE_STR_TAG) return 0;
	fwrite(o.poe_str->s,1,o.poe_str->len,stdout);
      } else {
	printf("array@<%p>",o.poe_arr);
      }
      break;
    case POE_TABLE_TAG:
      meta = get_metamethod(meta_tostr,o,tag,&metatag);
      if (metatag!=POE_UNDEF_TAG) {
	a = poe_make_arr(U);
	if (!a) return 0;
	if (!poe_arr_set(U,a,0,tag,o)) return 0;
	if (!poe_pushargv(U,a)) return 0;
	if (!poe_gc_control(U,(poe_obj)a,POE_ARR_TAG)) return 0;
	if (!poe_pushargc(U,1)) return 0;
	if (!poe_pushobj(U,meta,metatag)) return 0;
	if (!pexec_call(U)) return 0;
	o = poe_pop(U,&tag);
	if (tag!=POE_STR_TAG) return 0;
	fwrite(o.poe_str->s,1,o.poe_str->len,stdout);
      } else {
	printf("table@<%p>",o.poe_table);
      }
      break;
    case POE_STR_TAG:
      meta = get_metamethod(meta_tostr,o,tag,&metatag);
      if (metatag!=POE_UNDEF_TAG) {	
	a = poe_make_arr(U);
	if (!a) return 0;
	if (!poe_arr_set(U,a,0,tag,o)) return 0;
	if (!poe_pushargv(U,a)) return 0;
	if (!poe_gc_control(U,(poe_obj)a,POE_ARR_TAG)) return 0;
	if (!poe_pushargc(U,1)) return 0;
	if (!poe_pushobj(U,meta,metatag)) return 0;
	if (!pexec_call(U)) return 0;
	o = poe_pop(U,&tag);
	if (tag!=POE_STR_TAG) return 0;
	fwrite(o.poe_str->s,1,o.poe_str->len,stdout);
      } else {
	fwrite(o.poe_str->s,1,o.poe_str->len,stdout);
      }
      break;
    }
    if (i+1!=argc) fputc(' ',stdout);
  }
  retv = poe_make_arr(U);
  if (!retv) return 0;
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = 0;
  printf("\n");
  return 1;
}

int pstd_size(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i, sz, j;
  poe_obj o;
  char tag, atag;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_TABLE_TAG:
      sz = o.poe_table->n;
      break;
    case POE_STR_TAG:
      sz = o.poe_str->len;
      break;
    case POE_ARR_TAG:
      sz = 0;
      for (j=0; j<=o.poe_arr->max; j++) {
	if (!poe_pushobj(U,o,POE_ARR_TAG)) return 0;
	if (!poe_pushobj(U,(poe_obj)(long)j,POE_INT_TAG)) return 0;
	if (!pexec_arracc(U)) return 0;
	poe_pop(U,&atag);
	if (atag!=POE_UNDEF_TAG) sz++;
      }
      break;
    default:
      sz = -1;
      break;
    }
    if (!poe_pushobj(U,(poe_obj)(long)sz,POE_INT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pstd_time(poe_universe *U)
{
  poe_arr *retv = poe_make_arr(U);
  if (!retv) return 0;
  if (!poe_pushobj(U,(poe_obj)(long)time(NULL),POE_INT_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arrset(U)) return 0;
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = 1;
  return 1;
}

int pstd_tochar(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o, ret;
  char tag;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_INT_TAG:
      ret.poe_char = (char)o.poe_int;
      break;
    case POE_FLOAT_TAG:
      ret.poe_char = (char)o.poe_float;
      break;
    case POE_CHAR_TAG:
      ret.poe_char = o.poe_char;
      break;
    default:
      return 0;
    }
    if (!poe_pushobj(U,ret,POE_CHAR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pstd_tofloat(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o, ret;
  char tag;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_INT_TAG:
      ret.poe_float = (double)o.poe_int;
      break;
    case POE_FLOAT_TAG:
      ret.poe_float = o.poe_float;
      break;
    case POE_CHAR_TAG:
      ret.poe_float = (double)o.poe_char;
      break;
    case POE_STR_TAG:
      ret.poe_float = (double)atof(o.poe_str->s);
      break;
    default:
      return 0;
    }
    if (!poe_pushobj(U,ret,POE_FLOAT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}


int pstd_toint(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o, ret;
  char tag;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_INT_TAG:
      ret.poe_int = o.poe_int;
      break;
    case POE_FLOAT_TAG:
      ret.poe_int = (long)o.poe_float;
      break;
    case POE_CHAR_TAG:
      ret.poe_int = (long)o.poe_char;
      break;
    case POE_STR_TAG:
      ret.poe_int = (long)atoi(o.poe_str->s);
      break;
    default:
      return 0;
    }
    if (!poe_pushobj(U,ret,POE_INT_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}

int pstd_tostring(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv, *a;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o, ret, meta;
  char tag, metatag, chrbuf[32];
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_STR_TAG:
    case POE_TABLE_TAG:
    case POE_ARR_TAG:
      meta = get_metamethod(meta_tostr,o,tag,&metatag);
      if (metatag!=POE_UNDEF_TAG) {
	a = poe_make_arr(U);
	if (!a) return 0;
	if (!poe_arr_set(U,a,0,tag,o)) return 0;
	if (!poe_pushargv(U,a)) return 0;
	if (!poe_gc_control(U,(poe_obj)a,POE_ARR_TAG)) return 0;
	if (!poe_pushargc(U,1)) return 0;
	if (!poe_pushobj(U,meta,metatag)) return 0;
	if (!pexec_call(U)) return 0;
	ret = poe_pop(U,&tag);
	if (tag!=POE_STR_TAG) return 0;
      } else {
	switch (tag) {
	case POE_STR_TAG:
	  ret.poe_str = o.poe_str;
	  break;
	case POE_TABLE_TAG:
	  ret.poe_str = poe_make_str(U);
	  if (!ret.poe_str) return 0;
	  if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
	  snprintf(chrbuf,32,"table@<%p>",o.poe_table);
	  if (!pexec_writestr(U,chrbuf,NULL)) return 0;
	  ret = poe_pop(U,NULL);
	  break;
	case POE_ARR_TAG:
	  ret.poe_str = poe_make_str(U);
	  if (!ret.poe_str) return 0;
	  if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
	  snprintf(chrbuf,32,"arr@<%p>",o.poe_arr);
	  if (!pexec_writestr(U,chrbuf,NULL)) return 0;
	  ret = poe_pop(U,NULL);
	  break;
	}
      }
      break;
    case POE_INT_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"%ld",o.poe_int);
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_FLOAT_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"%lf",o.poe_float);
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_CHAR_TAG:
      ret.poe_str = poe_make_str(U);
      if (!poe_str_set(U,ret.poe_str,0,o.poe_char)) return 0;
      break;
    case POE_NULL_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"null");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_TRUE_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"true");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_FALSE_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"false");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_UNDEF_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"undef");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_PFUNC_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"pfunc@<%p>",o.poe_pfunc);
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_CFUNC_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"cfunc@<%p>",o.poe_cfunc);
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_CODE_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"code@<%p>",o.poe_code);
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_FILE_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"file@<%p>",o.poe_file);
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    }
    if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}


int pstd_type(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv, *a;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o, ret, meta;
  char tag, metatag, chrbuf[32];
  retv = poe_make_arr(U);
  if (!retv) return 0;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_STR_TAG:
    case POE_TABLE_TAG:
    case POE_ARR_TAG:
      meta = get_metamethod(meta_type,o,tag,&metatag);
      if (metatag!=POE_UNDEF_TAG) {
	a = poe_make_arr(U);
	if (!a) return 0;
	if (!poe_arr_set(U,a,0,tag,o)) return 0;
	if (!poe_pushargv(U,a)) return 0;
	if (!poe_gc_control(U,(poe_obj)a,POE_ARR_TAG)) return 0;
	if (!poe_pushargc(U,1)) return 0;
	if (!poe_pushobj(U,meta,metatag)) return 0;
	if (!pexec_call(U)) return 0;
	o = poe_pop(U,&tag);
	if (tag!=POE_STR_TAG) return 0;
      } else {
	switch (tag) {
	case POE_STR_TAG:
	  ret.poe_str = poe_make_str(U);
	  if (!ret.poe_str) return 0;
	  if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
	  snprintf(chrbuf,32,"str");
	  if (!pexec_writestr(U,chrbuf,NULL)) return 0;
	  ret = poe_pop(U,NULL);
	  break;
	case POE_TABLE_TAG:
	  ret.poe_str = poe_make_str(U);
	  if (!ret.poe_str) return 0;
	  if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
	  snprintf(chrbuf,32,"table");
	  if (!pexec_writestr(U,chrbuf,NULL)) return 0;
	  ret = poe_pop(U,NULL);
	  break;
	case POE_ARR_TAG:
	  ret.poe_str = poe_make_str(U);
	  if (!ret.poe_str) return 0;
	  if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
	  snprintf(chrbuf,32,"array");
	  if (!pexec_writestr(U,chrbuf,NULL)) return 0;
	  ret = poe_pop(U,NULL);
	  break;
	}
      }
      break;
    case POE_INT_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"int");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_FLOAT_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"float");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_CHAR_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"char");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_NULL_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"null");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_TRUE_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"bool");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_FALSE_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"bool");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_UNDEF_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"undef");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_PFUNC_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"func");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_CFUNC_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"func");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_CODE_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"code");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    case POE_FILE_TAG:
      ret.poe_str = poe_make_str(U);
      if (!ret.poe_str) return 0;
      if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
      snprintf(chrbuf,32,"file");
      if (!pexec_writestr(U,chrbuf,NULL)) return 0;
      ret = poe_pop(U,NULL);
      break;
    }
    if (!poe_pushobj(U,ret,POE_STR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
    if (!poe_gc_control(U,ret,POE_STR_TAG)) return 0;
  }
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = argc;
  return 1;
}
