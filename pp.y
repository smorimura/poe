%{
#include "y.tab.h"
  /* INSTRUCTIONS FOR COMPILING WITH YACC/BISON:
1) Let's assume this file is saved on your machine as ppy.y
2) Run yacc or bison to create an output file without #line directives as well
as a header file. On my machine, the command required is
bison -l -d ppy.y
3) This will produce a source code file y.tab.c and a header file y.tab.h.
As produced, the header file won't be compileable; we need to add the
struct definitions, enum definitions, and function declarations to it.
That information is given in the comment section directly below these 
instructions. 
4) Copy-paste the commented-out block of text here (starting at #ifndef Y_TAB_H
and ending at void yyerror(char *);), uncommented, to the very top of y.tab.h.
5) At the very bottom of y.tab.h, add the following line of text:
#endif
6) y.tab.h and y.tab.c should now compile with no further modifications.
  */
  // EXTRA y.tab.h INFORMATION HERE
  /*
#ifndef Y_TAB_H
#define Y_TAB_H
#include "pexec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct pptree_node pptree;
typedef union treedata {
  pptree *tree;
  char *str;
  long i;
  char c;
  double f;
 } treedata;
struct pptree_node {
  int type;
  treedata l1;
  treedata l2;
};
pptree *new_pptree(int, treedata, treedata);
enum PPTYPES {
  PPBLOCK, PPARR, PPTAB, PPOR, PPAND, PPLT, PPGT, PPLEQ, PPGEQ,
  PPEQ, PPNEQ, PPBOR, PPBXOR, PPBAND, PPPLUS, PPMINUS, PPSHL, PPSHR, PPMULT,
  PPDIV, PPMOD, PPNOT, PPLEN, PPUNM, PPMAX, PPBNOT, PPCALL, PPARRACC, PPTABACC,
  PPLTABACC, PPSTRLIT, PPMETA, PPSUP, PPINTLIT, PPCHRLIT, PPFLTLIT, PPFUNC, 
  PPCODE, PPIDENT, PPUNDEF, PPFALSE, PPNULL, PPTRUE, PPARGC, PPARGV, PPRETC, 
  PPRETV, PPGLOBALS, PPLOCALS, PPEXPRLIST, PPASSLIST, PPASSPAIR, PPIF, PPUNLESS,
  PPCOND, PPNONE, PPWHILE, PPUNTIL, PPDOW, PPDOU, PPFOR, PPIPAIR, PPFPAIR,
  PPASSIGN, PPRETURN, PPRETURNVC, PPCALLVC, PPEXIT, PPEXITVC, PPBREAK, 
  PPCONTINUE, PPDO
};
void pp_write_stm(pptree *);
void pp_write_block(pptree *);
void pp_free_tree(pptree *);

int pp_assign(pptree *);
int pp_enumerate_assign(pptree *);
 void pp_write_else(pptree *);

 void pp_write_expr(pptree *);

void pp_encode_int(long);
void pp_encode_float(double);

int len_expr_list(pptree *);
void pp_write_argenum(pptree *);
int pp_write_callnum(pptree *);
void pp_write_tablit(pptree *);
void yyerror(char *);

extern int yylineno;
  */

FILE *ppbcout;
%}

%union {
  long intval;
  char chrval;
  double fltval;
  char *strval;
  pptree *pptval;
};

%token PUNDEF PFALSE PNULL PTRUE
%token PARGC PARGV PRETC PRETV
%token PAND POR PNOT
%token <intval> PGLOBAL PGLOBALS PLOCAL PLOCALS PEXTERN PNORMAL
%token PFUNC PCALLVC PRETURNVC PEXIT PEXITVC PRETURN
%token PWHILE PUNLESS PFOR PIF PELSE PDO PUNTIL PEND PIN
%token PBLOCK
%token PEOF
%token PERROR PSUCCESS
%token PBREAK PCONTINUE

%token <strval> PSTR
%token <strval> PIDENTIFIER
%token <intval> PINT
%token <fltval> PFLT
%token <chrval> PCHR

%type <pptval> expr array table block statement script expr_list ass_list
%type <pptval> else_stm if_stm loop_stm do_stm for_stm function code 

%left POR
%left PAND
%left '<' PLEQ '>' PGEQ '=' PEQ PNEQ
%left '|'
%left '^'
%left '&'
%left '+' '-'
%left PSHL PSHR
%left '*' '/' '%'
%left PNOT '#' PUNMINUS '@' '~'
%left '.' PMET PSUP

%start script
%%
array:
'[' expr_list ']'   { $$ = new_pptree(PPARR, (treedata)$2, (treedata)0L); }
| '[' ']'           { $$ = new_pptree(PPARR, (treedata)0L, (treedata)0L); }
  ;
table:
'{' ass_list '}' { $$ = new_pptree(PPTAB, (treedata)$2, (treedata)0L); }  
| '{' '}'        { $$ = new_pptree(PPTAB, (treedata)0L, (treedata)0L); }
  ;

