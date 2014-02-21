/* pdo.h
 * definitions for bytecode instruction execution
 */
#ifndef PDO1_H_
#define PDO1_H_

#include "pdata.h"
#include "pmem.h"
#include "puniverse.h"
#include "pexec.h"
#include "pgc.h"

int pexec_pushint(poe_universe *, long);
int pexec_pushfloat(poe_universe *, double);
int pexec_pushchar(poe_universe *, char);
int pexec_pushcfunc(poe_universe *, int (*cfunc) (poe_universe *) );
int pexec_pushtrue(poe_universe *);
int pexec_pushfalse(poe_universe *);
int pexec_pushundef(poe_universe *);
int pexec_pushnull(poe_universe *);
int pexec_newtab(poe_universe *);
int pexec_newarr(poe_universe *);
int pexec_newstr(poe_universe *);
int pexec_writestr(poe_universe *, char *, int *);
int pexec_pushfunc(poe_universe *, char *, int *);
int pexec_pushcode(poe_universe *, char *, int *);
int pexec_argenum(poe_universe *, char *, int *);
int pexec_add(poe_universe *);
int pexec_sub(poe_universe *);
int pexec_mult(poe_universe *);
int pexec_div(poe_universe *);
int pexec_mod(poe_universe *);
int pexec_unm(poe_universe *);
int pexec_and(poe_universe *);
int pexec_or(poe_universe *);
int pexec_not(poe_universe *);
int pexec_band(poe_universe *);
int pexec_bor(poe_universe *);
int pexec_bxor(poe_universe *);
int pexec_bnot(poe_universe *);
int pexec_shr(poe_universe *);
int pexec_shl(poe_universe *);
int pexec_strlen(poe_universe *);
int pexec_arrmax(poe_universe *);
int pexec_eq(poe_universe *);
int pexec_lt(poe_universe *);
int pexec_le(poe_universe *);
int pexec_call(poe_universe *);
int pexec_swap(poe_universe *);
int pexec_return(poe_universe *);
int pexec_del(poe_universe *);
int pexec_do(poe_universe *);
int pexec_copy(poe_universe *);

#endif
