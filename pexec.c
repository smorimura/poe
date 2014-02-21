/* pexec.c
 * Functions for executing strings of instructions (i.e. "code") on
 * Poe universes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "pdata.h"
#include "pmem.h"
#include "puniverse.h"
#include "pexec.h"
#include "pgc.h"
#include "pdo1.h"
#include "pdo2.h"

char *POE_METAKEYS[] = {
  "add", "sub", "mult", "div", "mod", "unm", "strlen", "arrmax", "eq", "lt",
  "le", "band", "bor", "bxor", "bnot", "tabacc", "arracc", "tabset", "arrset",
  "call", "do", "tostr", "type", "onfor", "metaacc", "metaset",
  "superacc", "superset"
};

poe_obj poe_pop(poe_universe *U, char *tag)
{
  tagged_poe_obj a = U->main_stack_base[U->main_stack_index];
  U->main_stack_index --;
  if (tag) *tag = a.tag;
  return a.data;
}

int poe_truth_val(char tag)
{
  switch (tag) {
  case POE_UNDEF_TAG:
  case POE_FALSE_TAG:
  case POE_NULL_TAG:
    return 0;
  default:
    return 1;
  }
}

poe_obj get_metamethod(int meta, poe_obj o, char tag, char *rettag)
{
  switch (tag) {
  case POE_TABLE_TAG:
    if (o.poe_table->meta) 
      return poe_table_rawget(o.poe_table->meta,POE_METAKEYS[meta],rettag);
    *rettag = POE_UNDEF_TAG;
    return (poe_obj)0L;
    break;
  case POE_STR_TAG:
    if (o.poe_str->meta)
      return poe_table_rawget(o.poe_str->meta,POE_METAKEYS[meta],
			      rettag);
    *rettag = POE_UNDEF_TAG;
    return (poe_obj)0L;
    break;
  case POE_ARR_TAG:
    if (o.poe_arr->meta)
      return poe_table_rawget(o.poe_arr->meta,POE_METAKEYS[meta],rettag);
    *rettag = POE_UNDEF_TAG;
    return (poe_obj)0L;
    break;
  default:
    *rettag = POE_UNDEF_TAG;
    return (poe_obj)0L;
  }
}

poe_obj poe_binhandler(int meta, poe_obj o1, char t1, poe_obj o2, char t2, 
		       char *rettag)
{
  poe_obj method1, method2;
  char methodtag1, methodtag2;
  method1 = get_metamethod(meta,o1,t1,&methodtag1);
  if (methodtag1!=POE_UNDEF_TAG) {
    *rettag = methodtag1;
    return method1;
  }
  method2 = get_metamethod(meta,o2,t2,&methodtag2);
  *rettag = methodtag2;
  return method2;
}

poe_obj poe_cmphandler(int meta, poe_obj o1, char t1, poe_obj o2, char t2,
		       char *rettag)
{
  poe_obj method1, method2;
  char methodtag1, methodtag2;
  method1 = get_metamethod(meta,o1,t1,&methodtag1);
  method2 = get_metamethod(meta,o2,t2,&methodtag2);
  if ((methodtag1==methodtag2)&&(method1.poe_float==method2.poe_float)) {
    *rettag = methodtag1;
    return method1;
  } else {
    *rettag = POE_UNDEF_TAG;
    return (poe_obj)0L;
  }
}

int poe_binmeta(poe_universe *U, int meta, poe_obj o1, char t1, poe_obj o2, 
		char t2)
{
  poe_obj metamethod;
  char methodtag;
  poe_arr *argv;
  metamethod = poe_binhandler(meta,o1,t1,o2,t2,&methodtag);
  if (methodtag!=POE_UNDEF_TAG) {
    if (!poe_pushobj(U,metamethod,methodtag)) return 0;
    argv = poe_make_arr(U);
    if (!poe_arr_set(U,argv,0,t1,o1)) return 0;
    if (!poe_arr_set(U,argv,1,t2,o2)) return 0;
    if (!poe_pushargv(U,argv)) return 0;
    if (!poe_gc_control(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushargc(U,2)) return 0;
    if (!pexec_call(U)) return 0;
  } else return 0;
  return 1;
}

int poe_cmpmeta(poe_universe *U, int meta, poe_obj o1, char t1, poe_obj o2,
		char t2)
{
  poe_obj metamethod;
  char methodtag;
  poe_arr *argv;
  metamethod = poe_cmphandler(meta,o1,t1,o2,t2,&methodtag);
  if (methodtag!=POE_UNDEF_TAG) {
    if (!poe_pushobj(U,metamethod,methodtag)) return 0;
    argv = poe_make_arr(U);
    if (!argv) return 0;
    if (!poe_arr_set(U,argv,0,t1,o1)) return 0;
    if (!poe_arr_set(U,argv,1,t2,o2)) return 0;
    if (!poe_pushargv(U,argv)) return 0;
    if (!poe_gc_control(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushargc(U,2)) return 0;
    if (!pexec_call(U)) return 0; 
  } else return 0;
  return 1;
}

int poe_unmeta(poe_universe *U, int meta, poe_obj o, char t)
{
  poe_obj metamethod;
  char methodtag;
  poe_arr *argv;
  metamethod = get_metamethod(meta,o,t,&methodtag);
  if (methodtag!=POE_UNDEF_TAG) {
    if (!poe_pushobj(U,metamethod,methodtag)) return 0;
    argv = poe_make_arr(U);
    if (!poe_arr_set(U,argv,0,t,o)) return 0;
    if (!poe_pushargv(U,argv)) return 0;
    if (!poe_gc_control(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushargc(U,1)) return 0;
    if (!pexec_call(U)) return 0;
  } else return 0;
  return 1;
}

int poe_pushargv(poe_universe *U, poe_arr *a)
{
  if (U->argv_stack_index+1>=U->argv_stack_size)
    if (!enlarge_stack(U, POE_ARGV_STACK, 1))
      return 0;
  U->argv_stack_index ++;
  U->argv_stack_base[U->argv_stack_index] = a;
  return 1;
}

int poe_pushargc(poe_universe *U, int i)
{
  if (U->argc_stack_index+1>=U->argc_stack_size)
    if (!enlarge_stack(U, POE_ARGC_STACK, 1))
      return 0;
  U->argc_stack_index ++;
  U->argc_stack_base[U->argc_stack_index] = i;
  return 1;
}

int poe_pushobj(poe_universe *U, poe_obj o, char tag)
{
  if (U->main_stack_index+1>=U->main_stack_size)
    if (!enlarge_stack(U, POE_MAIN_STACK, 1))
      return 0;
  U->main_stack_index ++;
  U->main_stack_base[U->main_stack_index].data = o;
  U->main_stack_base[U->main_stack_index].tag = tag;
  return 1;
}

/* Given a pointer to a string of instructions, will robustly calculate the
   number of characters between that and the given marker,
   counting the first byte but not the marker.  */