expr:
expr POR expr     { $$ = new_pptree(PPOR, (treedata)$1, (treedata)$3); }
| expr PAND expr  { $$ = new_pptree(PPAND, (treedata)$1, (treedata)$3); }
| expr '<' expr   { $$ = new_pptree(PPLT, (treedata)$1, (treedata)$3); }
| expr '>' expr   { $$ = new_pptree(PPGT, (treedata)$1, (treedata)$3); }
| expr PLEQ expr  { $$ = new_pptree(PPLEQ, (treedata)$1, (treedata)$3); }
| expr PGEQ expr  { $$ = new_pptree(PPGEQ, (treedata)$1, (treedata)$3); }
| expr PEQ expr   { $$ = new_pptree(PPEQ, (treedata)$1, (treedata)$3); }
| expr PNEQ expr  { $$ = new_pptree(PPNEQ, (treedata)$1, (treedata)$3); }
| expr '|' expr    { $$ = new_pptree(PPBOR, (treedata)$1, (treedata)$3); }
| expr '^' expr    { $$ = new_pptree(PPBXOR, (treedata)$1, (treedata)$3); }
| expr '&' expr    { $$ = new_pptree(PPBAND, (treedata)$1, (treedata)$3); }
| expr '+' expr    { $$ = new_pptree(PPPLUS, (treedata)$1, (treedata)$3); }
| expr '-' expr    { $$ = new_pptree(PPMINUS, (treedata)$1, (treedata)$3); }
| expr PSHL expr   { $$ = new_pptree(PPSHL, (treedata)$1, (treedata)$3); }
| expr PSHR expr   { $$ = new_pptree(PPSHR, (treedata)$1, (treedata)$3); }
| expr '*' expr    { $$ = new_pptree(PPMULT, (treedata)$1, (treedata)$3); }
| expr '/' expr    { $$ = new_pptree(PPDIV, (treedata)$1, (treedata)$3); }
| expr '%' expr    { $$ = new_pptree(PPMOD, (treedata)$1, (treedata)$3); }
| PNOT expr        { $$ = new_pptree(PPNOT, (treedata)$2, (treedata)0L); }
| '#' expr         { $$ = new_pptree(PPLEN, (treedata)$2, (treedata)0L); }
| '-' expr %prec PUNMINUS  {
  $$ = new_pptree(PPUNM, (treedata)$2, (treedata)0L); }
| '@' expr         { $$ = new_pptree(PPMAX, (treedata)$2, (treedata)0L); }
| '~' expr         { $$ = new_pptree(PPBNOT, (treedata)$2, (treedata)0L); }
| expr '(' expr_list ')'   { $$ = new_pptree(PPCALL, (treedata)$1, 
					     (treedata)$3); }
| expr '(' ')'             { $$ = new_pptree(PPCALL, (treedata)$1,
					     (treedata)0L); }
| expr '[' expr ']'        { $$ = new_pptree(PPARRACC, (treedata)$1, 
					     (treedata)$3); }
| expr '.' PIDENTIFIER     
{ $$ = new_pptree(PPTABACC, (treedata)$1, 
		  (treedata)new_pptree(PPIDENT, (treedata)(long)PNORMAL,
				       (treedata)$3)); }
| expr '.' PSTR            
{ $$ = new_pptree(PPTABACC, (treedata)$1,
		  (treedata)new_pptree(PPSTRLIT,
				       (treedata)$3,
				       (treedata)0L)); } 
| expr '.' '(' expr ')'    { $$ = new_pptree(PPLTABACC, (treedata)$1, 
					     (treedata)$4); }
| expr PMET                { $$ = new_pptree(PPMETA, (treedata)$1, 
					     (treedata)0L); }
| expr PSUP                { $$ = new_pptree(PPSUP, (treedata)$1, 
					     (treedata)0L); }
| PINT                     { $$ = new_pptree(PPINTLIT, (treedata)$1, 
					     (treedata)0L); }
| PCHR                     { $$ = new_pptree(PPCHRLIT, (treedata)$1, 
					     (treedata)0L); }
| PSTR                     { $$ = new_pptree(PPSTRLIT, (treedata)$1, 
					     (treedata)0L); }
| PFLT                     { $$ = new_pptree(PPFLTLIT, (treedata)$1,
					     (treedata)0L); }
| array                    { $$ = $1; }
| table                    { $$ = $1; }
| function                 { $$ = $1; }
| code                     { $$ = $1; } 
| PLOCAL PIDENTIFIER       { $$ = new_pptree(PPIDENT, (treedata)(long)LOCAL, 
					     (treedata)$2); }
| PGLOBAL PIDENTIFIER      { $$ = new_pptree(PPIDENT, (treedata)(long)GLOBAL, 
					     (treedata)$2); }
| PEXTERN PIDENTIFIER      { $$ = new_pptree(PPIDENT, (treedata)(long)EXTERN, 
					     (treedata)$2); }
