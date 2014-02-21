/* pdo2.c
 * Execution of data structure/iteration instructions on Poe universes
 */

#include "pdata.h"
#include "pmem.h"
#include "puniverse.h"
#include "pgc.h"
#include "pexec.h"
#include "pdo1.h"
#include "pdo2.h"

/* for arracc and tabacc, we call the metamethod if the access returns
   undef. */

int pexec_arracc(poe_universe *U)
{
  int stindex = U->main_stack_index, havemeta = 0, needmeta = 0, status;
  long i;
  poe_obj meta, acc, argo;
  tagged_poe_obj *stack = U->main_stack_base, a = stack[stindex-1],
    b = stack[stindex];
  char metatag, acctag;
  poe_arr *argv;
  meta = get_metamethod(meta_arracc, a.data, a.tag, &metatag);
  if (metatag!=POE_UNDEF_TAG) havemeta = 1;
  switch (b.tag) {
  case POE_INT_TAG:
    i = b.data.poe_int;
    break;
  case POE_FLOAT_TAG:
    i = (int)(b.data.poe_float);
    break;
  case POE_CHAR_TAG:
    i = b.data.poe_char;
    break;
  default:
    if (havemeta) needmeta = 1;
    else return POE_FATAL_ERROR;
    break;
  }
  if (!needmeta) {
    switch (a.tag) {
    case POE_ARR_TAG:
      acc = poe_arr_get(a.data.poe_arr, i, &acctag);
      break;
    case POE_STR_TAG:
      acc.poe_char = poe_string_get(a.data.poe_str, i, &acctag);
      break;
    default:
      if (havemeta) needmeta = 1;
      else return POE_FATAL_ERROR;
      break;
    }
    switch (acctag) {
    case POE_UNDEF_TAG:
      if (havemeta) needmeta = 1;
      break;
    }
  }
  if (!needmeta) {
    U->main_stack_index --;
    stack[stindex-1].tag = acctag;
    stack[stindex-1].data = acc;
  } else {
    U->main_stack_index -= 2;
    argv = poe_make_arr(U);
    if (!argv) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,0,a.tag,a.data)) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,1,b.tag,b.data)) return POE_FATAL_ERROR;
    if (!poe_pushargv(U,argv)) return POE_FATAL_ERROR;
    argo.poe_arr = argv;
    if (!poe_gc_control(U,argo,POE_ARR_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushargc(U,2)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,meta,metatag)) return POE_FATAL_ERROR;
    if (!(status = pexec_call(U))) return POE_FATAL_ERROR;
    if (status==POE_HALT) return POE_HALT;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_tabacc(poe_universe *U, char *s)
{
  int stindex = U->main_stack_index, havemeta = 0, needmeta = 0, 
    needcontrol = 0, status;
  poe_str *ps = NULL;
  poe_obj meta, acc, argo;
  tagged_poe_obj *stack = U->main_stack_base, a = stack[stindex-1],
    b = stack[stindex];
  char metatag, acctag;
  poe_arr *argv;

  meta = get_metamethod(meta_tabacc, a.data, a.tag, &metatag);
  if (metatag!=POE_UNDEF_TAG) havemeta = 1; // mark that we have meta
  if (!s) { // get the string if we don't have it already
    switch (b.tag) {
    case POE_STR_TAG:
      ps = b.data.poe_str;
      s = ps->s;
      break;
    default:
      if (havemeta) needmeta = 1;
      else return POE_FATAL_ERROR;
      break;
    }
  }
  if (!needmeta) { // if we don't need a metatable at this point, we can go
    switch (a.tag) {
    case POE_TABLE_TAG:
      acc = poe_table_rawget(a.data.poe_table, s, &acctag); // get the object
      break;
    default:
      if (havemeta) needmeta = 1;
      else return POE_FATAL_ERROR;
      break;
    }
    if (acctag==POE_UNDEF_TAG) /* if we received an undef object and we have
				  a metamethod, call it */
      if (havemeta) needmeta = 1;
  }
  if (!needmeta) {
    U->main_stack_index --;
    stack[stindex-1].tag = acctag;
    stack[stindex-1].data = acc;
  } else {
    U->main_stack_index -= 2;
    if (!ps) {
      ps = poe_make_str(U);
      if (!ps) return POE_FATAL_ERROR;
      if (!poe_str_write(U,ps,(void *)s,strlen(s))) return POE_FATAL_ERROR;
      b.tag = POE_STR_TAG;
      b.data.poe_str = ps;
      needcontrol = 1;
    }
    argv = poe_make_arr(U);
    if (!argv) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,0,a.tag,a.data)) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,1,b.tag,b.data)) return POE_FATAL_ERROR;
    if (!poe_pushargv(U,argv)) return POE_FATAL_ERROR;
    argo.poe_arr = argv;
    if (!poe_gc_control(U,argo,POE_ARR_TAG)) return POE_FATAL_ERROR;
    if (needcontrol) {
      argo.poe_str = ps;
      if (!poe_gc_control(U,argo,POE_STR_TAG)) return POE_FATAL_ERROR;
    }
    if (!poe_pushargc(U,2)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,meta,metatag)) return POE_FATAL_ERROR;
    if (!(status = pexec_call(U))) return POE_FATAL_ERROR;
    if (status==POE_HALT) return POE_HALT;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_arrset(poe_universe *U)
{
  int index = U->main_stack_index, key, ch, status;
  tagged_poe_obj *stack = U->main_stack_base;
  tagged_poe_obj a = stack[index-2], b = stack[index-1], c = stack[index];
  poe_obj meta, argo;
  char metatag;
  poe_arr *argv;
  meta = get_metamethod(meta_arrset,b.data,b.tag,&metatag);
  if (metatag==POE_UNDEF_TAG) {
    switch (c.tag) {
    case POE_INT_TAG:
      key = c.data.poe_int;
      break;
    case POE_CHAR_TAG:
      key = c.data.poe_char;
      break;
    default:
      return POE_FATAL_ERROR;
    }
    switch (b.tag) {
    case POE_ARR_TAG:
      if (!poe_arr_set(U,b.data.poe_arr,key,a.tag,a.data))
	return POE_FATAL_ERROR;
      break;
    case POE_STR_TAG:
      if (a.tag==POE_CHAR_TAG) ch = a.data.poe_char;
      else if (a.tag==POE_UNDEF_TAG) ch = -1;
      else return POE_FATAL_ERROR;
      if (!poe_str_set(U,b.data.poe_str,key,ch))
	return POE_FATAL_ERROR;
      break;
    default:
      return POE_FATAL_ERROR;
    }
    U->main_stack_index -= 3;
  } else {
    U->main_stack_index -= 3;
    argv = poe_make_arr(U);
    if (!argv) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,2,a.tag,a.data)) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,0,b.tag,b.data)) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,1,c.tag,c.data)) return POE_FATAL_ERROR;
    if (!poe_pushargv(U,argv)) return POE_FATAL_ERROR;
    argo.poe_arr = argv;
    if (!poe_gc_control(U,argo,POE_ARR_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushargc(U,3)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,meta,metatag)) return POE_FATAL_ERROR;
    if (!(status = pexec_call(U))) return POE_FATAL_ERROR;
    if (status==POE_HALT) return POE_HALT;
    U->main_stack_index --; // we need to get rid of the return value
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_tabset(poe_universe *U, char *s)
{
  int index = U->main_stack_index, status;
  tagged_poe_obj *stack = U->main_stack_base;
  tagged_poe_obj a = stack[index-2], b = stack[index-1], c = stack[index];
  poe_obj meta, argo;
  char metatag;
  poe_arr *argv;
  poe_str *ps = NULL;
  meta = get_metamethod(meta_tabset,b.data,b.tag,&metatag);
  if (metatag==POE_UNDEF_TAG) {
    if (!s) {
      switch (c.tag) {
      case POE_STR_TAG:
	ps = c.data.poe_str;
	s = ps->s;
	break;
      default:
	return POE_FATAL_ERROR;
      }
    }
    switch (b.tag) {
    case POE_TABLE_TAG:
      if (!poe_table_rawset(U,b.data.poe_table,s,a.tag,a.data))
	return POE_FATAL_ERROR;
      break;
    default:
      return POE_FATAL_ERROR;
    }
    U->main_stack_index -= 3;
  } else {
    U->main_stack_index -= 3;
    if (!ps) {
      ps = poe_make_str(U);
      if (!ps) return POE_FATAL_ERROR;
      if (!poe_str_write(U,ps,(void *)s,strlen(s))) return POE_FATAL_ERROR;
      c.tag = POE_STR_TAG;
      c.data.poe_str = ps;
    }
    argv = poe_make_arr(U);
    if (!argv) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,2,a.tag,a.data)) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,0,b.tag,b.data)) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,1,c.tag,c.data)) return POE_FATAL_ERROR;
    if (!poe_pushargv(U,argv)) return POE_FATAL_ERROR;
    argo.poe_arr = argv;
    if (!poe_gc_control(U,argo,POE_ARR_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushargc(U,3)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,meta,metatag)) return POE_FATAL_ERROR;
    if (!(status = pexec_call(U))) return POE_FATAL_ERROR;
    if (status==POE_HALT) return POE_HALT;
    U->main_stack_index --;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

