/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
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




/* Copy the first part of user declarations.  */


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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE

{
  long intval;
  char chrval;
  double fltval;
  char *strval;
  pptree *pptval;
}
/* Line 193 of yacc.c.  */

	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */


#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  56
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2073

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  79
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  97
/* YYNRULES -- Number of states.  */
#define YYNSTATES  223

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   309

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    63,     2,    62,    55,     2,
      74,    75,    60,    56,    76,    57,    67,    61,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    77,    78,
      46,    48,    47,     2,    64,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    70,     2,    71,    54,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    72,    53,    73,    65,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    49,    50,    51,    52,    58,    59,    66,    68,    69
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,    10,    14,    17,    21,    25,    29,
      33,    37,    41,    45,    49,    53,    57,    61,    65,    69,
      73,    77,    81,    85,    89,    92,    95,    98,   101,   104,
     109,   113,   118,   122,   126,   132,   135,   138,   140,   142,
     144,   146,   148,   150,   152,   154,   157,   160,   163,   165,
     169,   171,   173,   175,   177,   179,   181,   183,   185,   187,
     189,   193,   195,   201,   205,   211,   214,   220,   227,   233,
     240,   247,   254,   261,   268,   279,   284,   287,   289,   291,
     293,   295,   301,   306,   312,   318,   323,   329,   333,   336,
     339,   341,   349,   356,   361,   366,   369,   370
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      94,     0,    -1,    70,    83,    71,    -1,    70,    71,    -1,
      72,    84,    73,    -1,    72,    73,    -1,    82,    12,    82,
      -1,    82,    11,    82,    -1,    82,    46,    82,    -1,    82,
      47,    82,    -1,    82,    52,    82,    -1,    82,    51,    82,
      -1,    82,    50,    82,    -1,    82,    49,    82,    -1,    82,
      53,    82,    -1,    82,    54,    82,    -1,    82,    55,    82,
      -1,    82,    56,    82,    -1,    82,    57,    82,    -1,    82,
      59,    82,    -1,    82,    58,    82,    -1,    82,    60,    82,
      -1,    82,    61,    82,    -1,    82,    62,    82,    -1,    13,
      82,    -1,    63,    82,    -1,    57,    82,    -1,    64,    82,
      -1,    65,    82,    -1,    82,    74,    83,    75,    -1,    82,
      74,    75,    -1,    82,    70,    82,    71,    -1,    82,    67,
      42,    -1,    82,    67,    41,    -1,    82,    67,    74,    82,
      75,    -1,    82,    69,    -1,    82,    68,    -1,    43,    -1,
      45,    -1,    41,    -1,    44,    -1,    80,    -1,    81,    -1,
      91,    -1,    92,    -1,    16,    42,    -1,    14,    42,    -1,
      18,    42,    -1,    42,    -1,    74,    82,    75,    -1,     3,
      -1,     4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,
      -1,     9,    -1,    10,    -1,    15,    -1,    17,    -1,    83,
      76,    82,    -1,    82,    -1,    84,    76,    42,    48,    82,
      -1,    42,    48,    82,    -1,    30,    77,    93,    33,    78,
      -1,    30,    86,    -1,    29,    82,    77,    93,    85,    -1,
      29,    82,    77,    93,    33,    78,    -1,    27,    82,    77,
      93,    85,    -1,    27,    82,    77,    93,    33,    78,    -1,
      26,    82,    77,    93,    33,    78,    -1,    32,    82,    77,
      93,    33,    78,    -1,    31,    77,    93,    26,    82,    78,
      -1,    31,    77,    93,    32,    82,    78,    -1,    28,    42,
      76,    42,    34,    82,    77,    93,    33,    78,    -1,    83,
      48,    82,    78,    -1,    82,    78,    -1,    86,    -1,    87,
      -1,    88,    -1,    89,    -1,    25,    74,    83,    75,    78,
      -1,    25,    74,    75,    78,    -1,    22,    74,    83,    75,
      78,    -1,    23,    74,    83,    75,    78,    -1,    23,    74,
      75,    78,    -1,    24,    74,    83,    75,    78,    -1,    31,
      82,    78,    -1,    39,    78,    -1,    40,    78,    -1,    78,
      -1,    20,    74,    83,    75,    77,    93,    33,    -1,    20,
      74,    75,    77,    93,    33,    -1,    21,    74,    83,    75,
      -1,    35,    77,    93,    33,    -1,    93,    90,    -1,    -1,
      93,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   120,   120,   121,   124,   125,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   151,   152,   153,
     155,   157,   159,   163,   168,   170,   172,   174,   176,   178,
     180,   182,   183,   184,   185,   186,   188,   190,   192,   194,
     195,   197,   199,   201,   203,   205,   207,   209,   211,   213,
     218,   220,   225,   233,   241,   242,   254,   258,   264,   268,
     278,   280,   285,   287,   292,   299,   300,   301,   302,   303,
     304,   305,   307,   309,   311,   313,   315,   317,   319,   321,
     323,   328,   330,   332,   337,   341,   342,   345
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PUNDEF", "PFALSE", "PNULL", "PTRUE",
  "PARGC", "PARGV", "PRETC", "PRETV", "PAND", "POR", "PNOT", "PGLOBAL",
  "PGLOBALS", "PLOCAL", "PLOCALS", "PEXTERN", "PNORMAL", "PFUNC",
  "PCALLVC", "PRETURNVC", "PEXIT", "PEXITVC", "PRETURN", "PWHILE",
  "PUNLESS", "PFOR", "PIF", "PELSE", "PDO", "PUNTIL", "PEND", "PIN",
  "PBLOCK", "PEOF", "PERROR", "PSUCCESS", "PBREAK", "PCONTINUE", "PSTR",
  "PIDENTIFIER", "PINT", "PFLT", "PCHR", "'<'", "'>'", "'='", "PNEQ",
  "PEQ", "PGEQ", "PLEQ", "'|'", "'^'", "'&'", "'+'", "'-'", "PSHR", "PSHL",
  "'*'", "'/'", "'%'", "'#'", "'@'", "'~'", "PUNMINUS", "'.'", "PSUP",
  "PMET", "'['", "']'", "'{'", "'}'", "'('", "')'", "','", "':'", "';'",
  "$accept", "array", "table", "expr", "expr_list", "ass_list", "else_stm",
  "if_stm", "loop_stm", "do_stm", "for_stm", "statement", "function",
  "code", "block", "script", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,    60,    62,    61,   301,
     302,   303,   304,   124,    94,    38,    43,    45,   305,   306,
      42,    47,    37,    35,    64,   126,   307,    46,   308,   309,
      91,    93,   123,   125,    40,    41,    44,    58,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    79,    80,    80,    81,    81,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      83,    83,    84,    84,    85,    85,    86,    86,    86,    86,
      87,    87,    88,    88,    89,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    91,    91,    91,    92,    93,    93,    94
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     2,     3,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     4,
       3,     4,     3,     3,     5,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     5,     3,     5,     2,     5,     6,     5,     6,
       6,     6,     6,     6,    10,     4,     2,     1,     1,     1,
       1,     5,     4,     5,     5,     4,     5,     3,     2,     2,
       1,     7,     6,     4,     4,     2,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      96,    97,     0,    50,    51,    52,    53,    54,    55,    56,
      57,     0,     0,    58,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    39,    48,    37,    40,    38,     0,     0,     0,
       0,     0,     0,     0,    90,    41,    42,    61,     0,    77,
      78,    79,    80,    95,    43,    44,     1,    24,    46,    45,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    96,    88,    89,    26,    25,    27,
      28,     3,    61,     0,     0,     5,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,    35,     0,
       0,    76,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,    96,     0,    96,     0,    87,    96,
       0,     2,     0,     4,     0,    49,     7,     6,     8,     9,
      13,    12,    11,    10,    14,    15,    16,    17,    18,    20,
      19,    21,    22,    23,    33,    32,     0,     0,    30,     0,
       0,    60,    96,     0,    93,     0,    85,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    63,
       0,     0,    31,    29,    75,     0,    96,    83,    84,    86,
      81,     0,     0,     0,    68,     0,     0,    66,     0,     0,
       0,     0,    34,    92,     0,    70,    96,    65,    69,     0,
      67,    72,    73,    71,    62,    91,     0,    96,     0,     0,
      64,     0,    74
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    45,    46,    47,    48,    86,   194,    49,    50,    51,
      52,    53,    54,    55,     1,     2
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -68
static const yytype_int16 yypact[] =
{
     -68,   872,     9,   -68,   -68,   -68,   -68,   -68,   -68,   -68,
     -68,  1463,   -28,   -68,   -18,   -68,    -9,    -5,    -4,    -3,
      -2,    -1,     0,  1463,  1463,    33,  1463,  1024,  1463,    19,
     -41,    22,   -68,   -68,   -68,   -68,   -68,  1463,  1463,  1463,
    1463,  1391,   -37,  1463,   -68,   -68,   -68,  1583,   -45,   -68,
     -68,   -68,   -68,   -68,   -68,   -68,   -68,   -49,   -68,   -68,
     -68,  1099,  1463,  1463,  1172,  1463,  1245,  1673,  1725,    21,
    1754,   -68,  1613,  1786,   -68,   -68,   -68,   -49,   -49,   -49,
     -49,   -68,  1944,   -65,    28,   -68,   -38,  1847,  1463,  1463,
    1463,  1463,  1463,  1463,  1463,  1463,  1463,  1463,  1463,  1463,
    1463,  1463,  1463,  1463,  1463,  1463,   -40,   -68,   -68,  1463,
    1318,   -68,  1463,  1463,    34,   -53,   -36,   -34,    35,   -31,
     -29,    36,   -27,   -68,   -68,    78,   -68,   948,   -68,   -68,
     340,   -68,  1463,   -68,    81,   -68,  1999,  1973,    73,    73,
      73,    73,    73,    73,    99,    48,   180,   256,   256,   326,
     326,   -49,   -49,   -49,   -68,   -68,  1463,  1912,   -68,   -15,
    1643,  1944,   -68,    47,   -68,    58,   -68,    59,    60,   -68,
      66,   416,   188,    91,   264,  1463,  1463,   492,   -68,  1944,
      98,  1879,   -68,   -68,   -68,   568,   -68,   -68,   -68,   -68,
     -68,    70,   -19,    71,   -68,  1463,    74,   -68,  1498,  1531,
      84,  1463,   -68,   -68,   644,   -68,   -68,   -68,   -68,  1815,
     -68,   -68,   -68,   -68,  1944,   -68,   720,   -68,    85,   796,
     -68,    92,   -68
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -68,   -68,   -68,   -11,     2,   -68,   -23,   -21,   -68,   -68,
     -68,   -68,   -68,   -68,   -67,   -68
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      57,   154,   155,   112,   127,    84,   131,   130,    24,    56,
      26,   113,    67,    68,    58,    70,    72,    73,   106,   107,
     108,   109,   163,   113,    59,   110,    77,    78,    79,    80,
      82,   113,    87,    60,   156,   133,    85,    75,   134,   164,
     113,   165,   113,    83,   167,   113,   168,   113,   170,   113,
      82,    82,    82,    82,    82,    82,   171,   172,   206,   174,
     183,   113,   177,   115,   116,   117,   119,   120,   122,    61,
      62,    63,    64,    65,    66,    69,   132,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   185,    74,   125,   157,    82,
      76,   160,   161,    98,    99,   100,   101,   102,   103,   104,
     105,   162,   159,   166,   169,   106,   107,   108,   109,   204,
     173,   179,   110,   180,   186,   195,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   187,   188,   189,   216,
     106,   107,   108,   109,   190,   181,   201,   110,   205,   208,
     219,   197,   210,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   213,   220,   198,   199,   106,   107,   108,   109,
     222,   207,     0,   110,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   209,     0,     0,     0,     0,     0,
     214,     3,     4,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,    14,    15,    16,     0,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   192,    27,
      28,   193,     0,    29,     0,     0,     0,    30,    31,    32,
      33,    34,    35,    36,     0,     0,    99,   100,   101,   102,
     103,   104,   105,     0,     0,    37,     0,   106,   107,   108,
     109,    38,    39,    40,   110,     0,     0,     0,    41,     0,
      42,     0,    43,     0,     0,     0,    44,     3,     4,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
      14,    15,    16,     0,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   192,    27,    28,   196,     0,    29,
       0,     0,     0,    30,    31,    32,    33,    34,    35,    36,
       0,     0,     0,     0,   101,   102,   103,   104,   105,     0,
       0,    37,     0,   106,   107,   108,   109,    38,    39,    40,
     110,     0,     0,     0,    41,     0,    42,     0,    43,     0,
       0,     0,    44,     3,     4,     5,     6,     7,     8,     9,
      10,     0,     0,    11,    12,    13,    14,    15,    16,     0,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,   178,     0,    29,     0,     0,     0,    30,
      31,    32,    33,    34,    35,    36,   103,   104,   105,     0,
       0,     0,     0,   106,   107,   108,   109,    37,     0,     0,
     110,     0,     0,    38,    39,    40,     0,     0,     0,     0,
      41,     0,    42,     0,    43,     0,     0,     0,    44,     3,
       4,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,    14,    15,    16,     0,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,   191,
       0,    29,     0,     0,     0,    30,    31,    32,    33,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,     0,     0,     0,     0,    38,
      39,    40,     0,     0,     0,     0,    41,     0,    42,     0,
      43,     0,     0,     0,    44,     3,     4,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,    14,    15,
      16,     0,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,   200,     0,    29,     0,     0,
       0,    30,    31,    32,    33,    34,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,    38,    39,    40,     0,     0,
       0,     0,    41,     0,    42,     0,    43,     0,     0,     0,
      44,     3,     4,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,    14,    15,    16,     0,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,   203,     0,    29,     0,     0,     0,    30,    31,    32,
      33,    34,    35,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,    38,    39,    40,     0,     0,     0,     0,    41,     0,
      42,     0,    43,     0,     0,     0,    44,     3,     4,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
      14,    15,    16,     0,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,   215,     0,    29,
       0,     0,     0,    30,    31,    32,    33,    34,    35,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,    38,    39,    40,
       0,     0,     0,     0,    41,     0,    42,     0,    43,     0,
       0,     0,    44,     3,     4,     5,     6,     7,     8,     9,
      10,     0,     0,    11,    12,    13,    14,    15,    16,     0,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,   218,     0,    29,     0,     0,     0,    30,
      31,    32,    33,    34,    35,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,    38,    39,    40,     0,     0,     0,     0,
      41,     0,    42,     0,    43,     0,     0,     0,    44,     3,
       4,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,    14,    15,    16,     0,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,   221,
       0,    29,     0,     0,     0,    30,    31,    32,    33,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,     0,     0,     0,     0,    38,
      39,    40,     0,     0,     0,     0,    41,     0,    42,     0,
      43,     0,     0,     0,    44,     3,     4,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,    14,    15,
      16,     0,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,     0,    29,     0,     0,
       0,    30,    31,    32,    33,    34,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,    38,    39,    40,     0,     0,
       0,     0,    41,     0,    42,     0,    43,     0,     0,     0,
      44,     3,     4,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,    14,    15,    16,     0,    17,    18,
      19,    20,    21,    22,   175,    24,    25,    26,     0,    27,
     176,     0,     0,    29,     0,     0,     0,    30,    31,    32,
      33,    34,    35,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,    38,    39,    40,     0,     0,     0,     0,    41,     0,
      42,     0,    43,     0,     0,     0,    44,     3,     4,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
      14,    15,    16,     0,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,     0,     0,     0,     0,    32,    33,    34,    35,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,    38,    39,    40,
       0,     0,     0,     0,    41,     0,    42,     0,    43,     0,
       0,    71,     3,     4,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,    14,    15,    16,     0,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,     0,     0,     0,     0,
      32,    33,    34,    35,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,     0,     0,     0,
       0,     0,    38,    39,    40,     0,     0,     0,     0,    41,
       0,    42,     0,    43,   114,     3,     4,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,    14,    15,
      16,     0,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,     0,
       0,     0,     0,    32,    33,    34,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,    38,    39,    40,     0,     0,
       0,     0,    41,     0,    42,     0,    43,   118,     3,     4,
       5,     6,     7,     8,     9,    10,     0,     0,    11,    12,
      13,    14,    15,    16,     0,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,     0,     0,     0,     0,    32,    33,    34,    35,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,     0,     0,    38,    39,
      40,     0,     0,     0,     0,    41,     0,    42,     0,    43,
     121,     3,     4,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,    14,    15,    16,     0,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,     0,     0,     0,     0,    32,
      33,    34,    35,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,    38,    39,    40,     0,     0,     0,     0,    41,     0,
      42,     0,    43,   158,     3,     4,     5,     6,     7,     8,
       9,    10,     0,     0,    11,    12,    13,    14,    15,    16,
       0,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,     0,     0,
       0,     0,    32,    33,    34,    35,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,    38,    39,    40,     0,     0,     0,
       0,    41,    81,    42,     0,    43,     3,     4,     5,     6,
       7,     8,     9,    10,     0,     0,    11,    12,    13,    14,
      15,    16,     0,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
       0,     0,     0,     0,    32,    33,    34,    35,    36,    88,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,    38,    39,    40,     0,
       0,     0,     0,    41,     0,    42,     0,    43,     0,     0,
       0,     0,    88,    89,    90,    91,     0,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,     0,     0,     0,     0,   106,   107,   108,   109,     0,
       0,     0,   110,     0,     0,   123,   211,    90,    91,     0,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,    88,    89,     0,     0,   106,   107,
     108,   109,     0,     0,     0,   110,     0,     0,   129,   212,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,     0,     0,     0,    90,
      91,     0,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,     0,     0,     0,     0,
     106,   107,   108,   109,    88,    89,     0,   110,     0,    90,
      91,   111,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,     0,     0,     0,     0,
     106,   107,   108,   109,    88,    89,     0,   110,     0,    90,
      91,   128,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,     0,     0,     0,     0,
     106,   107,   108,   109,     0,     0,     0,   110,     0,    90,
      91,   184,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    88,    89,     0,     0,
     106,   107,   108,   109,     0,     0,     0,   110,     0,     0,
     123,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,     0,     0,     0,
       0,    90,    91,     0,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,     0,     0,
       0,     0,   106,   107,   108,   109,     0,    88,    89,   110,
      90,    91,   124,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,     0,     0,     0,
       0,   106,   107,   108,   109,     0,    88,    89,   110,     0,
       0,   126,    90,    91,     0,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,     0,
       0,     0,     0,   106,   107,   108,   109,     0,    88,    89,
     110,    90,    91,   129,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,     0,     0,
       0,     0,   106,   107,   108,   109,     0,     0,     0,   110,
      88,    89,   217,    90,    91,     0,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
       0,     0,     0,     0,   106,   107,   108,   109,     0,     0,
       0,   110,   135,    88,    89,    90,    91,     0,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,     0,     0,     0,     0,   106,   107,   108,   109,
       0,     0,     0,   110,   202,    88,    89,     0,    90,    91,
       0,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,     0,     0,     0,     0,   106,
     107,   108,   109,   182,    88,     0,   110,     0,     0,     0,
      90,    91,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,     0,     0,     0,
       0,   106,   107,   108,   109,     0,     0,     0,   110,    90,
      91,     0,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,     0,     0,     0,     0,
     106,   107,   108,   109,     0,    90,    91,   110,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,     0,     0,     0,     0,   106,   107,   108,   109,
       0,     0,     0,   110
};