| PIDENTIFIER              { $$ = new_pptree(PPIDENT, (treedata)(long)NORMAL, 
					     (treedata)$1); }
| '(' expr ')'             { $$ = $2; }
| PUNDEF                   { $$ = new_pptree(PPUNDEF, (treedata)0L, 
					     (treedata)0L); }
| PFALSE                   { $$ = new_pptree(PPFALSE, (treedata)0L, 
					     (treedata)0L); }
| PNULL                    { $$ = new_pptree(PPNULL, (treedata)0L, 
					     (treedata)0L); }
| PTRUE                    { $$ = new_pptree(PPTRUE, (treedata)0L, 
					     (treedata)0L); }
| PARGC                    { $$ = new_pptree(PPARGC, (treedata)0L, 
					     (treedata)0L); }
| PARGV                    { $$ = new_pptree(PPARGV, (treedata)0L, 
					     (treedata)0L); }
| PRETC                    { $$ = new_pptree(PPRETC, (treedata)0L, 
					     (treedata)0L); }
| PRETV                    { $$ = new_pptree(PPRETV, (treedata)0L, 
					     (treedata)0L); }
| PGLOBALS                 { $$ = new_pptree(PPGLOBALS, (treedata)0L, 
					     (treedata)0L); }
| PLOCALS                  { $$ = new_pptree(PPLOCALS, (treedata)0L, 
					     (treedata)0L); }
;

expr_list:
expr_list ',' expr         { $$ = new_pptree(PPEXPRLIST, (treedata)$1, 
					     (treedata)$3); }
| expr                     { $$ = new_pptree(PPEXPRLIST, (treedata)0L, 
					     (treedata)$1); }
;

ass_list:
ass_list ',' PIDENTIFIER '=' expr 
{ $$ = new_pptree(PPASSLIST, (treedata)$1,
		  (treedata)new_pptree(PPASSPAIR,
				       (treedata)
				       new_pptree(PPIDENT,
						  (treedata)(long)PNORMAL,
						  (treedata)$3),
				       (treedata)$5)); }
| PIDENTIFIER '=' expr   { $$ = new_pptree(PPASSPAIR,
					   (treedata)new_pptree(PPIDENT,
						      (treedata)(long)PNORMAL,
								(treedata)$1),
					   (treedata)$3); }
;

else_stm:
PELSE ':' block PEND ';'    { $$ = $3;}
| PELSE if_stm  { $$ = $2; }
;

if_stm:
PIF expr ':' block else_stm 
{ $$ = new_pptree(PPIF,
		  (treedata)$2,
		  (treedata)new_pptree(PPCOND,(treedata)$4,(treedata)$5)); }
| PIF expr ':' block PEND ';'
{ $$ = new_pptree(PPIF,
		  (treedata)$2,
		  (treedata)new_pptree(PPCOND,(treedata)$4,
				       (treedata)new_pptree(PPNONE,(treedata)0L,
							    (treedata)0L))); }
| PUNLESS expr ':' block else_stm
{ $$ = new_pptree(PPUNLESS,
		  (treedata)$2,
		  (treedata)new_pptree(PPCOND,(treedata)$4,(treedata)$5)); }
| PUNLESS expr ':' block PEND ';'
  { $$ = new_pptree(PPUNLESS,
		    (treedata)$2,
		    (treedata)new_pptree(PPCOND,(treedata)$4,
					 (treedata)new_pptree(PPNONE,
							      (treedata)0L,
							      (treedata)0L))); }
;

loop_stm:
PWHILE expr ':' block PEND ';'
{ $$ = new_pptree(PPWHILE,(treedata)$2,(treedata)$4); }
| PUNTIL expr ':' block PEND ';'
{ $$ = new_pptree(PPUNTIL,(treedata)$2,(treedata)$4); }
;

do_stm:
PDO ':' block PWHILE expr ';'
{ $$ = new_pptree(PPDOW,(treedata)$5,(treedata)$3); }
| PDO ':' block PUNTIL expr ';'
{ $$ = new_pptree(PPDOU,(treedata)$5,(treedata)$3); }
;

for_stm:
PFOR PIDENTIFIER ',' PIDENTIFIER PIN expr ':' block PEND ';'
{ $$ = new_pptree(PPFOR,
		  (treedata)new_pptree(PPIPAIR,(treedata)$2,(treedata)$4),
		  (treedata)new_pptree(PPFPAIR,(treedata)$6,(treedata)$8)); }
;

statement:
expr_list '=' expr ';' { $$ = new_pptree(PPASSIGN,(treedata)$1,(treedata)$3); }
| expr ';'         { $$ = $1; }
| if_stm           { $$ = $1; }
| loop_stm         { $$ = $1; }
| do_stm           { $$ = $1; }
| for_stm          { $$ = $1; }
| PRETURN '(' expr_list ')' ';'   { $$ = new_pptree(PPRETURN,(treedata)$3,
						 (treedata)0L); }