/* Here, the table and the assignment on are on the stack.  We simply need to 
push the table onto the stack once more (as well as a null value) and
then call tabset, which does the dirty work. Further, the table will remain
on the stack automatically. */
int pexec_tabkeep(poe_universe *U, char *s)
{
  int index = U->main_stack_index, result;
  tagged_poe_obj *stack = U->main_stack_base, a = stack[index-1];
  if (!poe_pushobj(U,a.data,a.tag)) return POE_FATAL_ERROR;
  if (!poe_pushobj(U,a.data,POE_NULL_TAG)) return POE_FATAL_ERROR;
  result = pexec_tabset(U,s);
  pgc_countdown(U);
  return result;
}

/* Here, the array, assignment, and integer key are on the stack, in that order.
To call pexec_arrset, we need the stack's top elements to be [assignment, arr,
integer key]. To do this, we first push the array onto the stack and then
swap the top two elements. */
int pexec_arrkeep(poe_universe *U)
{
  int index = U->main_stack_index, result;
  tagged_poe_obj *stack = U->main_stack_base, a = stack[index-2];
  if (!poe_pushobj(U,a.data,a.tag)) return POE_FATAL_ERROR;
  if (!pexec_swap(U)) return POE_FATAL_ERROR;
  result = pexec_arrset(U);
  pgc_countdown(U);
  return result;
}

