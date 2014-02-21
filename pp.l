%{
#include "y.tab.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#define PPARSE_STRMIN 32
char *POE_STRBUF;
int POE_BUFIND;
int POE_BUFSZ;
long pparse_oct(char *);
long pparse_bin(char *);
long pparse_hex(char *);
int pparse_addchar(char);

extern YYSTYPE yylval;
%}

%x BLOCKCOMMENT
%x LINECOMMENT
%x STRING

Dec        [0-9]
Oct        [0-7]
Bin        [0-1]
Hex        [0-9A-Fa-f]
InitCh     [a-zA-Z_!?]
ICh        [a-zA-Z_!?0-9]

%%

"/*"                { BEGIN(BLOCKCOMMENT); }
<BLOCKCOMMENT>"*/"  { BEGIN(INITIAL); }
<BLOCKCOMMENT>.     { }
<BLOCKCOMMENT>\n    { }
"#!"                { BEGIN(LINECOMMENT); }
<LINECOMMENT>\n     { BEGIN(INITIAL); }
<LINECOMMENT>.      { }

\"                  { BEGIN(STRING); 
                      POE_BUFIND = 0;
                      POE_BUFSZ = PPARSE_STRMIN;
                      POE_STRBUF = malloc(PPARSE_STRMIN);
                      if (!POE_STRBUF) return PERROR; }
<STRING>\"          { BEGIN(INITIAL);
                      pparse_addchar('\0');
                      yylval.strval = POE_STRBUF;
                      return PSTR; }
<STRING>\\a      { if (pparse_addchar('\a')==PERROR) return PERROR; }
<STRING>\a       { if (pparse_addchar('\a')==PERROR) return PERROR; }
<STRING>\\b      { if (pparse_addchar('\b')==PERROR) return PERROR; }
<STRING>\b       { if (pparse_addchar('\b')==PERROR) return PERROR; }
<STRING>\\f      { if (pparse_addchar('\f')==PERROR) return PERROR; }
<STRING>\f       { if (pparse_addchar('\f')==PERROR) return PERROR; }
<STRING>\\n      { if (pparse_addchar('\n')==PERROR) return PERROR; }
<STRING>\n       { if (pparse_addchar('\n')==PERROR) return PERROR; }
<STRING>\\r      { if (pparse_addchar('\r')==PERROR) return PERROR; }
<STRING>\r       { if (pparse_addchar('\r')==PERROR) return PERROR; }
<STRING>\\t      { if (pparse_addchar('\t')==PERROR) return PERROR; }
<STRING>\t       { if (pparse_addchar('\t')==PERROR) return PERROR; }
<STRING>\\\\     { if (pparse_addchar('\\')==PERROR) return PERROR; }
<STRING>\\\"     { if (pparse_addchar('"')==PERROR) return PERROR; }
<STRING>\\\n     { }
<STRING>.        { if (pparse_addchar(*yytext)==PERROR) return PERROR; }

"and"             { return PAND; }
"argc"            { return PARGC; }
"argv"            { return PARGV; }
"block"           { return PBLOCK; }
"break"           { return PBREAK; }
"callvc"          { return PCALLVC; }
"continue"        { return PCONTINUE; }
"do"              { return PDO; }
"else"            { return PELSE; }
"end"             { return PEND; }
"exit"            { return PEXIT; }
"exitvc"          { return PEXITVC; }
"extern"          { return PEXTERN; }
"false"           { return PFALSE; }
"for"             { return PFOR; }
"func"            { return PFUNC; }
"global"          { return PGLOBAL; }
"globals"         { return PGLOBALS; }
"if"              { return PIF; }
"in"              { return PIN; }
"local"           { return PLOCAL; }
"locals"          { return PLOCALS; }
"not"             { return PNOT; }
"null"            { return PNULL; }
"or"              { return POR; }
"retc"            { return PRETC; }
"return"          { return PRETURN; }
"returnvc"        { return PRETURNVC; }
"retv"            { return PRETV; }
"true"            { return PTRUE; }
"undef"           { return PUNDEF; }
"unless"          { return PUNLESS; }
"until"           { return PUNTIL; }
"while"           { return PWHILE; }