int size_code(char *code, char marker)
{
  int i;
  char c;
  for (i=0; 1; i++) {
    c = getinstr(code[i]);
    if (c==marker) return i;
    switch (c) {
    case PUSHINT:
      i += 1+code[i+1];
      break;
    case PUSHFLOAT:
      i += 5;
      break;
    case PUSHCHAR:
      i ++;
      break;
    case WRITESTR:
    case TABKEEP:
      i ++;
      while (code[i]) i++;
      break;
    case PUSHFUNC:
    case PUSHCODE:
      i += size_code(code+i+1,CODEEND) + 1; // +1 is to allot for the CODEEND
      break;
    case FORPREP:
      i += size_code(code+i+1,FORITER) + 1; // +1 is to allot for the FORITER
      break;
    case ARGENUM:
      i ++;
      while (1) {
	if (code[i]) {
	  while (code[i]) i++;
	  i ++;
	} else break;
      }
      break;
    case ASS:
    case GET:
      i += 2;
      while (code[i]) i++;
      break;
    case WHILEPREP:
    case DOWPREP:
      i += size_code(code+i+1,CODEEND) + 1;
      i += size_code(code+i+1,WHILEITER) + 1;
      break;
    case IF:
      i += size_code(code+i+1,ELSE) + 1;
      i += size_code(code+i+1,IFEND) + 1;
      break;
    default:
      break;
    }
  }
}