| PRETURN '(' ')' ';'             { $$ = new_pptree(PPRETURN,(treedata)0L,
						    (treedata)0L); }
| PRETURNVC '(' expr_list ')' ';' { $$ = new_pptree(PPRETURNVC,(treedata)$3,
						 (treedata)0L); }
| PEXIT '(' expr_list ')'  ';'    { $$ = new_pptree(PPEXIT,(treedata)$3,
						 (treedata)0L); }
| PEXIT '(' ')' ';'               { $$ = new_pptree(PPEXIT,(treedata)0L,
						    (treedata)0L); }
| PEXITVC '(' expr_list ')' ';'   { $$ = new_pptree(PPEXITVC,(treedata)$3,
						 (treedata)0L); }
| PDO expr ';'                 { $$ = new_pptree(PPDO, (treedata)$2,
						 (treedata)0L); }
| PBREAK ';'                   { $$ = new_pptree(PPBREAK,(treedata)0L,
						 (treedata)0L); }
| PCONTINUE ';'                { $$ = new_pptree(PPCONTINUE,(treedata)0L,
						 (treedata)0L); }
| ';'                          { $$ = new_pptree(PPNONE, (treedata)0L,
						 (treedata)0L); }
;

function:
PFUNC '(' expr_list ')' ':' block PEND
{ $$ = new_pptree(PPFUNC,(treedata)$3,(treedata)$6); }
| PFUNC '(' ')' ':' block PEND
{ $$ = new_pptree(PPFUNC,(treedata)0L,(treedata)$5); }
| PCALLVC '(' expr_list ')'   { $$ = new_pptree(PPCALLVC,(treedata)$3,
						 (treedata)0L); }
;

code:
PBLOCK ':' block PEND { $$ = new_pptree(PPCODE,(treedata)$3,(treedata)0L); }
;

block:
block statement { $$ = new_pptree(PPBLOCK,(treedata)$1,(treedata)$2); }
| /* NULL */  { $$ = new_pptree(PPNONE,(treedata)0L,(treedata)0L); }
  ;
script:
block         { pp_write_block($1); pp_free_tree($1); 
  fputc(CODEEND,ppbcout); YYACCEPT; }
;

%%

pptree *new_pptree(int type, treedata l1, treedata l2)
{
  pptree *new = malloc(sizeof(pptree));
  if (!new) return NULL;
  new->type = type;
  new->l1 = l1;
  new->l2 = l2;
  return new;
}

/* pp_assign: assume an item is at the top of the stack, and write an assignment
   of that item to the slot specified by the given tree. */
int pp_assign(pptree *tree)
{
  switch (tree->type) {
  case PPIDENT:
    fputc(ASS,ppbcout);
    fputc(tree->l1.i,ppbcout);
    fputs(tree->l2.str,ppbcout);
    fputc('\0',ppbcout);
    break;
  case PPTABACC:
    pp_write_expr(tree->l1.tree);
    switch (tree->l2.tree->type) {
    case PPIDENT:
      fputc(NEWSTR,ppbcout);
      fputc(WRITESTR,ppbcout);
      fputs(tree->l2.tree->l2.str,ppbcout);
      fputc('\0',ppbcout);
      break;
    case PPSTRLIT:
      fputc(NEWSTR,ppbcout);
      fputc(WRITESTR,ppbcout);
      fputs(tree->l2.tree->l1.str,ppbcout);
      fputc('\0',ppbcout);
      break;
    default:
      pp_write_expr(tree->l2.tree);
      break;
    }
    fputc(TABSET,ppbcout);
    break;
  case PPLTABACC:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(TABSET,ppbcout);
    break;
  case PPARRACC:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(ARRSET,ppbcout);
    break;
  case PPMETA:
    pp_write_expr(tree->l1.tree);
    fputc(SETMETA,ppbcout);
    break;
  case PPSUP:
    pp_write_expr(tree->l1.tree);
    fputc(SETSUP,ppbcout);
    break;
  default:
    printf("parse error: invalid assignment\n"); exit(1);
  }
  return 1;
}

/* pp_enumerate_assign: assign a particular index of argv to the given
   expression in the expression list; determine index through recursion.
   The given trees must always be expr_lists. */
int pp_enumerate_assign(pptree *tree)
{
  int index;
  if (tree->l1.i==0) 
    index = 0;
  else
    index = pp_enumerate_assign(tree->l1.tree);
  fputc(RETV,ppbcout);
  fputc(PUSHINT,ppbcout);
  pp_encode_int(index);
  fputc(ARRACC,ppbcout);
  pp_assign(tree->l2.tree);
  return index+1;
}

void pp_write_else(pptree *tree)
{
  switch (tree->type) {
  case PPBLOCK:
    pp_write_block(tree);
    break;
  case PPIF:
  case PPUNLESS:
    pp_write_stm(tree);
    break;
  case PPNONE:
  default:
    break;
  }
}