\'\\a\'           { yylval.chrval = '\a'; return PCHR; }
\'\\b\'           { yylval.chrval = '\b'; return PCHR; }
\'\\f\'           { yylval.chrval = '\f'; return PCHR; }
\'\\n\'           { yylval.chrval = '\n'; return PCHR; }
\'\\r\'           { yylval.chrval = '\r'; return PCHR; }
\'\\t\'           { yylval.chrval = '\t'; return PCHR; }
\'\\\'\'          { yylval.chrval = '\''; return PCHR; }
\'\\\\\'          { yylval.chrval = '\\'; return PCHR; }
\'.\'             { yylval.chrval = yytext[1]; return PCHR; }

0[xX]{Hex}+       { yylval.intval = pparse_hex(yytext); return PINT; }
{Dec}+d?          { yylval.intval = atoi(yytext); return PINT; }
{Oct}+o           { yylval.intval = pparse_oct(yytext); return PINT; }
{Bin}+b           { yylval.intval = pparse_bin(yytext); return PINT; }
{Dec}+y             { yylval.chrval = (char)atoi(yytext); return PCHR; }
{Dec}+\.{Dec}+    { yylval.fltval = atof(yytext); return PFLT; }

{InitCh}{ICh}*      { yylval.strval = strdup(yytext); return PIDENTIFIER; }

">>"                { return PSHR; }
"<<"                { return PSHL; }
"<="                { return PLEQ; }
">="                { return PGEQ; }
"=="                { return PEQ; }
"~="                { return PNEQ; }
".*"                { return PMET; }
".^"                { return PSUP; }
"{"                 { return '{'; }
"}"                 { return '}'; }
"("                 { return '('; }
")"                 { return ')'; }
"["                 { return '['; }
"]"                 { return ']'; }
"."                 { return '.'; }
"#"                 { return '#'; }
"-"                 { return '-'; }
"@"                 { return '@'; }
"~"                 { return '~'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"%"                 { return '%'; }
"+"                 { return '+'; }
"&"                 { return '&'; }
"^"                 { return '^'; }
"|"                 { return '|'; }
"<"                 { return '<'; }
">"                 { return '>'; }
"="                 { return '='; }
";"                 { return ';'; }
":"                 { return ':'; }
","                 { return ','; }

[ \t\v\n\f]         { }
.                   { }

%%

int yywrap()
{
  return 1;
}

long pparse_oct(char *str)
{
  int i = 0, neg = 0;
  long acc = 0;
  if (str[i]=='-') {
    neg = 1;
    i++;
  }
  while (isdigit(str[i])) {
    acc *= 8;
    acc += str[i]-'0';
    i++;
  }
  if (neg) acc = -acc;
  return acc;
}

long pparse_bin(char *str)
{
  int i = 0, neg = 0;
  long acc = 0;
  if (str[i]=='-') {
    neg = 1;
    i++;
  } 
  while (isdigit(str[i])) {
    acc *= 2;
    acc += str[i]-'0';
    i++;
  }
  if (neg) acc = -acc;
  return acc;
}

long pparse_hex(char *str)
{
  int i = 0, neg = 0;
  long acc = 0;
  char c;
  if (str[i]=='-') {
    neg = 1;
    i++;
  }
  if ((!strncmp(str+i,"0x",2))||(!strncmp(str+i,"0X",2))) i+=2;
  while (c = str[i]) {
    acc *= 16;
    if ((c>='0')&&(c<='9'))
      acc += str[i]-'0';
    else if ((c>='A')&&(c<='F'))
      acc += str[i]-'A'+10;
    else if ((c>='a')&&(c<='f'))
      acc += str[i]-'a'+10;
    i++;
  }
  if (neg) acc = -acc;
  return acc;
}

int pparse_addchar(char c)
{
  char *otherbuf;
  if ((POE_BUFIND+1)>=POE_BUFSZ) {
    POE_BUFSZ *= 2;
    otherbuf = malloc(POE_BUFSZ);
    if (!otherbuf) {
      free(POE_STRBUF);
      return PERROR;
    }
    strncpy(otherbuf,POE_STRBUF,POE_BUFIND);
    free(POE_STRBUF);
    POE_STRBUF = otherbuf;
  }
  POE_STRBUF[POE_BUFIND] = c;
  POE_BUFIND ++;
  return PSUCCESS;
}