static const yytype_int16 yycheck[] =
{
      11,    41,    42,    48,    71,    42,    71,    74,    27,     0,
      29,    76,    23,    24,    42,    26,    27,    28,    67,    68,
      69,    70,    75,    76,    42,    74,    37,    38,    39,    40,
      41,    76,    43,    42,    74,    73,    73,    78,    76,    75,
      76,    75,    76,    41,    75,    76,    75,    76,    75,    76,
      61,    62,    63,    64,    65,    66,   123,   124,    77,   126,
      75,    76,   129,    61,    62,    63,    64,    65,    66,    74,
      74,    74,    74,    74,    74,    42,    48,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   162,    77,    76,   109,   110,
      78,   112,   113,    55,    56,    57,    58,    59,    60,    61,
      62,    77,   110,    78,    78,    67,    68,    69,    70,   186,
      42,   132,    74,    42,    77,    34,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    78,    78,    78,   206,
      67,    68,    69,    70,    78,   156,    48,    74,    78,    78,
     217,   174,    78,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    78,    78,   175,   176,    67,    68,    69,    70,
      78,   192,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,    -1,    -1,    -1,    -1,    -1,
     201,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    13,    14,    15,    16,    17,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    43,    44,    45,    -1,    -1,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    57,    -1,    67,    68,    69,
      70,    63,    64,    65,    74,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    -1,    -1,    -1,    78,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    13,    14,    15,
      16,    17,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    42,    43,    44,    45,
      -1,    -1,    -1,    -1,    58,    59,    60,    61,    62,    -1,
      -1,    57,    -1,    67,    68,    69,    70,    63,    64,    65,
      74,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    78,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    13,    14,    15,    16,    17,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    42,    43,    44,    45,    60,    61,    62,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    57,    -1,    -1,
      74,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    72,    -1,    74,    -1,    -1,    -1,    78,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    13,
      14,    15,    16,    17,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,    43,
      44,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,
      74,    -1,    -1,    -1,    78,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    13,    14,    15,    16,    17,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    42,    43,    44,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    -1,    -1,
      -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,    -1,
      78,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    13,    14,    15,    16,    17,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    43,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    -1,    -1,    -1,    78,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    13,    14,    15,
      16,    17,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    42,    43,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    -1,    78,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    13,    14,    15,    16,    17,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    42,    43,    44,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    72,    -1,    74,    -1,    -1,    -1,    78,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    13,
      14,    15,    16,    17,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,    43,
      44,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,
      74,    -1,    -1,    -1,    78,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    13,    14,    15,    16,    17,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    -1,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    42,    43,    44,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    -1,    -1,
      -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,    -1,
      78,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    13,    14,    15,    16,    17,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    -1,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    43,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    -1,    -1,    -1,    78,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    13,    14,    15,
      16,    17,    18,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      -1,    77,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    13,    14,    15,    16,    17,    18,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      41,    42,    43,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    75,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    13,    14,    15,    16,    17,
      18,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    41,    42,    43,    44,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    -1,    -1,
      -1,    -1,    70,    -1,    72,    -1,    74,    75,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    13,    14,
      15,    16,    17,    18,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    41,    42,    43,    44,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,
      75,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    13,    14,    15,    16,    17,    18,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,
      42,    43,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,
      72,    -1,    74,    75,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    13,    14,    15,    16,    17,    18,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    41,    42,    43,    44,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,
      -1,    70,    71,    72,    -1,    74,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    13,    14,    15,    16,
      17,    18,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,    11,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,    -1,
      -1,    -1,    11,    12,    46,    47,    -1,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    74,    -1,    -1,    77,    78,    46,    47,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    11,    12,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    74,    -1,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,    46,
      47,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    11,    12,    -1,    74,    -1,    46,
      47,    78,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    11,    12,    -1,    74,    -1,    46,
      47,    78,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    74,    -1,    46,
      47,    78,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    11,    12,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    74,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,
      -1,    46,    47,    -1,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    11,    12,    74,
      46,    47,    77,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    11,    12,    74,    -1,
      -1,    77,    46,    47,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    11,    12,
      74,    46,    47,    77,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    74,
      11,    12,    77,    46,    47,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    74,    75,    11,    12,    46,    47,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    74,    75,    11,    12,    -1,    46,    47,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    11,    -1,    74,    -1,    -1,    -1,
      46,    47,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    74,    46,
      47,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    46,    47,    74,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    74
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    93,    94,     3,     4,     5,     6,     7,     8,     9,
      10,    13,    14,    15,    16,    17,    18,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    31,    32,    35,
      39,    40,    41,    42,    43,    44,    45,    57,    63,    64,
      65,    70,    72,    74,    78,    80,    81,    82,    83,    86,
      87,    88,    89,    90,    91,    92,     0,    82,    42,    42,
      42,    74,    74,    74,    74,    74,    74,    82,    82,    42,
      82,    77,    82,    82,    77,    78,    78,    82,    82,    82,
      82,    71,    82,    83,    42,    73,    84,    82,    11,    12,
      46,    47,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    67,    68,    69,    70,
      74,    78,    48,    76,    75,    83,    83,    83,    75,    83,
      83,    75,    83,    77,    77,    76,    77,    93,    78,    77,
      93,    71,    48,    73,    76,    75,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    41,    42,    74,    82,    75,    83,
      82,    82,    77,    75,    75,    75,    78,    75,    75,    78,
      75,    93,    93,    42,    93,    26,    32,    93,    33,    82,
      42,    82,    71,    75,    78,    93,    77,    78,    78,    78,
      78,    33,    30,    33,    85,    34,    33,    85,    82,    82,
      33,    48,    75,    33,    93,    78,    77,    86,    78,    82,
      78,    78,    78,    78,    82,    33,    93,    77,    33,    93,
      78,    33,    78
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

    { (yyval.pptval) = new_pptree(PPARR, (treedata)(yyvsp[(2) - (3)].pptval), (treedata)0L); }
    break;

  case 3:

    { (yyval.pptval) = new_pptree(PPARR, (treedata)0L, (treedata)0L); }
    break;

  case 4:

    { (yyval.pptval) = new_pptree(PPTAB, (treedata)(yyvsp[(2) - (3)].pptval), (treedata)0L); }
    break;

  case 5:

    { (yyval.pptval) = new_pptree(PPTAB, (treedata)0L, (treedata)0L); }
    break;

  case 6:

    { (yyval.pptval) = new_pptree(PPOR, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 7:

    { (yyval.pptval) = new_pptree(PPAND, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 8:

    { (yyval.pptval) = new_pptree(PPLT, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 9:

    { (yyval.pptval) = new_pptree(PPGT, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 10:

    { (yyval.pptval) = new_pptree(PPLEQ, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 11:

    { (yyval.pptval) = new_pptree(PPGEQ, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 12:

    { (yyval.pptval) = new_pptree(PPEQ, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 13:

    { (yyval.pptval) = new_pptree(PPNEQ, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 14:

    { (yyval.pptval) = new_pptree(PPBOR, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 15:

    { (yyval.pptval) = new_pptree(PPBXOR, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 16:

    { (yyval.pptval) = new_pptree(PPBAND, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 17:

    { (yyval.pptval) = new_pptree(PPPLUS, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 18:

    { (yyval.pptval) = new_pptree(PPMINUS, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 19:

    { (yyval.pptval) = new_pptree(PPSHL, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 20:

    { (yyval.pptval) = new_pptree(PPSHR, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 21:

    { (yyval.pptval) = new_pptree(PPMULT, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 22:

    { (yyval.pptval) = new_pptree(PPDIV, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 23:

    { (yyval.pptval) = new_pptree(PPMOD, (treedata)(yyvsp[(1) - (3)].pptval), (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 24:

    { (yyval.pptval) = new_pptree(PPNOT, (treedata)(yyvsp[(2) - (2)].pptval), (treedata)0L); }
    break;

  case 25:

    { (yyval.pptval) = new_pptree(PPLEN, (treedata)(yyvsp[(2) - (2)].pptval), (treedata)0L); }
    break;

  case 26:

    {
  (yyval.pptval) = new_pptree(PPUNM, (treedata)(yyvsp[(2) - (2)].pptval), (treedata)0L); }
    break;

  case 27:

    { (yyval.pptval) = new_pptree(PPMAX, (treedata)(yyvsp[(2) - (2)].pptval), (treedata)0L); }
    break;

  case 28:

    { (yyval.pptval) = new_pptree(PPBNOT, (treedata)(yyvsp[(2) - (2)].pptval), (treedata)0L); }
    break;

  case 29:

    { (yyval.pptval) = new_pptree(PPCALL, (treedata)(yyvsp[(1) - (4)].pptval), 
					     (treedata)(yyvsp[(3) - (4)].pptval)); }
    break;

  case 30:

    { (yyval.pptval) = new_pptree(PPCALL, (treedata)(yyvsp[(1) - (3)].pptval),
					     (treedata)0L); }
    break;

  case 31:

    { (yyval.pptval) = new_pptree(PPARRACC, (treedata)(yyvsp[(1) - (4)].pptval), 
					     (treedata)(yyvsp[(3) - (4)].pptval)); }
    break;

  case 32:

    { (yyval.pptval) = new_pptree(PPTABACC, (treedata)(yyvsp[(1) - (3)].pptval), 
		  (treedata)new_pptree(PPIDENT, (treedata)(long)PNORMAL,
				       (treedata)(yyvsp[(3) - (3)].strval))); }
    break;

  case 33:

    { (yyval.pptval) = new_pptree(PPTABACC, (treedata)(yyvsp[(1) - (3)].pptval),
		  (treedata)new_pptree(PPSTRLIT,
				       (treedata)(yyvsp[(3) - (3)].strval),
				       (treedata)0L)); }
    break;

  case 34:

    { (yyval.pptval) = new_pptree(PPLTABACC, (treedata)(yyvsp[(1) - (5)].pptval), 
					     (treedata)(yyvsp[(4) - (5)].pptval)); }
    break;

  case 35:

    { (yyval.pptval) = new_pptree(PPMETA, (treedata)(yyvsp[(1) - (2)].pptval), 
					     (treedata)0L); }
    break;

  case 36:

    { (yyval.pptval) = new_pptree(PPSUP, (treedata)(yyvsp[(1) - (2)].pptval), 
					     (treedata)0L); }
    break;

  case 37:

    { (yyval.pptval) = new_pptree(PPINTLIT, (treedata)(yyvsp[(1) - (1)].intval), 
					     (treedata)0L); }
    break;

  case 38:

    { (yyval.pptval) = new_pptree(PPCHRLIT, (treedata)(yyvsp[(1) - (1)].chrval), 
					     (treedata)0L); }
    break;

  case 39:

    { (yyval.pptval) = new_pptree(PPSTRLIT, (treedata)(yyvsp[(1) - (1)].strval), 
					     (treedata)0L); }
    break;

  case 40:

    { (yyval.pptval) = new_pptree(PPFLTLIT, (treedata)(yyvsp[(1) - (1)].fltval),
					     (treedata)0L); }
    break;

  case 41:

    { (yyval.pptval) = (yyvsp[(1) - (1)].pptval); }
    break;

  case 42:

    { (yyval.pptval) = (yyvsp[(1) - (1)].pptval); }
    break;

  case 43:

    { (yyval.pptval) = (yyvsp[(1) - (1)].pptval); }
    break;

  case 44:

    { (yyval.pptval) = (yyvsp[(1) - (1)].pptval); }
    break;

  case 45:

    { (yyval.pptval) = new_pptree(PPIDENT, (treedata)(long)LOCAL, 
					     (treedata)(yyvsp[(2) - (2)].strval)); }
    break;

  case 46:

    { (yyval.pptval) = new_pptree(PPIDENT, (treedata)(long)GLOBAL, 
					     (treedata)(yyvsp[(2) - (2)].strval)); }
    break;

  case 47:

    { (yyval.pptval) = new_pptree(PPIDENT, (treedata)(long)EXTERN, 
					     (treedata)(yyvsp[(2) - (2)].strval)); }
    break;

  case 48:

    { (yyval.pptval) = new_pptree(PPIDENT, (treedata)(long)NORMAL, 
					     (treedata)(yyvsp[(1) - (1)].strval)); }
    break;

  case 49:

    { (yyval.pptval) = (yyvsp[(2) - (3)].pptval); }
    break;

  case 50:

    { (yyval.pptval) = new_pptree(PPUNDEF, (treedata)0L, 
					     (treedata)0L); }
    break;

  case 51:

    { (yyval.pptval) = new_pptree(PPFALSE, (treedata)0L, 
					     (treedata)0L); }
    break;

  case 52:

    { (yyval.pptval) = new_pptree(PPNULL, (treedata)0L, 
					     (treedata)0L); }
    break;

  case 53:

    { (yyval.pptval) = new_pptree(PPTRUE, (treedata)0L, 
					     (treedata)0L); }
    break;

  case 54:

    { (yyval.pptval) = new_pptree(PPARGC, (treedata)0L, 
					     (treedata)0L); }
    break;

  case 55:

    { (yyval.pptval) = new_pptree(PPARGV, (treedata)0L, 
					     (treedata)0L); }
    break;

  case 56:

    { (yyval.pptval) = new_pptree(PPRETC, (treedata)0L, 
					     (treedata)0L); }
    break;

  case 57:

    { (yyval.pptval) = new_pptree(PPRETV, (treedata)0L, 
					     (treedata)0L); }
    break;

  case 58:

    { (yyval.pptval) = new_pptree(PPGLOBALS, (treedata)0L, 
					     (treedata)0L); }
    break;

  case 59:

    { (yyval.pptval) = new_pptree(PPLOCALS, (treedata)0L, 
					     (treedata)0L); }
    break;

  case 60:

    { (yyval.pptval) = new_pptree(PPEXPRLIST, (treedata)(yyvsp[(1) - (3)].pptval), 
					     (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 61:

    { (yyval.pptval) = new_pptree(PPEXPRLIST, (treedata)0L, 
					     (treedata)(yyvsp[(1) - (1)].pptval)); }
    break;

  case 62:

    { (yyval.pptval) = new_pptree(PPASSLIST, (treedata)(yyvsp[(1) - (5)].pptval),
		  (treedata)new_pptree(PPASSPAIR,
				       (treedata)
				       new_pptree(PPIDENT,
						  (treedata)(long)PNORMAL,
						  (treedata)(yyvsp[(3) - (5)].strval)),
				       (treedata)(yyvsp[(5) - (5)].pptval))); }
    break;

  case 63:

    { (yyval.pptval) = new_pptree(PPASSPAIR,
					   (treedata)new_pptree(PPIDENT,
						      (treedata)(long)PNORMAL,
								(treedata)(yyvsp[(1) - (3)].strval)),
					   (treedata)(yyvsp[(3) - (3)].pptval)); }
    break;

  case 64:

    { (yyval.pptval) = (yyvsp[(3) - (5)].pptval);}
    break;

  case 65:

    { (yyval.pptval) = (yyvsp[(2) - (2)].pptval); /* new_pptree(PPIF,
		  (treedata)$3,
		  (treedata)new_pptree(PPCOND,(treedata)$5,(treedata)$6)); */ }
    break;

  case 66:

    { (yyval.pptval) = new_pptree(PPIF,
		  (treedata)(yyvsp[(2) - (5)].pptval),
		  (treedata)new_pptree(PPCOND,(treedata)(yyvsp[(4) - (5)].pptval),(treedata)(yyvsp[(5) - (5)].pptval))); }
    break;

  case 67:

    { (yyval.pptval) = new_pptree(PPIF,
		  (treedata)(yyvsp[(2) - (6)].pptval),
		  (treedata)new_pptree(PPCOND,(treedata)(yyvsp[(4) - (6)].pptval),
				       (treedata)new_pptree(PPNONE,(treedata)0L,
							    (treedata)0L))); }
    break;

  case 68:

    { (yyval.pptval) = new_pptree(PPUNLESS,
		  (treedata)(yyvsp[(2) - (5)].pptval),
		  (treedata)new_pptree(PPCOND,(treedata)(yyvsp[(4) - (5)].pptval),(treedata)(yyvsp[(5) - (5)].pptval))); }
    break;

  case 69:

    { (yyval.pptval) = new_pptree(PPUNLESS,
		    (treedata)(yyvsp[(2) - (6)].pptval),
		    (treedata)new_pptree(PPCOND,(treedata)(yyvsp[(4) - (6)].pptval),
					 (treedata)new_pptree(PPNONE,
							      (treedata)0L,
							      (treedata)0L))); }
    break;

  case 70:

    { (yyval.pptval) = new_pptree(PPWHILE,(treedata)(yyvsp[(2) - (6)].pptval),(treedata)(yyvsp[(4) - (6)].pptval)); }
    break;

  case 71:

    { (yyval.pptval) = new_pptree(PPUNTIL,(treedata)(yyvsp[(2) - (6)].pptval),(treedata)(yyvsp[(4) - (6)].pptval)); }
    break;

  case 72:

    { (yyval.pptval) = new_pptree(PPDOW,(treedata)(yyvsp[(5) - (6)].pptval),(treedata)(yyvsp[(3) - (6)].pptval)); }
    break;

  case 73:

    { (yyval.pptval) = new_pptree(PPDOU,(treedata)(yyvsp[(5) - (6)].pptval),(treedata)(yyvsp[(3) - (6)].pptval)); }
    break;

  case 74:

    { (yyval.pptval) = new_pptree(PPFOR,
		  (treedata)new_pptree(PPIPAIR,(treedata)(yyvsp[(2) - (10)].strval),(treedata)(yyvsp[(4) - (10)].strval)),
		  (treedata)new_pptree(PPFPAIR,(treedata)(yyvsp[(6) - (10)].pptval),(treedata)(yyvsp[(8) - (10)].pptval))); }
    break;

  case 75:

    { (yyval.pptval) = new_pptree(PPASSIGN,(treedata)(yyvsp[(1) - (4)].pptval),(treedata)(yyvsp[(3) - (4)].pptval)); }
    break;

  case 76:

    { (yyval.pptval) = (yyvsp[(1) - (2)].pptval); }
    break;

  case 77:

    { (yyval.pptval) = (yyvsp[(1) - (1)].pptval); }
    break;

  case 78:

    { (yyval.pptval) = (yyvsp[(1) - (1)].pptval); }
    break;

  case 79:

    { (yyval.pptval) = (yyvsp[(1) - (1)].pptval); }
    break;

  case 80:

    { (yyval.pptval) = (yyvsp[(1) - (1)].pptval); }
    break;

  case 81:

    { (yyval.pptval) = new_pptree(PPRETURN,(treedata)(yyvsp[(3) - (5)].pptval),
						 (treedata)0L); }
    break;

  case 82:

    { (yyval.pptval) = new_pptree(PPRETURN,(treedata)0L,
						    (treedata)0L); }
    break;

  case 83:

    { (yyval.pptval) = new_pptree(PPRETURNVC,(treedata)(yyvsp[(3) - (5)].pptval),
						 (treedata)0L); }
    break;

  case 84:

    { (yyval.pptval) = new_pptree(PPEXIT,(treedata)(yyvsp[(3) - (5)].pptval),
						 (treedata)0L); }
    break;

  case 85:

    { (yyval.pptval) = new_pptree(PPEXIT,(treedata)0L,
						    (treedata)0L); }
    break;

  case 86:

    { (yyval.pptval) = new_pptree(PPEXITVC,(treedata)(yyvsp[(3) - (5)].pptval),
						 (treedata)0L); }
    break;

  case 87:

    { (yyval.pptval) = new_pptree(PPDO, (treedata)(yyvsp[(2) - (3)].pptval),
						 (treedata)0L); }
    break;

  case 88:

    { (yyval.pptval) = new_pptree(PPBREAK,(treedata)0L,
						 (treedata)0L); }
    break;

  case 89:

    { (yyval.pptval) = new_pptree(PPCONTINUE,(treedata)0L,
						 (treedata)0L); }
    break;

  case 90:

    { (yyval.pptval) = new_pptree(PPNONE, (treedata)0L,
						 (treedata)0L); }
    break;

  case 91:

    { (yyval.pptval) = new_pptree(PPFUNC,(treedata)(yyvsp[(3) - (7)].pptval),(treedata)(yyvsp[(6) - (7)].pptval)); }
    break;

  case 92:

    { (yyval.pptval) = new_pptree(PPFUNC,(treedata)0L,(treedata)(yyvsp[(5) - (6)].pptval)); }
    break;

  case 93:

    { (yyval.pptval) = new_pptree(PPCALLVC,(treedata)(yyvsp[(3) - (4)].pptval),
						 (treedata)0L); }
    break;

  case 94:

    { (yyval.pptval) = new_pptree(PPCODE,(treedata)(yyvsp[(3) - (4)].pptval),(treedata)0L); }
    break;

  case 95:

    { (yyval.pptval) = new_pptree(PPBLOCK,(treedata)(yyvsp[(1) - (2)].pptval),(treedata)(yyvsp[(2) - (2)].pptval)); }
    break;

  case 96:

    { (yyval.pptval) = new_pptree(PPNONE,(treedata)0L,(treedata)0L); }
    break;

  case 97:

    { pp_write_block((yyvsp[(1) - (1)].pptval)); pp_free_tree((yyvsp[(1) - (1)].pptval)); 
  fputc(CODEEND,ppbcout); YYACCEPT; }
    break;


/* Line 1267 of yacc.c.  */

      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}





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

