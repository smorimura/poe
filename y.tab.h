/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */

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

#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PUNDEF = 258,
     PFALSE = 259,
     PNULL = 260,
     PTRUE = 261,
     PARGC = 262,
     PARGV = 263,
     PRETC = 264,
     PRETV = 265,
     PAND = 266,
     POR = 267,
     PNOT = 268,
     PGLOBAL = 269,
     PGLOBALS = 270,
     PLOCAL = 271,
     PLOCALS = 272,
     PEXTERN = 273,
     PNORMAL = 274,
     PFUNC = 275,
     PCALLVC = 276,
     PRETURNVC = 277,
     PEXIT = 278,
     PEXITVC = 279,
     PRETURN = 280,
     PWHILE = 281,
     PUNLESS = 282,
     PFOR = 283,
     PIF = 284,
     PELSE = 285,
     PDO = 286,
     PUNTIL = 287,
     PEND = 288,
     PIN = 289,
     PBLOCK = 290,
     PEOF = 291,
     PERROR = 292,
     PSUCCESS = 293,
     PBREAK = 294,
     PCONTINUE = 295,
     PSTR = 296,
     PIDENTIFIER = 297,
     PINT = 298,
     PFLT = 299,
     PCHR = 300,
     PNEQ = 301,
     PEQ = 302,
     PGEQ = 303,
     PLEQ = 304,
     PSHR = 305,
     PSHL = 306,
     PUNMINUS = 307,
     PSUP = 308,
     PMET = 309
   };
#endif
/* Tokens.  */
#define PUNDEF 258
#define PFALSE 259
#define PNULL 260
#define PTRUE 261
#define PARGC 262
#define PARGV 263
#define PRETC 264
#define PRETV 265
#define PAND 266
#define POR 267
#define PNOT 268
#define PGLOBAL 269
#define PGLOBALS 270
#define PLOCAL 271
#define PLOCALS 272
#define PEXTERN 273
#define PNORMAL 274
#define PFUNC 275
#define PCALLVC 276
#define PRETURNVC 277
#define PEXIT 278
#define PEXITVC 279
#define PRETURN 280
#define PWHILE 281
#define PUNLESS 282
#define PFOR 283
#define PIF 284
#define PELSE 285
#define PDO 286
#define PUNTIL 287
#define PEND 288
#define PIN 289
#define PBLOCK 290
#define PEOF 291
#define PERROR 292
#define PSUCCESS 293
#define PBREAK 294
#define PCONTINUE 295
#define PSTR 296
#define PIDENTIFIER 297
#define PINT 298
#define PFLT 299
#define PCHR 300
#define PNEQ 301
#define PEQ 302
#define PGEQ 303
#define PLEQ 304
#define PSHR 305
#define PSHL 306
#define PUNMINUS 307
#define PSUP 308
#define PMET 309




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE

{
  long intval;
  char chrval;
  double fltval;
  char *strval;
  pptree *pptval;
}
/* Line 1529 of yacc.c.  */

	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

#endif