/* A Poe int is a single byte representing the size s of the integer in bytes,
followed by s bytes of integer encoding, MSB first. Two's complement is NOT
a factor as the negation is given by a separate value. So we can represent
the value 256 by
2 0x01 0x00
Moreover, we can most easily represent 0 by
0
with no bytes of encoding. However, it is also valid to represent it by
1 0x00
or
2 0x00 0x00
etc. */
long parse_int(char size, unsigned char *code, char neg)
{
  long ret = 0L;
  int i;
  for (i=0; i<size; i++)
    ret += code[i]<<(8*(size-i-1));
  if (neg) ret = -ret;
  return ret;
}

/* A Poe float, encoded in bytecode, is 4 bytes of mantissa followed by a
single byte representing the signed exponent of 10 that will be multiplied
by that mantissa. For example, if we want to encode 0.3, the easiest way
will be
00 00 00 03 -1 => 3 * 10^-1 => 0.3
Though there is always more than one way to encode a Poe float, for example
00 00 00 1E -2 => 30 * 10^-2 => 0.3
Float literals require a bit more decoding power than integers. Further, because
this conversion scheme is simple, Poe float decoding will probably not be
very exact (though this remains to be seen from testing).  */
double parse_float(unsigned char *code, char neg)
{
  double ret;
  unsigned long x = 0, i;
  signed char c, z;
  for (i=0; i<4; i++)
    x += code[i]<<(8*(3-i));
  ret = (double)x;
  c = code[4];
  if (c<=0) 
    for (z=0; z>c; z--)
      ret /= 10;
  else 
    for (z=0; z<c; z++)
      ret *= 10;
  if (neg) ret = -ret;
  return ret;
}

/* poe_exec_code: executes bytecode printed on a string in memory, returning
when an error is encountered or when a RETURN or CODEEND statement is reached.
*/
int poe_exec_code(poe_universe *U, char *code, int *offset)
{
  /* TODO: need to add garbage collection into the mix here */
  int i, r;
  for (i=0; 
       (r = poe_exec_instr(U, code[i], code+i+1, &i)) == POE_CONTINUE;);
  if (offset) *offset = i+1;
  return r;
}

