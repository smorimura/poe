/* pdo2.h
 * Execution of data structure/iteration instructions on Poe Universes
 */

#ifndef PDO2_H_
#define PDO2_H_

#include "pdata.h"
#include "pmem.h"
#include "puniverse.h"
#include "pgc.h"
#include "pexec.h"
#include "pdo1.h"

// ARR INT -> ANY
int pexec_arracc(poe_universe *);
// TABLE STR -> ANY
int pexec_tabacc(poe_universe *, char *);
// ANY ARR INT -> X
int pexec_arrset(poe_universe *);
// ANY TABLE STR -> X
int pexec_tabset(poe_universe *, char *);
int pexec_tabkeep(poe_universe *, char *);
int pexec_arrkeep(poe_universe *);
// ARR -> X
int pexec_pushargv(poe_universe *);
// INT -> X
int pexec_pushargc(poe_universe *);
// ARR -> X
int pexec_pushretv(poe_universe *);
// INT -> X
int pexec_pushretc(poe_universe *);
int pexec_argv(poe_universe *);
int pexec_argc(poe_universe *);
int pexec_retv(poe_universe *);
int pexec_retc(poe_universe *);
int poe_extern_ass(poe_universe *, poe_table *, char *, poe_obj, char);
poe_obj poe_symbol_get(poe_universe *, poe_table *, char *, char *);
int pexec_ass(poe_universe *, char, char *);
int pexec_get(poe_universe *, char, char *);
int pexec_getmeta(poe_universe *);
int pexec_setmeta(poe_universe *);
int pexec_getsup(poe_universe *);
int pexec_setsup(poe_universe *);
// ITERABLE STRING STRING -> X
int pexec_forprep(poe_universe *, char *, int *);
// X -> X
int poe_table_dokeyval(poe_universe *, for_obj *, poe_obj_list *);
int poe_strarr_dokeyval(poe_universe *, for_obj *, int, poe_obj, char);
int poe_arr_next(poe_universe *, for_obj *, int *);
int poe_table_next(poe_universe *, for_obj *, int *);
int poe_str_next(poe_universe *, for_obj *, int *);
int pexec_foriter(poe_universe *, int *);
int pexec_whileprep(poe_universe *, char *, int *);
int pexec_whileiter(poe_universe *, int *);
int pexec_dowprep(poe_universe *, char *, int *);
int pexec_if(poe_universe *, char *, int *);
int pexec_break(poe_universe *, int *);
int pexec_cont(poe_universe *, int *);

#endif
