/* pbc.h
 * bytecode compilation
 */

#include "pdata.h"
#include "puniverse.h"
#include "lex.yy.h"

/* GENERAL COMPILATION MODUS:
1) Get a file stream of source code characters. Redirect that to yyin.
2) Find a file to write that stream to. Redirect that to yyout.
3) call yyparse.
*/

int pbc_compile(FILE *, FILE *);
int pbc_compiles(char *, char *);
FILE *pbc_compile_from_string(char *);
