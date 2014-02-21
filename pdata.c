/* pdata.c
 * Basic functions for data manipulation
 */

#include "pdata.h"
#include "pmem.h"

int poe_goodprimes[] = {
  43, 97, 193, 389, 769, 1543, 3079, 6151
};
#define GOODPRIMES_SIZE 8

int poe_highprime(int a)
{
  int i;
  for (i=0; i<GOODPRIMES_SIZE-1; i++)
    if (poe_goodprimes[i]>a)
      return poe_goodprimes[i];
  return a;
}

int poe_lowprime(int a)
{
  int i;
  for (i=GOODPRIMES_SIZE-1; i>=0; i--)
    if (poe_goodprimes[i]<a)
      return poe_goodprimes[i];
  return a;
}

/* Each function handles errors differently; errors in these functions
   will come primarily from data allocation. */

/* Bernstein algorithm from comp.lang.c; adapted from "djb2" section of
   http://www.cse.yorku.ca/~oz/hash.html */
unsigned long hash(poe_str *ps)
{
  unsigned long h = 5381;
  int i, len = ps->len;
  char *s = ps->s;
  for (i=0; i<len; i++)
    h = ((h << 5) + h) + s[i]; /* h*33 + c */
  return h;
}

/* same as hash, but requires a NULL-terminated char* rather than a poe_str */
unsigned long rawhash(char *s)
{
  unsigned long h = 5381;
  int i;
  char c;
  for (i=0; c = s[i]; i++)
    h = ((h << 5) + h) + c; /* h*33 + c */
  return h;

}

/* return the poe_obj at index i of array arr, storing the type in
   tag */
poe_obj poe_arr_get(poe_arr *arr, long i, char *tag)
{
  if (!arr) {
    *tag = POE_UNDEF_TAG;
    return (poe_obj)0L;
  }
  if (i>(arr->max)) {
    *tag = POE_UNDEF_TAG;
    return (poe_obj)0L;
  }
  *tag = (arr->a)[i].tag;
  return (arr->a)[i].data;
}

poe_obj poe_table_rawget(poe_table *tab, char *s, char *tag)
{
  poe_obj_list *l;
  if (!tab) {
    *tag = POE_UNDEF_TAG;
    return (poe_obj)0L;
  }
  l = (tab->t)[rawhash(s)%(tab->size)];
  if (!l) {
    *tag = POE_UNDEF_TAG;
    return (poe_obj)0L;
  }
  while (l) {
    if (!strcmp(s,l->s)) {
      *tag = l->tag;
      return l->first;
    }
    l = l->next;
  }
  *tag = POE_UNDEF_TAG;
  return (poe_obj)0L;
}

/* return the poe_obj with key s in table tab, storing the type in tag */
poe_obj poe_table_get(poe_table *tab, poe_str *s, char *tag)
{
  return poe_table_rawget(tab,s->s,tag);
}

/* return the character at index i of string s, storing the type in tag */
char poe_string_get(poe_str *s, long i, char *tag)
{
  if (i>=(s->len)) {
    *tag = POE_UNDEF_TAG;
    return 0;
  }
  *tag = POE_CHAR_TAG;
  return (s->s)[i];
}

/* store the object o with type tag at position x of array arr. To set
   to undef, make sure tag is POE_UNDEF_TAG. Returns 1 on success
   and 0 on error. */
int poe_arr_set(poe_universe *U, poe_arr *arr, long x, char tag, poe_obj o)
{
  int newsize, oldsize, z;
  tagged_poe_obj *new_arr, *old_arr = arr->a;
  if (x<(oldsize = arr->size)) {
    old_arr[x].tag = tag;
    old_arr[x].data = o;
    arr->max = x>(arr->max) ? x : arr->max;
  } else {
    new_arr = malloc((newsize = x*2) * sizeof(tagged_poe_obj));
    if (!new_arr) {
      poe_emergency_gc(U);
      new_arr = malloc(newsize * sizeof(tagged_poe_obj));
      if (!new_arr) return 0;
    }
    for (z=0; z<oldsize; z++)
      new_arr[z] = old_arr[z];
    free(old_arr);
    new_arr[x].tag = tag;
    new_arr[x].data = o;
    arr->a = new_arr;
    arr->size = newsize;
    arr->max = x;
  }
  return 1;
}