/* Returns 1 on successful execution of the instruction, 0 if there was a fatal
error in execution (due to type-incorrectness, memory allocation, etc.), and
2 if the instruction successfully executed but a RETURN instruction was hit,
which signifies that this line of code should no longer be executed, and
that the calling code (if there is any) should continue to be executed.
Further, a return value of 3 means the instruction was successfully executed,
but it was a CODEEND instruction, which means that this line of code should
no longer be executed and that the calling code should continue to be
executed 
int *offset is a pointer to an integer, where the "offset" of this instruction
will be stored; the offset is the relative address of the next instruction in
the line of code that should be executed. For example, if the instruction is
PUSHFLOAT, *offset will be set to 6, as there are 5 bytes of float encoding
after the instruction, so the interpreter will find the next instruction
6 bytes after this one.  If, however, the instruction is a PUSHTRUE, no post-
instruction option bytes are needed, so *offset will be set to 1, signifying
that the next instruction in line is just the next byte. If an instruction which
calls for the halting of the code was executed (RETURN or CODEEND), then 
*offset will be set to 0, signifying that no more code in this block should
be executed.
char *code is a pointer to the character *following the instruction*.  */
int poe_exec_instr(poe_universe *U, char i, char *code, int *offset)
{
  char opt = getoption(i), instr = getinstr(i);
  poe_table *t;
  switch (instr) {
    /* value/struct creation */
  case PUSHINT:
    *offset += 2+*code; // default 1 + length 1 + length of representation
    return pexec_pushint(U,parse_int(*code,code+1,opt));
    break;
  case PUSHFLOAT:
    *offset += 6;
    return pexec_pushfloat(U,parse_float(code,opt));
    break;
  case PUSHCHAR:
    *offset += 2;
    return pexec_pushchar(U,*code);
    break;
  case PUSHTRUE:
    *offset += 1;
    return pexec_pushtrue(U);
    break;
  case PUSHFALSE:
    *offset += 1;
    return pexec_pushfalse(U);
    break;
  case PUSHUNDEF:
    *offset += 1;
    return pexec_pushundef(U);
    break;
  case PUSHNULL:
    *offset += 1;
    return pexec_pushnull(U);
    break;
  case NEWTAB:
    *offset += 1;
    return pexec_newtab(U);
    break;
  case NEWARR:
    *offset += 1;
    return pexec_newarr(U);
    break;
  case NEWSTR:
    *offset += 1;
    return pexec_newstr(U);
    break;
  case WRITESTR:
    return pexec_writestr(U, code, offset);
    break;
  case PUSHFUNC:
    return pexec_pushfunc(U, code, offset);
    break;
  case PUSHCODE:
    return pexec_pushcode(U, code, offset);
    break;
  case CODEEND:
    return POE_CODEEND;
    break;
  case EXIT:
    return POE_HALT;
    break;
  case ARGENUM:
    return pexec_argenum(U, code, offset);
    break;
    /* operators */
  case ADD:
    *offset += 1;
    return pexec_add(U);
    break;
  case SUB:
    *offset += 1;
    return pexec_sub(U);
    break;
  case MULT:
    *offset += 1;
    return pexec_mult(U);
    break;
  case DIV:
    *offset += 1;
    return pexec_div(U);
    break;
  case MOD:
    *offset += 1;
    return pexec_mod(U);
    break;
  case UNM:
    *offset += 1;
    return pexec_unm(U);
    break;
  case AND:
    *offset += 1;
    return pexec_and(U);
    break;
  case OR:
    *offset += 1;
    return pexec_or(U);
    break;
  case NOT:
    *offset += 1;
    return pexec_not(U);
    break;
  case BAND:
    *offset += 1;
    return pexec_band(U);
    break;
  case BOR:
    *offset += 1;
    return pexec_bor(U);
    break;
  case BXOR:
    *offset += 1;
    return pexec_bxor(U);
    break;
  case BNOT:
    *offset += 1;
    return pexec_bnot(U);
    break;
  case SHR:
    *offset += 1;
    return pexec_shr(U);
    break;
  case SHL:
    *offset += 1;
    return pexec_shl(U);
    break;
  case STRLEN:
    *offset += 1;
    return pexec_strlen(U);
    break;
  case ARRMAX:
    *offset += 1;
    return pexec_arrmax(U);
    break;
  case EQ:
    *offset += 1;
    return pexec_eq(U);
    break;
  case LT:
    *offset += 1;
    return pexec_lt(U);
    break;
  case LE:
    *offset += 1;
    return pexec_le(U);
    break;
  case CALL:
    *offset += 1;
    return pexec_call(U);
    break;
  case SWAP:
    *offset += 1;
    return pexec_swap(U);
    break;
  case RETURN:
    *offset += 1;
    return pexec_return(U);
    break;
  case DEL:
    *offset += 1;
    return pexec_del(U);
    break;
  case DO:
    *offset += 1;
    return pexec_do(U);
    break;
  case COPY:
    *offset += 1;
    return pexec_copy(U);
    break;

    /* manipulating data structures */
  case ARRACC:
    *offset += 1;
    return pexec_arracc(U);
    break;
  case TABACC:
    *offset += 1;
    return pexec_tabacc(U,NULL);
    break;
  case ARRSET:
    *offset += 1;
    return pexec_arrset(U);
    break;
  case TABSET:
    *offset += 1;
    return pexec_tabset(U,NULL);
    break;
  case TABKEEP:
    *offset += strlen(code)+2;
    return pexec_tabkeep(U,code);
  case ARRKEEP:
    *offset += 1;
    return pexec_arrkeep(U);
  case PUSHARGV:
    *offset += 1;
    return pexec_pushargv(U);
    break;
  case PUSHARGC:
    *offset += 1;
    return pexec_pushargc(U);
    break;
  case PUSHRETV:
    *offset += 1;
    return pexec_pushretv(U);
    break;
  case PUSHRETC:
    *offset += 1;
    return pexec_pushretc(U);
    break;
  case ARGV:
    *offset += 1;
    return pexec_argv(U);
    break;
  case ARGC:
    *offset += 1;
    return pexec_argc(U);
    break;
  case RETV:
    *offset += 1;
    return pexec_retv(U);
    break;
  case RETC:
    *offset += 1;
    return pexec_retc(U);
    break;
  case ASS:
    *offset += strlen(code+1)+3;
    return pexec_ass(U,*code,code+1);
    break;
  case GET:
    *offset += strlen(code+1)+3;
    return pexec_get(U,*code,code+1);
    break;
  case GETMETA:
    *offset += 1;
    return pexec_getmeta(U);
    break;
  case SETMETA:
    *offset += 1;
    return pexec_setmeta(U);
    break;
  case GETSUP:
    *offset += 1;
    return pexec_getsup(U);
    break;
  case SETSUP:
    *offset += 1;
    return pexec_setsup(U);
    break;
  case GLOBALS:
    *offset += 1;
    t = U->locals_stack_base[U->locals_stack_index];
    while (t->super_tag==POE_TABLE_TAG) t = t->super.poe_table;
    if (!poe_pushobj(U,(poe_obj)t,POE_TABLE_TAG)) return POE_FATAL_ERROR;
    return POE_CONTINUE;
    break;
  case LOCALS:
    *offset += 1;
    if (!poe_pushobj(U,(poe_obj)U->locals_stack_base[U->locals_stack_index],
		     POE_TABLE_TAG)) return POE_FATAL_ERROR;
    return POE_CONTINUE;
    break;
    /* iteration/control flow */
  case FORPREP:
    return pexec_forprep(U,code,offset);
    break;
  case FORITER:
    return pexec_foriter(U,offset);
    break;
  case WHILEPREP:
    return pexec_whileprep(U,code,offset);
    break;
  case WHILEITER:
    return pexec_whileiter(U,offset);
    break;
  case DOWPREP:
    return pexec_dowprep(U,code,offset);
    break;
  case IF:
    return pexec_if(U,code,offset);
    break;
  case ELSE:
    *offset += size_code(code,IFEND)+2;
    return POE_CONTINUE;
    break;
  case IFEND:
    *offset += 1;
    return POE_CONTINUE;
    break;
  case BREAK:
    return pexec_break(U,offset);
    break;
  case CONT:
    return pexec_cont(U,offset);
    break;
  default:
    return 0;
    break;
  }
  return 1;
}

char *pint_load_file(const char *s)
{
  struct stat st;
  int size, i, inchar;
  char *ret;
  FILE *f;
  if (!stat(s, &st)) size = st.st_size + 3;
  else return NULL;
  ret = malloc(size);
  if (!ret) return NULL;

  f = fopen(s,"r");
  if (!f) return NULL;

  for (i=0; (inchar = fgetc(f)) != EOF; i++) {
    ret[i] = inchar;
  }
  ret[i] = CODEEND;
  fclose(f);
  return ret;
}

char *pint_load_filef(FILE *f, int *sz)
{
  struct stat st;
  int size, i;
  char *ret;
  if (!fstat(fileno(f), &st)) size = st.st_size + 3;
  else return NULL;
  ret = malloc(size);
  if (!ret) return NULL;

  for (i=0; size>1; size--,i++) {
    ret[i] = fgetc(f);
  }
  ret[i] = CODEEND;
  return ret;
}
