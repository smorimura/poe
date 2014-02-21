/* pexec.h
 * Definitions and macros for executing strings of instructions (i.e. "code")
 * on Poe universes
 */
#ifndef PEXEC_H_
#define PEXEC_H_

#include "pdata.h"
#include "pmem.h"
#include "puniverse.h"
#include "pgc.h"
#include "pdo1.h"
#include "pdo2.h"

enum POE_INSTRS {
  /* CREATING VALUES/STRUCTURES */
  PUSHINT, PUSHFLOAT, PUSHCHAR, PUSHTRUE, PUSHFALSE, PUSHUNDEF, PUSHNULL,
  NEWTAB, NEWARR, NEWSTR, WRITESTR, PUSHFUNC, PUSHCODE, CODEEND, EXIT, ARGENUM, 
  /* OPERATORS */
  ADD, SUB, MULT, DIV, MOD, UNM, AND, OR, NOT, BAND, BOR, BXOR, BNOT, SHR,
  SHL, STRLEN, ARRMAX, EQ, LT, LE, DO, CALL, SWAP, RETURN, DEL, COPY,
  /* MANIPULATING DATA STRUCTURES */
  ARRACC, TABACC, ARRSET, TABSET, TABKEEP, ARRKEEP, PUSHARGV, PUSHARGC, 
  PUSHRETV, PUSHRETC, ARGV, ARGC, RETV, RETC, ASS, GET, LOCAL, NORMAL, GLOBAL, 
  EXTERN, GETMETA, SETMETA, GETSUP, SETSUP, GLOBALS, LOCALS,
  /* ITERATION AND CONTROL FLOW */
  FORPREP, FORITER, WHILEPREP, WHILEITER, DOWPREP, IF, ELSE, IFEND, BREAK, 
  CONT
};

enum POE_EXEC_SUCCESS {
  POE_FATAL_ERROR = 0, POE_CONTINUE = 1, POE_RETURN = 2, POE_CODEEND = 3,
  POE_HALT = 4
};

enum POE_METAS {
  meta_add, meta_sub, meta_mult, meta_div, meta_mod, meta_unm, meta_strlen,
  meta_arrmax, meta_eq, meta_lt, meta_le, meta_band, meta_bor, meta_bxor,
  meta_bnot, meta_tabacc, meta_arracc, meta_tabset, meta_arrset, meta_call,
  meta_do, meta_tostr, meta_type, meta_onfor, meta_metaacc,
  meta_metaset, meta_superacc, meta_superset
};

/* char *POE_METAKEYS[] = {
  "add", "sub", "mult", "div", "mod", "unm", "strlen", "arrmax", "eq", "lt",
  "le", "band", "bor", "bxor", "bnot", "tabacc", "stracc", "tabset", "strset",
  "call", "do", "tostr", "type", "onfor", "foriter", "metacc", "metaset",
  "superacc", "superset"
  }; */

#define getoption(i) ((i>>7)&1)
#define getinstr(i) (i&0x7F)

int poe_pushargv(poe_universe *, poe_arr *);
int poe_pushargc(poe_universe *, int);
int size_code(char *, char);
long parse_int(char, unsigned char *, char);
double parse_float(unsigned char *, char);
int poe_exec_instr(poe_universe *, char, char *, int *);
int poe_exec_code(poe_universe *, char *, int *);
int poe_truth_val(char);
int poe_pushobj(poe_universe *, poe_obj, char);
poe_obj get_metamethod(int, poe_obj, char, char *);
poe_obj poe_binhandler(int, poe_obj, char, poe_obj, char, char *);
poe_obj poe_cmphandler(int, poe_obj, char, poe_obj, char, char *);
int poe_binmeta(poe_universe *, int, poe_obj, char, poe_obj, char);
int poe_cmpmeta(poe_universe *, int, poe_obj, char, poe_obj, char);
int poe_unmeta(poe_universe *, int, poe_obj, char);
poe_obj poe_pop(poe_universe *, char *);

char *pint_load_file(const char *);
char *pint_load_filef(FILE *, int *);

#endif