/* set the character c to s[i]. Will set character to undef if c<0.
   Will raise an error (return 0) if i>len(s). Returns 1 on success,
   0 on failure. */
int poe_str_set(poe_universe *U, poe_str *s, long i, int c)
{
  int len = s->len, z;
  char *new_str, *old_str = s->s;
  if (i>len) return 0;
  if (i==len) {
    if (len+1>=s->size) { // case: we need to re-allocate string
      new_str = malloc((s->size)*2);
      if (!new_str) {
	poe_emergency_gc(U);
	new_str = malloc((s->size)*2);
	if (!new_str) return 0;
      }
      for (z=0; z<s->size; z++)
	new_str[z] = old_str[z];
      free(old_str);
      s->size *= 2;
      s->s = new_str;
    }
    (s->s)[i+1] = '\0';
    s->len ++;
  }
  if (c<0) {
    (s->s)[i] = '\0';
    s->len = i;
  } else 
    (s->s)[i] = c;
  return 1;
}

/* write n characters of s to ps. Return 1 on success and 0 on failure. */
int poe_str_write(poe_universe *U, poe_str *ps, void *s, int n)
{
  char *new_str, *S = s;
  if (n+1>=(ps->size)) { // case: we need to resize string
    new_str = malloc(n*2);
    if (!new_str) {
      poe_emergency_gc(U);
      new_str = malloc(n*2);
      if (!new_str) return 0;
    }
    free(ps->s);
    ps->s = new_str;
    ps->size = n*2;
  }
  new_str = ps->s;
  ps->len = n;
  while (n--)
    *new_str++ = *S++;
  *new_str = '\0';
  return 1;
}

/* rehash every value in the given table old into the new poe_obj_list */
void rehash_everything(poe_table *o, poe_obj_list **new, int newsize)
{
  int i;
  poe_obj_list **old = o->t, *l, *buf;
  for (i=0; i<(o->size); i++)
    for (l = old[i]; l; l = buf) {
      buf = l->next;
      poe_obj_list_attach(l,new+rawhash(l->s)%newsize);
    }
  return;
}

/* set the key s in table t to the given poe_obj o w/ type tag. Returns
   1 on success and 0 on failure. */
int poe_table_rawset(poe_universe *U, poe_table *tab, char *s, char tag, 
		     poe_obj o)
{
  poe_obj_list **old, **new, *l, *ret;
  int i, newsize;
  if (((float)(tab->n)/(float)(tab->size))>MAX_LOAD_FACTOR) {
    // case: we must resize the table
    newsize = poe_highprime(tab->size);
    new = malloc(sizeof(poe_obj_list *) * newsize);
    if (!new) {
      poe_emergency_gc(U);
      new = malloc(sizeof(poe_obj_list *) * newsize);
      if (!new) return 0;
    }
    for (i=0; i<newsize; i++)
      new[i] = NULL;
    old = tab->t;
    rehash_everything(tab,new,newsize);
    tab->t = new;
    tab->size = newsize;
    free(old);
  }
  i = rawhash(s)%(tab->size);
  if (tag==POE_UNDEF_TAG) { // case: we need to delete an entry
    l = (tab->t)[i];
    while (l) {
      if (!strcmp(s,l->s)) {
	poe_obj_list_remove(U,l);
	tab->n--;
	break;
      }
      l = l->next;
    }
  } else { // case: we need to add/edit an entry
    for (l = (tab->t)[i]; l; l = l->next) {
      if (!strcmp(s,l->s)) {
	l->first = o;
	l->tag = tag;
	return 1;
      }
    }
    if (!(ret = poe_obj_list_cons(U,tag,o,tab->t+i,s))) return 0;
    tab->n++;
  }
  return 1;
}

/* int poe_str_set(poe_universe *U, poe_table *tab, poe_str *s, char tag, 
		poe_obj o)
{
  return poe_str_rawset(U,tab,s->s,tag,o);
}
*/
