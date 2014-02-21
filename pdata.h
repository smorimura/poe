/* pdata.h
 * definitions for atomic, compound data types used by Poe
 */

#ifndef PDATA_H_
#define PDATA_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A NOTE ON OBJECTS: A "Poe object", in terms of the PCL, is either a) a value
or b) a pointer to a compound data structure. Tables, arrays, strings, and
Poe code and functions (pfuncs) are compound data structures; all other Poe 
data are "values". The union "poe_obj" describes all of these objects; in order
to know which type to take from the Union, there exists the struct
"tagged_poe_obj". A Poe array is simply an array of tagged_poe_obj; a Poe
table is an array of lists of tagged Poe objs; and a string is simply
a string of bytes (with no tags being needed because a string can only
hold characters). Meanwhile, the various stacks which hold the Poe runtime
data are simply arrays of tagged_poe_obj, meaning that external heap storage
is needed only for tables, arrays, strings, pfuncs, and code. */

typedef union poe_obj poe_obj;
typedef struct tagged_poe_obj tagged_poe_obj;
typedef struct poe_obj_list poe_obj_list;
typedef struct poe_universe poe_universe;

/* A poe_obj is either an immediate value (long int, double, char, FILE *,
   pointer to C function) or a pointer to a composite data structure */
union poe_obj {
  /* ATOMIC DATA */
  long poe_int;
  double poe_float;
  char poe_char;
  FILE *poe_file;
  int (*poe_cfunc) (poe_universe*);
  /* COMPOSITE DATA */
  struct poe_str *poe_str;
  struct poe_arr *poe_arr;
  struct poe_table *poe_table;
  struct poe_code *poe_code;
  struct poe_pfunc *poe_pfunc;
  /* SIMPLE DATA (NULL, UNDEF, TRUE, FALSE): No simple data is kept inside the
     poe_obj itself, as you can glean these values from the tag of the 
     tagged_poe_obj itself. */
};

typedef struct poe_table {
  unsigned int gc_info : 2;
  unsigned int control : 1;
  unsigned int super_tag : 6; 
  int n;
  int size;
  poe_obj_list **t;
  struct poe_table *meta;
  poe_obj super;
} poe_table;

typedef struct poe_str {
  unsigned int gc_info : 2;
  unsigned int control : 1;
  unsigned int super_tag : 6;
  int len;
  int size;
  char *s;
  poe_table *meta;
  poe_obj super; /* type of super is given by super_tag */
} poe_str;

typedef struct poe_arr {
  unsigned int gc_info : 2;
  unsigned int control : 1;
  char super_tag : 6;
  int max;
  int size;
  tagged_poe_obj *a;
  poe_table *meta;
  poe_obj super; /* type of super is given by super_tag */
} poe_arr;

typedef struct poe_code {
  unsigned int gc_info : 2; 
  unsigned int control : 1;
  int size;
  char *code;
} poe_code;

typedef struct poe_pfunc {
  unsigned int gc_info : 2; 
  unsigned int control : 1;
  poe_table *parent;
  int size;
  char *code;
} poe_pfunc;

struct tagged_poe_obj {
  unsigned int tag : 6; /* (tagged_poe_objects never need to be gc'ed) */
  poe_obj data;
};

struct poe_obj_list {
  unsigned int tag : 6;
  poe_obj first;
  char s[32]; /* a copy of the symbolname is kept in here */
  poe_obj_list *next;
  poe_obj_list *prev;
  poe_obj_list **head;
};

#define POE_NULL_TAG 0
#define POE_INT_TAG 1
#define POE_CHAR_TAG 2
#define POE_FLOAT_TAG 3
#define POE_STR_TAG 4
#define POE_ARR_TAG 5
#define POE_TABLE_TAG 6
#define POE_CODE_TAG 7
#define POE_FILE_TAG 8
#define POE_CFUNC_TAG 9
#define POE_PFUNC_TAG 10
#define POE_FALSE_TAG 16
#define POE_TRUE_TAG 17
#define POE_UNDEF_TAG 32
//#define POE_OBJ_LIST_TAG 33

#define MAX_LOAD_FACTOR 0.8f

int poe_highprime(int);
int poe_lowprime(int);

void poe_emergency_gc(poe_universe *);
int poe_gc_control(poe_universe *, poe_obj, char tag);

unsigned long hash (poe_str *);
unsigned long rawhash (char *);

poe_obj poe_arr_get(poe_arr *, long, char *);
poe_obj poe_table_rawget(poe_table *, char *, char *);
poe_obj poe_table_get(poe_table *, poe_str *, char *);
char poe_string_get(poe_str *, long, char *);

int poe_arr_set(poe_universe *, poe_arr *, long, char, poe_obj);
int poe_str_set(poe_universe *, poe_str *, long, int);
int poe_str_write(poe_universe *, poe_str *, void *, int);
void rehash_everything(poe_table *, poe_obj_list **, int);
int poe_table_set(poe_universe *, poe_table *, poe_str *, char, poe_obj);
int poe_table_rawset(poe_universe *, poe_table *, char *, char, poe_obj);

#endif
