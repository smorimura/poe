/* pbc.c
 * bytecode compilation
 */

#include <stdio.h>
#include <stdlib.h>
#include "pbc.h"
#include "lex.yy.h"
#include "y.tab.h"

extern FILE *yyin;
extern FILE *yyout;
extern FILE *ppbcout;

extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char *);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer );

int pbc_compile(FILE *in, FILE *out)
{
  yyin = in;
  ppbcout = out;
  return !yyparse();
}

// returns a pointer to a temporary file that contains the bytecode
FILE *pbc_compile_from_string(char *in)
{
  YY_BUFFER_STATE state = yy_scan_string(in);
  int inchar;
  ppbcout = tmpfile();
  if (!ppbcout) return NULL;
  if (yyparse()) return NULL;
  rewind(ppbcout);
  yy_delete_buffer(state);
  return ppbcout;
}

int pbc_compiles(char *in, char *out)
{
  int i;
  FILE *fin, *fout;
  fin = fopen(in,"r");
  if (!fin) return 0;
  fout = fopen(out,"w");
  if (!fout) return 0;
  i = pbc_compile(fin,fout);
  fclose(fin);
  fclose(fout);
  return i;
}