int pexec_pushargv(poe_universe *U)
{
  int result;
  tagged_poe_obj o = U->main_stack_base[U->main_stack_index];
  if (o.tag==POE_ARR_TAG) {
    U->main_stack_index --;
    result = poe_pushargv(U,o.data.poe_arr);
    pgc_countdown(U);
    return result;
      }
  return POE_FATAL_ERROR;
}

int pexec_pushargc(poe_universe *U)
{
  int result, i;
  tagged_poe_obj o = U->main_stack_base[U->main_stack_index];
  if (o.tag==POE_INT_TAG) {
    U->main_stack_index --;
    i = o.data.poe_int;
  } else if (o.tag==POE_CHAR_TAG) {
    U->main_stack_index --;
    i = o.data.poe_char;
  } else return POE_FATAL_ERROR;
  result = poe_pushargc(U,i);
  pgc_countdown(U);
  return result;
}

int pexec_pushretv(poe_universe *U)
{
  tagged_poe_obj o = U->main_stack_base[U->main_stack_index];
  U->main_stack_index --;
  if (o.tag==POE_ARR_TAG) {
    U->retv = o.data.poe_arr;
    pgc_countdown(U);
    return POE_CONTINUE;
  } else return POE_FATAL_ERROR;
}

int pexec_pushretc(poe_universe *U)
{
  tagged_poe_obj o = U->main_stack_base[U->main_stack_index];
  U->main_stack_index --;
  if (o.tag==POE_INT_TAG) {
    U->retc = o.data.poe_int;
  } else if (o.tag==POE_CHAR_TAG) {
    U->retc = o.data.poe_char;
  } else return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_argv(poe_universe *U)
{
  poe_arr *a = U->argv_stack_base[U->argv_stack_index];
  poe_obj o;
  o.poe_arr = a;
  if (!poe_pushobj(U,o,POE_ARR_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_argc(poe_universe *U)
{
  long i = U->argc_stack_base[U->argc_stack_index];
  poe_obj o;
  o.poe_int = i;
  if (!poe_pushobj(U,o,POE_INT_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_retv(poe_universe *U)
{
  poe_obj o;
  o.poe_arr = U->retv;
  if (!poe_pushobj(U,o,POE_ARR_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_retc(poe_universe *U)
{
  poe_obj o;
  o.poe_int = U->retc;
  if (!poe_pushobj(U,o,POE_INT_TAG)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int poe_extern_ass(poe_universe *U, poe_table *tab, char *s, poe_obj o,
		   char otag)
{
  poe_obj res;
  char restag;
  if (!tab) return POE_FATAL_ERROR;
  res.poe_table = tab;
  if (!poe_pushobj(U,res,POE_TABLE_TAG)) return POE_FATAL_ERROR;
  if (!poe_pushobj(U,res,POE_NULL_TAG)) return POE_FATAL_ERROR;
  if (!pexec_tabacc(U,s)) return POE_FATAL_ERROR;
  res = poe_pop(U,&restag);
  if (restag!=POE_UNDEF_TAG) {
    res.poe_table = tab;
    if (!poe_pushobj(U,o,otag)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,res,POE_TABLE_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,res,POE_NULL_TAG)) return POE_FATAL_ERROR;
    if (!pexec_tabset(U,s)) return POE_FATAL_ERROR;
    return POE_CONTINUE;
  }
  if (tab->super_tag!=POE_TABLE_TAG) return POE_FATAL_ERROR;
  return poe_extern_ass(U,tab->super.poe_table,s,o,otag);
}

poe_obj poe_symbol_get(poe_universe *U, poe_table *tab, char *s, char *tag)
{
  poe_obj res;
  char restag;
  if (!tab) {
    *tag = POE_UNDEF_TAG;
    return (poe_obj)0L;
  }
  res.poe_table = tab;
  if (!poe_pushobj(U,res,POE_TABLE_TAG)) {
    *tag = POE_UNDEF_TAG; 
    return (poe_obj)0L; 
  }
  if (!poe_pushobj(U,res,POE_NULL_TAG)) {
    *tag = POE_UNDEF_TAG;
    return (poe_obj)0L;
  }
  if (!pexec_tabacc(U,s)) {
    *tag = POE_UNDEF_TAG;
    return (poe_obj)0L;
  }
  res = poe_pop(U,&restag);
  if (restag!=POE_UNDEF_TAG) { 
    *tag = restag;
    return res;
  }
  if (tab->super_tag!=POE_TABLE_TAG) {
    *tag = POE_UNDEF_TAG;
    return (poe_obj)0L;
  }
  return poe_symbol_get(U,tab->super.poe_table,s,tag);
}

/* ASSIGNMENT MODUS: a "normal" assignment is always a local assignment.
An external assignment recursively searches through the local tree until the
symbol is defined in some context, and then redefines the symbol in that
context. A global assignment always assigns the symbol at the top of the
local tree. */
int pexec_ass(poe_universe *U, char type, char *symbol)
{
  poe_table *tab = U->locals_stack_base[U->locals_stack_index];
  poe_obj tab_obj, ass_obj;
  char ass_type;
  if (type==NORMAL) type = LOCAL;
  if (type==LOCAL) {
    tab_obj.poe_table = tab;
    if (!poe_pushobj(U,tab_obj,POE_TABLE_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,tab_obj,POE_NULL_TAG)) return POE_FATAL_ERROR;
    if (!pexec_tabset(U,symbol)) return POE_FATAL_ERROR;
  } else if (type==EXTERN) {
    ass_obj = poe_pop(U,&ass_type);
    if (tab->super_tag!=POE_TABLE_TAG) return POE_FATAL_ERROR;
    if (!poe_extern_ass(U,tab->super.poe_table,symbol,ass_obj,ass_type))
      return POE_FATAL_ERROR;
  } else if (type==GLOBAL) {
    for (; tab->super_tag==POE_TABLE_TAG; tab = tab->super.poe_table);
    tab_obj.poe_table = tab;
    if (!poe_pushobj(U,tab_obj,POE_TABLE_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,tab_obj,POE_NULL_TAG)) return POE_FATAL_ERROR;
    if (!pexec_tabset(U,symbol)) return POE_FATAL_ERROR;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_get(poe_universe *U, char type, char *symbol)
{
  char o_tag;
  poe_obj o;
  poe_table *tab = U->locals_stack_base[U->locals_stack_index];
  if (type==NORMAL) {
    o = poe_symbol_get(U,tab,symbol,&o_tag);
    if (!poe_pushobj(U,o,o_tag)) return POE_FATAL_ERROR;
  } else if (type==EXTERN) {
    if (tab->super_tag==POE_TABLE_TAG) { 
      tab = tab->super.poe_table;
      o = poe_symbol_get(U,tab,symbol,&o_tag);
      if (!poe_pushobj(U,o,o_tag)) return POE_FATAL_ERROR;
    } else if (tab->super_tag==POE_UNDEF_TAG) {
      if (!poe_pushobj(U,o,POE_UNDEF_TAG)) return POE_FATAL_ERROR;
    } else return POE_FATAL_ERROR;
  } else if (type==LOCAL) {
    o.poe_table = tab;
    if (!poe_pushobj(U,o,POE_TABLE_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,o,POE_NULL_TAG)) return POE_FATAL_ERROR;
    if (!pexec_tabacc(U,symbol)) return POE_FATAL_ERROR;
  } else if (type==GLOBAL) {
    for (; tab->super_tag==POE_TABLE_TAG; tab = tab->super.poe_table);
    o.poe_table = tab;
    if (!poe_pushobj(U,o,POE_TABLE_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,o,POE_NULL_TAG)) return POE_FATAL_ERROR;
    if (!pexec_tabacc(U,symbol)) return POE_FATAL_ERROR;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_getmeta(poe_universe *U)
{
  int index = U->main_stack_index, status;
  tagged_poe_obj *stack = U->main_stack_base, a = stack[index];
  poe_obj meta, argo;
  char metatag;
  poe_arr *argv;
  meta = get_metamethod(meta_metaacc, a.data, a.tag, &metatag);
  if (metatag==POE_UNDEF_TAG) {
    switch (a.tag) {
    case POE_TABLE_TAG:
      stack[index].tag = POE_TABLE_TAG;
      stack[index].data.poe_table = a.data.poe_table->meta;
      break;
    case POE_ARR_TAG:
      stack[index].tag = POE_TABLE_TAG;
      stack[index].data.poe_table = a.data.poe_arr->meta;
      break;
    case POE_STR_TAG:
      stack[index].tag = POE_TABLE_TAG;
      stack[index].data.poe_table = a.data.poe_str->meta;
      break;
    default:
      return POE_FATAL_ERROR;
    }
  } else {
    U->main_stack_index --;
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
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_setmeta(poe_universe *U)
{
  int index = U->main_stack_index, status;
  tagged_poe_obj *stack = U->main_stack_base, a = stack[index-1], 
    b = stack[index];
  poe_obj meta, argo;
  char metatag;
  poe_arr *argv;
  poe_table *tab;
  meta = get_metamethod(meta_metaset, b.data, b.tag, &metatag);
  if (metatag==POE_UNDEF_TAG) {
    switch (a.tag) {
    case POE_TABLE_TAG:
      tab = a.data.poe_table;
      break;
    default:
      return POE_FATAL_ERROR;
    }
    switch (b.tag) {
    case POE_TABLE_TAG:
      b.data.poe_table->meta = tab;
      break;
    case POE_ARR_TAG:
      b.data.poe_arr->meta = tab;
      break;
    case POE_STR_TAG:
      b.data.poe_str->meta = tab;
      break;
    default:
      return POE_FATAL_ERROR;
    }
    U->main_stack_index -= 2;
  } else {
    U->main_stack_index -= 2;
    argv = poe_make_arr(U);
    if (!argv) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,0,b.tag,b.data)) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,1,a.tag,a.data)) return POE_FATAL_ERROR;
    if (!poe_pushargv(U,argv)) return POE_FATAL_ERROR;
    argo.poe_arr = argv;
    if (!poe_gc_control(U,argo,POE_ARR_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushargc(U,2)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,meta,metatag)) return POE_FATAL_ERROR;
    if (!(status = pexec_call(U))) return POE_FATAL_ERROR;
    if (status==POE_HALT) return POE_HALT;
    U->main_stack_index --;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_getsup(poe_universe *U)
{
  int i = U->main_stack_index, status;
  tagged_poe_obj *stack = U->main_stack_base, a = stack[i];
  poe_obj meta, argo;
  char metatag;
  poe_arr *argv;
  meta = get_metamethod(meta_superacc, a.data, a.tag, &metatag);
  if (metatag==POE_UNDEF_TAG) {
    switch (a.tag) {
    case POE_TABLE_TAG:
      stack[i].tag = a.data.poe_table->super_tag;
      stack[i].data = a.data.poe_table->super;
      break;
    case POE_ARR_TAG:
      stack[i].tag = a.data.poe_arr->super_tag;
      stack[i].data = a.data.poe_arr->super;
      break;
    case POE_STR_TAG:
      stack[i].tag = a.data.poe_str->super_tag;
      stack[i].data = a.data.poe_str->super;
      break;
    case POE_PFUNC_TAG:
      if (!a.data.poe_pfunc->parent) {
	stack[i].tag = POE_UNDEF_TAG;
      } else {
	stack[i].tag = POE_TABLE_TAG;
	stack[i].data.poe_table = a.data.poe_pfunc->parent;
      }
      break;
    default:
      return POE_FATAL_ERROR;
    }
  } else {
    U->main_stack_index --;
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
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_setsup(poe_universe *U)
{
  int index = U->main_stack_index, status;
  tagged_poe_obj *stack = U->main_stack_base, a = stack[index-1], 
    b = stack[index];
  poe_obj meta, argo;
  char metatag;
  poe_arr *argv;
  meta = get_metamethod(meta_superacc, b.data, b.tag, &metatag);
  if (metatag==POE_UNDEF_TAG) {
    switch (b.tag) {
    case POE_TABLE_TAG:
      b.data.poe_table->super_tag = a.tag;
      b.data.poe_table->super = a.data;
      break;
    case POE_ARR_TAG:
      b.data.poe_arr->super_tag = a.tag;
      b.data.poe_arr->super = a.data;
      break;
    case POE_STR_TAG:
      b.data.poe_str->super_tag = a.tag;
      b.data.poe_str->super = a.data;
      break;
    case POE_PFUNC_TAG:
      if (a.tag==POE_UNDEF_TAG) {
	b.data.poe_pfunc->parent = NULL;
      } else if (a.tag==POE_TABLE_TAG) {
	b.data.poe_pfunc->parent = a.data.poe_table;
      } else return POE_FATAL_ERROR;
      break;
    default:
      return POE_FATAL_ERROR;
    }
  } else {
    U->main_stack_index -= 2;
    argv = poe_make_arr(U);
    if (!argv) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,0,b.tag,b.data)) return POE_FATAL_ERROR;
    if (!poe_arr_set(U,argv,1,a.tag,a.data)) return POE_FATAL_ERROR;
    if (!poe_pushargv(U,argv)) return POE_FATAL_ERROR;
    argo.poe_arr = argv;
    if (!poe_gc_control(U,argo,POE_ARR_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushargc(U,2)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,meta,metatag)) return POE_FATAL_ERROR;
    if (!(status = pexec_call(U))) return POE_FATAL_ERROR;
    if (status==POE_HALT) return POE_HALT;
    U->main_stack_index --;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_forprep(poe_universe *U, char *code, int *offset)
{
  int index = U->main_stack_index, status;
  tagged_poe_obj *main_stack = U->main_stack_base, a = main_stack[index-2],
    b = main_stack[index-1], c = main_stack[index];
  poe_obj meta;
  for_obj *for_stack = U->for_stack_base;
  char metatag;
  poe_arr *argv;
  U->main_stack_index -= 3;
  if (U->for_stack_index+1>=U->for_stack_size) {
    if (!enlarge_stack(U,POE_FOR_STACK,1))
      return POE_FATAL_ERROR;
    for_stack = U->for_stack_base;
  }
  if (U->loop_stack_index+1>=U->loop_stack_size)
    if (!enlarge_stack(U,POE_LOOP_STACK,1))
      return POE_FATAL_ERROR;
  meta = get_metamethod(meta_onfor, a.data, a.tag, &metatag);
  if (metatag!=POE_UNDEF_TAG) {
    argv = poe_make_arr(U);
    if (!argv) return 0;
    if (!poe_arr_set(U,argv,0,a.tag,a.data)) return POE_FATAL_ERROR;
    if (!poe_pushargv(U,argv)) return POE_FATAL_ERROR;
    if (!poe_gc_control(U,(poe_obj)argv,POE_ARR_TAG)) return POE_FATAL_ERROR;
    if (!poe_pushargc(U,1)) return POE_FATAL_ERROR;
    if (!poe_pushobj(U,meta,metatag)) return POE_FATAL_ERROR;
    if (!(status = pexec_call(U))) return POE_FATAL_ERROR;
    if (status==POE_HALT) return POE_FATAL_ERROR;
    a.data = poe_pop(U,&metatag);
    a.tag = metatag;
  }
  index = ++U->for_stack_index;
  U->loop_stack_index++;
  U->info_stack_base[U->info_stack_index] ++;
  if ((b.tag!=POE_STR_TAG)||(c.tag!=POE_STR_TAG)) return POE_FATAL_ERROR;
  if (!((a.tag==POE_TABLE_TAG)||(a.tag==POE_STR_TAG)||(a.tag==POE_ARR_TAG)))
    return POE_FATAL_ERROR;
  for_stack[index].tag = a.tag;
  for_stack[index].iterable = a.data;
  for_stack[index].key_sname = b.data.poe_str;
  for_stack[index].val_sname = c.data.poe_str;
  for_stack[index].index = -1;
  for_stack[index].node = NULL;
  for_stack[index].offset = *offset+1;
  for_stack[index].break_offset = *offset + 2 + size_code(code,FORITER);
  U->loop_stack_base[U->loop_stack_index] = FOR_LOOP;
  if (!pexec_foriter(U,offset)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int poe_table_dokeyval(poe_universe *U, for_obj *f, poe_obj_list *l)
{
  poe_str *ps;
  poe_obj o;
  /* ASSIGN KEY SYMBOL */
  ps = poe_make_str(U);
  if (!poe_str_write(U,ps,l->s,strlen(l->s))) return POE_FATAL_ERROR;
  o.poe_str = ps;
  if (!poe_pushobj(U,o,POE_STR_TAG)) return POE_FATAL_ERROR;
  if (!poe_gc_control(U,o,POE_STR_TAG)) return POE_FATAL_ERROR;
  o.poe_table = U->locals_stack_base[U->locals_stack_index];
  if (!poe_pushobj(U,o,POE_TABLE_TAG)) return POE_FATAL_ERROR;
  o.poe_str = f->key_sname;
  if (!poe_pushobj(U,o,POE_STR_TAG)) return POE_FATAL_ERROR;
  if (!pexec_tabset(U,NULL)) return POE_FATAL_ERROR;
  /* ASSIGN VALUE SYMBOL */
  if (!poe_pushobj(U,l->first,l->tag)) return POE_FATAL_ERROR;
  o.poe_table = U->locals_stack_base[U->locals_stack_index];
  if (!poe_pushobj(U,o,POE_TABLE_TAG)) return POE_FATAL_ERROR;
  o.poe_str = f->val_sname;
  if (!poe_pushobj(U,o,POE_STR_TAG)) return POE_FATAL_ERROR;
  if (!pexec_tabset(U,NULL)) return POE_FATAL_ERROR;
  return POE_CONTINUE;
}

int poe_strarr_dokeyval(poe_universe *U, for_obj *f, int key, poe_obj ass, 
			char tag)
{
  poe_obj o;
  /* INT KEY */
  o.poe_int = key;
  if (!poe_pushobj(U,o,POE_INT_TAG)) return POE_FATAL_ERROR;
  o.poe_table = U->locals_stack_base[U->locals_stack_index];
  if (!poe_pushobj(U,o,POE_TABLE_TAG)) return POE_FATAL_ERROR;
  o.poe_str = f->key_sname;
  if (!poe_pushobj(U,o,POE_STR_TAG)) return POE_FATAL_ERROR;
  if (!pexec_tabset(U,NULL)) return POE_FATAL_ERROR;
  /* VALUE */
  if (!poe_pushobj(U,ass,tag)) return POE_FATAL_ERROR;
  o.poe_table = U->locals_stack_base[U->locals_stack_index];
  if (!poe_pushobj(U,o,POE_TABLE_TAG)) return POE_FATAL_ERROR;
  o.poe_str = f->val_sname;
  if (!poe_pushobj(U,o,POE_STR_TAG)) return POE_FATAL_ERROR;
  if (!pexec_tabset(U,NULL)) return POE_FATAL_ERROR;
  return POE_CONTINUE;
}

int poe_table_next(poe_universe *U, for_obj *f, int *offset)
{
  int i;
  poe_obj_list *l = f->node;
  poe_table *t = f->iterable.poe_table;
  if ((l==NULL)||(l->next==NULL)) { // get next node
    for (i=++(f->index); ((l = (t->t)[i])==NULL)&&(i<t->size); i++);
    if (i>=t->size) { // done iterating
      *offset = f->break_offset;
      U->for_stack_index --;
      U->loop_stack_index --;
      U->info_stack_base[U->info_stack_index] --;
    } else { // found a new item at the head of a list
      *offset = f->offset;
      f->index = i;
      f->node = l;
      poe_table_dokeyval(U,f,l);
    }
  } else {
    *offset = f->offset;
    l = f->node = l->next;
    poe_table_dokeyval(U,f,l);
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int poe_arr_next(poe_universe *U, for_obj *f, int *offset)
{
  poe_arr *a = f->iterable.poe_arr;
  int i, max = a->max;
  for (i=++f->index; ((a->a)[i].tag==POE_UNDEF_TAG)&&(i<=max); i++);
  if (i>max) { // done iterating
    *offset = f->break_offset;
    U->for_stack_index --;
    U->loop_stack_index --;
    U->info_stack_base[U->info_stack_index] --;
  } else { // we've got a new item
    *offset = f->offset;
    f->index = i;
    poe_strarr_dokeyval(U,f,i,(a->a)[i].data,(a->a)[i].tag);
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int poe_str_next(poe_universe *U, for_obj *f, int *offset)
{
  poe_str *s = f->iterable.poe_str;
  int index = ++f->index;
  if (index>=(s->len)) { // done iterating
    *offset = f->break_offset;
    U->for_stack_index --;
    U->loop_stack_index --;
    U->info_stack_base[U->info_stack_index] --;
  } else { // new char
    *offset = f->offset;
    poe_strarr_dokeyval(U,f,index,(poe_obj)(s->s)[index],POE_CHAR_TAG);
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_foriter(poe_universe *U, int *offset)
{
  switch (U->for_stack_base[U->for_stack_index].tag) {
  case POE_TABLE_TAG:
    if (!poe_table_next(U,U->for_stack_base+U->for_stack_index,offset))
      return POE_FATAL_ERROR;
    break;
  case POE_ARR_TAG:
    if (!poe_arr_next(U,U->for_stack_base+U->for_stack_index,offset))
      return POE_FATAL_ERROR;
    break;
  case POE_STR_TAG:
    if (!poe_str_next(U,U->for_stack_base+U->for_stack_index,offset))
      return POE_FATAL_ERROR;
    break;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

/* JUST HAVE TO DO THESE THREE */
int pexec_whileprep(poe_universe *U, char *code, int *offset)
{
  int index, size;
  while_obj *while_stack;
  if (U->while_stack_index+1>=U->while_stack_size)
    if (!enlarge_stack(U,POE_WHILE_STACK,1))
      return POE_FATAL_ERROR;
  if (U->loop_stack_index+1>=U->loop_stack_size)
    if (!enlarge_stack(U,POE_LOOP_STACK,1))
      return POE_FATAL_ERROR;
  while_stack = U->while_stack_base;
  index = ++U->while_stack_index;
  U->loop_stack_index++;
  U->info_stack_base[U->info_stack_index] ++;
  while_stack[index].expr = code;
  while_stack[index].offset = *offset+(size = size_code(code,CODEEND)+2);
  size--;
  while_stack[index].break_offset = while_stack[index].offset + 
    size_code(code+size,WHILEITER) + 1;
  U->loop_stack_base[U->loop_stack_index] = WHILE_LOOP;
  if (!pexec_whileiter(U,offset)) return POE_FATAL_ERROR;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_whileiter(poe_universe *U, int *offset)
{
  char atag;
  if (poe_exec_code(U,U->while_stack_base[U->while_stack_index].expr,NULL)
      !=POE_CODEEND) return POE_FATAL_ERROR;
  poe_pop(U,&atag);
  if (poe_truth_val(atag))  // case: we do another iteration
    *offset = U->while_stack_base[U->while_stack_index].offset;
  else {
    *offset = U->while_stack_base[U->while_stack_index].break_offset;
    U->while_stack_index --;
    U->loop_stack_index --;
    U->info_stack_base[U->info_stack_index] --;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_dowprep(poe_universe *U, char *code, int *offset)
{
  int index, size;
  while_obj *while_stack;
  if (U->while_stack_index+1>=U->while_stack_size)
    if (!enlarge_stack(U,POE_WHILE_STACK,1))
      return POE_FATAL_ERROR;
  if (U->loop_stack_index+1>=U->loop_stack_size)
    if (!enlarge_stack(U,POE_LOOP_STACK,1))
      return POE_FATAL_ERROR;
  while_stack = U->while_stack_base;
  index = ++U->while_stack_index;
  U->loop_stack_index++;
  U->info_stack_base[U->info_stack_index] ++;
  while_stack[index].expr = code;
  while_stack[index].offset = *offset+(size = size_code(code,CODEEND)+2);
  size--;
  while_stack[index].break_offset = while_stack[index].offset + 
    size_code(code+size,WHILEITER) + 1;
  U->loop_stack_base[U->loop_stack_index] = WHILE_LOOP;
  *offset = while_stack[index].offset;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_if(poe_universe *U, char *code, int *offset)
{
  tagged_poe_obj a = U->main_stack_base[U->main_stack_index--];
  if (poe_truth_val(a.tag)) {
    *offset += 1;
  } else {
    *offset += size_code(code,ELSE) + 2;
  }
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_break(poe_universe *U, int *offset)
{
  int loop = U->loop_stack_base[U->loop_stack_index--];
  if (loop==FOR_LOOP) {  // case: for loop 
    if (offset)
      *offset = U->for_stack_base[U->for_stack_index--].break_offset;
  }
  else // case: while loop
    if (offset)
      *offset = U->while_stack_base[U->while_stack_index--].break_offset;
  U->info_stack_base[U->info_stack_index] --;
  pgc_countdown(U);
  return POE_CONTINUE;
}

int pexec_cont(poe_universe *U, int *offset)
{
  int loop = U->loop_stack_base[U->loop_stack_index], ret;
  if (loop==FOR_LOOP)
    ret = pexec_foriter(U,offset);
  else ret = pexec_whileiter(U,offset);
  pgc_countdown(U);
  return ret;
}