void pp_write_stm(pptree *tree)
{
  int i;
  pptree *t;
  switch (tree->type) {
  case PPASSIGN:
    pp_write_expr(tree->l2.tree);
    if ((tree->l1.tree->l1.i==0)&&
	(tree->l1.tree->l2.tree->type==PPIDENT)) {
      /* CASE: we can do a simple assignment */
      fputc(ASS,ppbcout);
      fputc(tree->l1.tree->l2.tree->l1.i,ppbcout);
      fputs(tree->l1.tree->l2.tree->l2.str,ppbcout);
      fputc('\0',ppbcout);
    } else if (tree->l1.tree->l1.i==0) {
      /* CASE: only one assignment */
      pp_assign(tree->l1.tree->l2.tree);
    } else if (tree->l1.tree->l1.i!=0) {
      /* CASE: there is more than one assignment to do */
      fputc(DEL,ppbcout);
      pp_enumerate_assign(tree->l1.tree);
    }
    break;
  case PPIF: 
    pp_write_expr(tree->l1.tree);
    fputc(IF,ppbcout);
    pp_write_block(tree->l2.tree->l1.tree);
    fputc(ELSE,ppbcout);
    pp_write_else(tree->l2.tree->l2.tree);
    fputc(IFEND,ppbcout);
    break;
  case PPUNLESS: 
    pp_write_expr(tree->l1.tree);
    fputc(NOT,ppbcout);
    fputc(IF,ppbcout);
    pp_write_block(tree->l2.tree->l1.tree);
    fputc(ELSE,ppbcout);
    pp_write_else(tree->l2.tree->l2.tree);
    fputc(IFEND,ppbcout);
    break; 
  case PPBREAK:
    fputc(BREAK,ppbcout);
    break;
  case PPCONTINUE:
    fputc(CONT,ppbcout);
    break;
  case PPWHILE: 
    fputc(WHILEPREP,ppbcout);
    pp_write_expr(tree->l1.tree);
    fputc(CODEEND,ppbcout);
    pp_write_block(tree->l2.tree);
    fputc(WHILEITER,ppbcout);
    break; 
  case PPUNTIL: 
    fputc(WHILEPREP,ppbcout);
    pp_write_expr(tree->l1.tree);
    fputc(NOT,ppbcout);
    fputc(CODEEND,ppbcout);
    pp_write_block(tree->l2.tree);
    fputc(WHILEITER,ppbcout);
    break; 
  case PPDO:
    pp_write_expr(tree->l1.tree);
    fputc(DO,ppbcout);
    break;
  case PPDOW:
    fputc(DOWPREP,ppbcout);
    pp_write_expr(tree->l1.tree);
    fputc(CODEEND,ppbcout);
    pp_write_block(tree->l2.tree);
    fputc(WHILEITER,ppbcout);
    break; 
  case PPDOU:
    fputc(WHILEPREP,ppbcout);
    pp_write_expr(tree->l1.tree);
    fputc(NOT,ppbcout);
    fputc(CODEEND,ppbcout);
    pp_write_block(tree->l2.tree);
    fputc(WHILEITER,ppbcout);
    break; 
  case PPFOR: 
    pp_write_expr(tree->l2.tree->l1.tree);
    fputc(NEWSTR,ppbcout);
    fputc(WRITESTR,ppbcout);
    fputs(tree->l1.tree->l1.str,ppbcout);
    fputc('\0',ppbcout);
    fputc(NEWSTR,ppbcout);
    fputc(WRITESTR,ppbcout);
    fputs(tree->l1.tree->l2.str,ppbcout);
    fputc('\0',ppbcout);
    fputc(FORPREP,ppbcout);
    pp_write_block(tree->l2.tree->l2.tree);
    fputc(FORITER,ppbcout);
    break;
  case PPRETURN:
    fputc(NEWARR,ppbcout);
    i = pp_write_callnum(tree->l1.tree);
    fputc(PUSHRETV,ppbcout);
    fputc(PUSHINT,ppbcout);
    pp_encode_int(i);
    fputc(PUSHRETC,ppbcout);
    fputc(RETURN,ppbcout);
    break;
  case PPRETURNVC:
    if (len_expr_list(tree->l1.tree)!=2) exit(1);
    pp_write_expr(tree->l1.tree->l1.tree->l2.tree);
    fputc(PUSHRETV,ppbcout);
    pp_write_expr(tree->l1.tree->l2.tree);
    fputc(PUSHRETC,ppbcout);
    fputc(RETURN,ppbcout);
    break;
  case PPEXIT:
    fputc(NEWARR,ppbcout);
    i = pp_write_callnum(tree->l1.tree);
    fputc(PUSHARGV,ppbcout);
    fputc(PUSHINT,ppbcout);
    pp_encode_int(i);
    fputc(PUSHARGC,ppbcout);
    fputc(EXIT,ppbcout);
    break;
  case PPEXITVC:
    if (len_expr_list(tree->l1.tree)!=2) exit(1);
    pp_write_expr(tree->l1.tree->l1.tree->l2.tree);
    fputc(PUSHRETV,ppbcout);
    pp_write_expr(tree->l1.tree->l2.tree);
    fputc(PUSHRETC,ppbcout);
    fputc(EXIT,ppbcout);
    break;
  default:  // just normal expression
    pp_write_expr(tree);
    fputc(DEL,ppbcout);
    break;
  }
}

int len_expr_list(pptree *tree)
{
  if (tree==NULL) return 0;
  if (!tree->l1.tree) {
    return 1;
  } else return 1+len_expr_list(tree->l1.tree);
}

void pp_write_argenum(pptree *tree)
{
  if (!tree) return;
  if (!tree->l1.tree) { // case: first argument
    if (tree->l2.tree->type!=PPIDENT) {
      printf("pparse: argument found that is not simple identifier\n");
      exit(1);
    }
    fputs(tree->l2.tree->l2.str,ppbcout);
    fputc('\0',ppbcout);
  } else {
    pp_write_argenum(tree->l1.tree);
    fputs(tree->l2.tree->l2.str,ppbcout);
    fputc('\0',ppbcout);
  }
}

/* assume argv is always on stack */
int pp_write_callnum(pptree *tree)
{
  int index;
  if (!tree) return 0;
  if (!tree->l1.tree) { // case: first argument
    index = 0;
  } else {
    index = pp_write_callnum(tree->l1.tree);
  }
  pp_write_expr(tree->l2.tree);
  fputc(PUSHINT,ppbcout);
  pp_encode_int(index);
  fputc(ARRKEEP,ppbcout);
  return index+1;
}

// assume table is already at top of stack
void pp_write_tablit(pptree *tree)
{
  if (tree->type==PPASSLIST) {
    pp_write_tablit(tree->l1.tree);
    tree = tree->l2.tree;
  } 
  pp_write_expr(tree->l2.tree);
  fputc(TABKEEP,ppbcout);
  fputs(tree->l1.tree->l2.str,ppbcout);
  fputc('\0',ppbcout);
  return;
}

void pp_write_expr(pptree *tree)
{
  int i;
  switch (tree->type) {
  case PPTAB:
    fputc(NEWTAB,ppbcout);
    if (tree->l1.tree) pp_write_tablit(tree->l1.tree);
    break;
  case PPARR:
    fputc(NEWARR,ppbcout);
    if (tree->l1.i!=0) 
      pp_write_callnum(tree->l1.tree);
    break;
  case PPOR:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(OR,ppbcout);
    break;
  case PPAND:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(AND,ppbcout);
    break;
  case PPLT:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(LT,ppbcout);
    break;
  case PPGT:
    pp_write_expr(tree->l2.tree);
    pp_write_expr(tree->l1.tree);
    fputc(LT,ppbcout);
    break;
  case PPLEQ:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(LE,ppbcout);
    break;
  case PPGEQ:
    pp_write_expr(tree->l2.tree);
    pp_write_expr(tree->l1.tree);
    fputc(LE,ppbcout);
    break;
  case PPEQ:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(EQ,ppbcout);
    break;
  case PPNEQ:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(EQ,ppbcout);
    fputc(NOT,ppbcout);
    break;
  case PPBOR:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(BOR,ppbcout);
    break;
  case PPBXOR:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(BXOR,ppbcout);
    break;
  case PPBAND:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(BAND,ppbcout);
    break;
  case PPPLUS:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(ADD,ppbcout);
    break;
  case PPMINUS:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(SUB,ppbcout);
    break;
  case PPSHL:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(SHL,ppbcout);
    break;
  case PPSHR:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(SHR,ppbcout);
    break;
  case PPMULT:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(MULT,ppbcout);
    break;
  case PPDIV:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(DIV,ppbcout);
    break;
  case PPMOD:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(MOD,ppbcout);
    break;
  case PPNOT:
    pp_write_expr(tree->l1.tree);
    fputc(NOT,ppbcout);
    break;
  case PPLEN:
    pp_write_expr(tree->l1.tree);
    fputc(STRLEN,ppbcout);
    break;
  case PPUNM:
    pp_write_expr(tree->l1.tree);
    fputc(UNM,ppbcout);
    break;
  case PPMAX:
    pp_write_expr(tree->l1.tree);
    fputc(ARRMAX,ppbcout);
    break;
  case PPBNOT:
    pp_write_expr(tree->l1.tree);
    fputc(BNOT,ppbcout);
    break;
  case PPCALL:
    if (!tree->l2.tree) {
      pp_write_expr(tree->l1.tree);
      fputc(NEWARR,ppbcout);
      fputc(PUSHARGV,ppbcout);
      fputc(PUSHINT,ppbcout);
      fputc(0,ppbcout);
      fputc(PUSHARGC,ppbcout);
      fputc(CALL,ppbcout);
    } else {
      fputc(NEWARR,ppbcout);
      i = pp_write_callnum(tree->l2.tree);
      fputc(PUSHARGV,ppbcout);
      fputc(PUSHINT,ppbcout);
      pp_encode_int(i);
      fputc(PUSHARGC,ppbcout);
      pp_write_expr(tree->l1.tree);
      fputc(CALL,ppbcout);
    }
    break;
  case PPARRACC:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(ARRACC,ppbcout);
    break;
  case PPTABACC:
    pp_write_expr(tree->l1.tree);
    switch (tree->l2.tree->type) {
    case PPIDENT:
      fputc(NEWSTR,ppbcout);
      fputc(WRITESTR,ppbcout);
      fputs(tree->l2.tree->l2.str,ppbcout);
      fputc('\0',ppbcout);
      break;
    case PPSTRLIT:
      fputc(NEWSTR,ppbcout);
      fputc(WRITESTR,ppbcout);
      fputs(tree->l2.tree->l1.str,ppbcout);
      fputc('\0',ppbcout);
      break;
    default:
      pp_write_expr(tree->l2.tree);
      break;
    }
    fputc(TABACC,ppbcout);
    break;
  case PPLTABACC:
    pp_write_expr(tree->l1.tree);
    pp_write_expr(tree->l2.tree);
    fputc(TABACC,ppbcout);
    break;
  case PPSTRLIT:
    fputc(NEWSTR,ppbcout);
    fputc(WRITESTR,ppbcout);
    fputs(tree->l1.str,ppbcout);
    fputc('\0',ppbcout);
    break;
  case PPMETA:
    pp_write_expr(tree->l1.tree);
    fputc(GETMETA,ppbcout);
    break;
  case PPSUP:
    pp_write_expr(tree->l1.tree);
    fputc(GETSUP,ppbcout);
    break;
  case PPINTLIT:
    if (tree->l1.i<0) fputc(PUSHINT|(1<<7),ppbcout);
    else fputc(PUSHINT,ppbcout);
    pp_encode_int(tree->l1.i);
    break;
  case PPCHRLIT:
    fputc(PUSHCHAR,ppbcout);
    fputc(tree->l1.c,ppbcout);
    break;
  case PPFLTLIT:
    if (tree->l1.f<0.0) fputc(PUSHFLOAT|(1<<7),ppbcout);
    else fputc(PUSHFLOAT,ppbcout);
    pp_encode_float(tree->l1.f);
    break;
  case PPFUNC:
    fputc(PUSHFUNC,ppbcout);
    fputc(ARGENUM,ppbcout);
    pp_write_argenum(tree->l1.tree);
    fputc('\0',ppbcout);
    pp_write_block(tree->l2.tree);
    fputc(CODEEND,ppbcout);
    break;
  case PPCALLVC:
    if (len_expr_list(tree->l1.tree)!=3) { 
      yyerror("callvc syntax error\n");
      exit(1);
    }
    pp_write_expr(tree->l1.tree->l1.tree->l2.tree);
    fputc(PUSHARGV,ppbcout);
    pp_write_expr(tree->l1.tree->l2.tree);
    fputc(PUSHARGC,ppbcout);
    pp_write_expr(tree->l1.tree->l1.tree->l1.tree->l2.tree);
    fputc(CALL,ppbcout);
    break;
  case PPCODE:
    fputc(PUSHCODE,ppbcout);
    pp_write_block(tree->l1.tree);
    fputc(CODEEND,ppbcout);
    break;
  case PPIDENT:
    fputc(GET,ppbcout);
    fputc(tree->l1.i,ppbcout);
    fputs(tree->l2.str,ppbcout);
    fputc('\0',ppbcout);
    break;
  case PPUNDEF:
    fputc(PUSHUNDEF,ppbcout);
    break;
  case PPFALSE:
    fputc(PUSHFALSE,ppbcout);
    break;
  case PPNULL:
    fputc(PUSHNULL,ppbcout);
    break;
  case PPTRUE:
    fputc(PUSHTRUE,ppbcout);
    break;
  case PPARGC:
    fputc(ARGC,ppbcout);
    break;
  case PPARGV:
    fputc(ARGV,ppbcout);
    break;
  case PPRETC:
    fputc(RETC,ppbcout);
    break;
  case PPRETV:
    fputc(RETV,ppbcout);
    break;
  case PPGLOBALS:
    fputc(GLOBALS,ppbcout);
    break;
  case PPLOCALS:
    fputc(LOCALS,ppbcout);
    break;
  }
}

void pp_write_block(pptree *tree)
{
  if (tree->type==PPNONE) return;
  pp_write_block(tree->l1.tree);
  pp_write_stm(tree->l2.tree);
  return;
}

void pp_free_tree(pptree *tree)
{
  if (!tree) return;
  switch (tree->type) {
  case PPBLOCK:
    pp_free_tree(tree->l1.tree);
    pp_free_tree(tree->l2.tree);
    free(tree);
    break;
  case PPARR:
  case PPTAB:
    if (tree->l1.tree!=NULL) pp_free_tree(tree->l1.tree);
    free(tree);
    break;
  case PPOR:
  case PPAND:
  case PPLT:
  case PPGT:
  case PPLEQ:
  case PPGEQ:
  case PPEQ:
  case PPNEQ:
  case PPBOR:
  case PPBXOR:
  case PPBAND:
  case PPPLUS:
  case PPMINUS:
  case PPSHL:
  case PPSHR:
  case PPMULT:
  case PPDIV:
  case PPMOD:
    pp_free_tree(tree->l1.tree);
    pp_free_tree(tree->l2.tree);
    free(tree);
    break;
  case PPNOT:
  case PPLEN:
  case PPUNM:
  case PPMAX:
  case PPBNOT:
    pp_free_tree(tree->l1.tree);
    free(tree);
    break;
  case PPCALL:
  case PPARRACC:
  case PPTABACC:
  case PPLTABACC:
    pp_free_tree(tree->l1.tree);
    pp_free_tree(tree->l2.tree);
    free(tree);
    break;
  case PPSTRLIT:
    free(tree->l1.str);
    free(tree);
    break;
  case PPMETA:
  case PPSUP:
    pp_free_tree(tree->l1.tree);
    free(tree);
    break;
  case PPINTLIT:
  case PPCHRLIT:
  case PPFLTLIT:
    free(tree);
    break;
  case PPFUNC:
    if (tree->l1.i!=0) pp_free_tree(tree->l1.tree);
    pp_free_tree(tree->l2.tree);
    free(tree);
    break;
  case PPCODE:
    pp_free_tree(tree->l1.tree);
    free(tree);
    break;
  case PPIDENT:
    free(tree->l2.str);
    free(tree);
    break;
  case PPUNDEF:
  case PPFALSE:
  case PPNULL:
  case PPTRUE:
  case PPARGC:
  case PPARGV:
  case PPRETC:
  case PPRETV:
  case PPGLOBALS:
  case PPLOCALS:
  case PPBREAK:
  case PPCONTINUE:
    free(tree);
    break;
  case PPEXPRLIST:
    if (tree->l1.i!=0) pp_free_tree(tree->l1.tree);
    pp_free_tree(tree->l2.tree);
    free(tree);
    break;
  case PPASSLIST:
    pp_free_tree(tree->l1.tree);
    pp_free_tree(tree->l2.tree->l1.tree);
    pp_free_tree(tree->l2.tree->l2.tree);
    free(tree);
    break;
  case PPASSPAIR:
    pp_free_tree(tree->l1.tree);
    pp_free_tree(tree->l2.tree);
    free(tree);
    break;
  case PPIF:
  case PPUNLESS:
  case PPCOND:
    pp_free_tree(tree->l1.tree);
    pp_free_tree(tree->l2.tree);
    free(tree);
    break;
  case PPNONE:
    free(tree);
    break;
  case PPWHILE:
  case PPUNTIL:
  case PPDOW:
  case PPDOU:
  case PPFOR:
  case PPFPAIR:
  case PPASSIGN:
    pp_free_tree(tree->l1.tree);
    pp_free_tree(tree->l2.tree);
    free(tree);
    break;
  case PPIPAIR:
    free(tree->l1.str);
    free(tree->l2.str);
    free(tree);
    break;
  case PPRETURN:
  case PPRETURNVC:
  case PPCALLVC:
  case PPEXIT:
  case PPEXITVC:
  case PPDO:
    if (tree->l1.tree) pp_free_tree(tree->l1.tree);
    free(tree);
    break;
  }
}

void pp_encode_int(long i)
{
  long c;
  int bytes = 0;
  if (i<0) i = -i;
  for (c=i; c; c = c>>8) bytes++;
  fputc(bytes,ppbcout);
  for (; bytes; bytes--) {
    fputc((i>>(8*(bytes-1)))&255,ppbcout);
  }
  return;
}

void pp_encode_float(double f)
{
  unsigned long i;
  int exp = 0;
  if (f<0) f = -f;
  if (f!=0.0) {
    if ((f>=400000000.0)&&(f<=4000000000.0)) ;
    else if (f<400000000.0) {
      while (f<400000000.0) { 
	exp --;
	f *= 10.0;
      }
    } else {
      while (f>4000000000.0) {
	exp ++;
	f /= 10.0;
      }
    }
  }
  i = (unsigned long)f;
  fputc((i>>24)&255,ppbcout);
  fputc((i>>16)&255,ppbcout);
  fputc((i>>8)&255,ppbcout);
  fputc(i&255,ppbcout);
  fputc((signed char)exp,ppbcout);
  return;
}

void yyerror(char *txt)
{
  fprintf(stderr,"%s\n",txt);
  fprintf(stderr,"parse error near line no. %d\n",yylineno);
  exit(1);
}
