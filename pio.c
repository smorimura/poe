/* pio.c: I/O standard library functions
 *
 */

#include "pdata.h"
#include "puniverse.h"
#include "pio.h"
#include "pexec.h"
#include "pdo1.h"
#include "pdo2.h"
#include <stdio.h>

int poe_loadio(poe_universe *U)
{
  poe_table *io;
  io = poe_make_table(U);
  if (!io) return 0;
  if (!poe_table_rawset(U,io,"close",POE_CFUNC_TAG,(poe_obj)&pio_close)) 
    return 0;
  if (!poe_table_rawset(U,io,"flush",POE_CFUNC_TAG,(poe_obj)&pio_flush)) 
    return 0;
  if (!poe_table_rawset(U,io,"open",POE_CFUNC_TAG,(poe_obj)&pio_open)) 
    return 0;
  if (!poe_table_rawset(U,io,"read",POE_CFUNC_TAG,(poe_obj)&pio_read)) 
    return 0;
  if (!poe_table_rawset(U,io,"readc",POE_CFUNC_TAG,(poe_obj)&pio_readc))
    return 0;
  if (!poe_table_rawset(U,io,"readf",POE_CFUNC_TAG,(poe_obj)&pio_readf)) 
    return 0;
  if (!poe_table_rawset(U,io,"readi",POE_CFUNC_TAG,(poe_obj)&pio_readi)) 
    return 0;
  if (!poe_table_rawset(U,io,"readl",POE_CFUNC_TAG,(poe_obj)&pio_readl)) 
    return 0;
  if (!poe_table_rawset(U,io,"stderr",POE_FILE_TAG,(poe_obj)stderr)) return 0;
  if (!poe_table_rawset(U,io,"stdin",POE_FILE_TAG,(poe_obj)stdin)) return 0;
  if (!poe_table_rawset(U,io,"stdout",POE_FILE_TAG,(poe_obj)stdout)) return 0;
  if (!poe_table_rawset(U,io,"tostr",POE_CFUNC_TAG,(poe_obj)&pio_tostr)) 
    return 0;
  if (!poe_table_rawset(U,io,"write",POE_CFUNC_TAG,(poe_obj)&pio_write)) 
    return 0;
  if (!poe_pushobj(U,(poe_obj)io,POE_TABLE_TAG)) return 0;
  if (!pexec_ass(U,GLOBAL,"io")) return 0;
  if (!poe_gc_control(U,(poe_obj)io,POE_TABLE_TAG)) return 0;
  return 1;
}

int pio_close(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    if (tag!=POE_FILE_TAG) {
      printf("argument %d is not a file\n",i);
      return 0;
    }
    fclose(o.poe_file);
  }
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retv = retv;
  U->retc = 0;
  return 1;
}

int pio_flush(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], i;
  poe_obj o;
  char tag;
  for (i=0; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    if (tag!=POE_FILE_TAG) {
      printf("argument %d is not a file\n",i);
      return 0;
    }
    fflush(o.poe_file);
  }
  retv = poe_make_arr(U);
  if (!retv) return 0;
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  U->retc = 0;
  return 1;
}

int pio_open(poe_universe *U)
{
  poe_obj fname, opts;
  poe_arr *argv, *retv;
  char ftype, opttype;
  FILE *f;
  if (U->argc_stack_base[U->argc_stack_index]!=2) {
    printf("io.open: did not receive 2 arguments\n");
    return 0;
  }
  argv = U->argv_stack_base[U->argv_stack_index];
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  fname = poe_pop(U,&ftype);
  if (ftype!=POE_STR_TAG) {
    printf("io.open: arg 1 is not a string\n");
    return 0;
  }
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)1L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  opts = poe_pop(U,&opttype);
  if (opttype!=POE_STR_TAG) {
    printf("io.open: arg 2 is not a string\n");
    return 0;
  }
  f = fopen(fname.poe_str->s,opts.poe_str->s);
  if (!f) return 0;
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (!poe_arr_set(U,retv,0,POE_FILE_TAG,(poe_obj)f)) return 0;
  U->retv = retv;
  U->retc = 1;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}

int pio_read(poe_universe *U)
{
  poe_arr *argv, *retv;
  poe_str *buf;
  FILE *f;
  int i, n, argc, inchar, control = 0;
  poe_obj no, nf;
  char notag, nftag;
  argv = U->argv_stack_base[U->argv_stack_index];
  if ((argc = U->argc_stack_base[U->argc_stack_index])==0) {
    printf("io.read: received 0 arguments\n");
    return 0;
  } else if (argc==1) { // we need file and strbuf
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    no = poe_pop(U,&notag);
    if (notag!=POE_INT_TAG) {
      printf("io.read: argument 1 is not an integer\n");
      return 0;
    }
    n = no.poe_int;
    if (!pexec_get(U,GLOBAL,"io")) return 0;
    if (!pexec_pushnull(U)) return 0;
    if (!pexec_tabacc(U,"stdin")) return 0;
    nf = poe_pop(U,&nftag);
    if (nftag!=POE_FILE_TAG) {
      printf("io.read: argument 2 is not a file\n");
      return 0;
    }
    f = nf.poe_file;
    buf = poe_make_str(U);
    control = 1;
    if (!buf) return 0;
  } else if (argc==2) { // we just need strbuf
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    no = poe_pop(U,&notag);
    if (notag!=POE_INT_TAG) {
      printf("io.read: argument 1 is not an integer\n");
      return 0;
    }
    n = no.poe_int;
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)1L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    nf = poe_pop(U,&nftag);
    if (nftag!=POE_FILE_TAG) {
      printf("io.read: argument 2 is not an file\n");
      return 0;
    }
    f = nf.poe_file;
    buf = poe_make_str(U);
    if (!buf) return 0;
    control = 1;
  } else if (argc>=3) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    no = poe_pop(U,&notag);
    if (notag!=POE_INT_TAG) {
      printf("io.read: argument 1 is not an integer\n");
      return 0;
    }
    n = no.poe_int;
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)1L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    nf = poe_pop(U,&nftag);
    if (nftag!=POE_FILE_TAG) {
      printf("io.read: argument 2 is not an file\n");
      return 0;
    }
    f = nf.poe_file;
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)2L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    no = poe_pop(U,&notag);
    if (notag!=POE_STR_TAG) {
      printf("io.read: argument 3 is not a string\n");
      return 0;
    }
    buf = no.poe_str;
  }
  for (i=0; i<n; i++) {
    inchar = fgetc(f);
    if (inchar==EOF) break;
    if (!poe_pushobj(U,(poe_obj)(char)inchar,POE_CHAR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)buf,POE_STR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (!poe_pushobj(U,(poe_obj)buf,POE_STR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arrset(U)) return 0;
  if (inchar==EOF) {
    if (!poe_pushobj(U,(poe_obj)1L,POE_TRUE_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)1L,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  U->retv = retv;
  U->retc = 1;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  if (control) 
    if (!poe_gc_control(U,(poe_obj)buf,POE_STR_TAG)) return 0;
  return 1;
}

int pio_readc(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index], inchar;
  poe_obj o;
  char tag;
  FILE *in;
  if (argc>=1) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    if (tag!=POE_FILE_TAG) {
      printf("io.readi: arg1 is not a file\n");
      return 0;
    }
    in = o.poe_file;
  } else if (argc==0) {
    if (!pexec_get(U,GLOBAL,"io")) return 0;
    if (!pexec_pushnull(U)) return 0;
    if (!pexec_tabacc(U,"stdin")) return 0;
    o = poe_pop(U,&tag);
    if (tag!=POE_FILE_TAG) {
      printf("io.readi: io.stdin is not a file\n");
      return 0;
    }
    in = o.poe_file;
  }
  retv = poe_make_arr(U);
  if (!retv) return 0;
  inchar = fgetc(in);
  if (inchar==EOF) {
    if (!poe_pushobj(U,(poe_obj)0L,POE_NULL_TAG)) return 0;
  } else {
    if (!poe_pushobj(U,(poe_obj)(char)inchar,POE_CHAR_TAG)) return 0;
  }
  if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arrset(U)) return 0;
  U->retv = retv;
  U->retc = 1;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}

int pio_readl(poe_universe *U)
{
  poe_arr *argv, *retv;
  poe_str *sout;
  FILE *in;
  int i, argc, inchar, control = 0;
  char otag;
  poe_obj o;
  argv = U->argv_stack_base[U->argv_stack_index];
  if ((argc = U->argc_stack_base[U->argc_stack_index])==0) { // file and str
    if (!pexec_get(U,GLOBAL,"io")) return 0;
    if (!pexec_pushnull(U)) return 0;
    if (!pexec_tabacc(U,"stdin")) return 0;
    o = poe_pop(U,&otag);
    if (otag!=POE_FILE_TAG) {
      printf("io.readf: io.stdin is not a file\n");
      return 0;
    }
    in = o.poe_file;
    sout = poe_make_str(U);
    if (!sout) return 0;
    control = 1;
  } else if (argc==1) { // just str
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&otag);
    if (otag!=POE_FILE_TAG) {
      printf("io.readf: argument 1 is not a file\n");
      return 0;
    }
    in = o.poe_file;
    sout = poe_make_str(U);
    if (!sout) return 0;
    control = 1;
  } else if (argc==2) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&otag);
    if (otag!=POE_FILE_TAG) {
      printf("io.readf: argument 1 is not a file\n");
      return 0;
    }
    in = o.poe_file;
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)1L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&otag);
    if (otag!=POE_STR_TAG) {
      printf("io.readf: argument 2 is not a string\n");
      return 0;
    }
    sout = o.poe_str;
  }
  i = 0;
  inchar = fgetc(in);
  while (1) {
    if (inchar==EOF) break;
    if (inchar=='\n') break;
    if (!poe_pushobj(U,(poe_obj)(char)inchar,POE_CHAR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)sout,POE_STR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
    i ++;
    inchar = fgetc(in);
  }
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (!poe_pushobj(U,(poe_obj)sout,POE_STR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arrset(U)) return 0;
  if (inchar==EOF) {
    if (!poe_pushobj(U,(poe_obj)1L,POE_TRUE_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)1L,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
    U->retc = 2;
  } else U->retc = 1;
  U->retv = retv;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  if (control)
    if (!poe_gc_control(U,(poe_obj)sout,POE_STR_TAG)) return 0;
  return 1;
}

int pio_readi(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index];
  long i;
  poe_obj o;
  char tag;
  FILE *in;
  if (argc>=1) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    if (tag!=POE_FILE_TAG) {
      printf("io.readi: arg1 is not a file\n");
      return 0;
    }
    in = o.poe_file;
  } else if (argc==0) {
    if (!pexec_get(U,GLOBAL,"io")) return 0;
    if (!pexec_pushnull(U)) return 0;
    if (!pexec_tabacc(U,"stdin")) return 0;
    o = poe_pop(U,&tag);
    if (tag!=POE_FILE_TAG) {
      printf("io.readi: io.stdin is not a file\n");
      return 0;
    }
    in = o.poe_file;
  }
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (fscanf(in,"%ld",&i)!=1) {
    if (!pexec_pushnull(U)) return 0;
  } else {
    if (!poe_pushobj(U,(poe_obj)i,POE_INT_TAG)) return 0;
  }
  if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arrset(U)) return 0;
  U->retv = retv;
  U->retc = 1;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}

int pio_readf(poe_universe *U)
{
  poe_arr *argv = U->argv_stack_base[U->argv_stack_index], *retv;
  int argc = U->argc_stack_base[U->argc_stack_index];
  double i;
  poe_obj o;
  char tag;
  FILE *in;
  if (argc>=1) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    if (tag!=POE_FILE_TAG) {
      printf("io.readi: arg1 is not a file\n");
      return 0;
    }
    in = o.poe_file;
  } else if (argc==0) {
    if (!pexec_get(U,GLOBAL,"io")) return 0;
    if (!pexec_pushnull(U)) return 0;
    if (!pexec_tabacc(U,"stdin")) return 0;
    o = poe_pop(U,&tag);
    if (tag!=POE_FILE_TAG) {
      printf("io.readi: io.stdin is not a file\n");
      return 0;
    }
    in = o.poe_file;
  }
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (fscanf(in,"%lf",&i)!=1) {
    if (!pexec_pushnull(U)) return 0;
  } else {
    if (!poe_pushobj(U,(poe_obj)i,POE_FLOAT_TAG)) return 0;
  }
  if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arrset(U)) return 0;
  U->retv = retv;
  U->retc = 1;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}

/* This tostr implementation is not as smart as it should be -- this is a 
   known issue */

int pio_tostr(poe_universe *U)
{
  poe_arr *argv, *retv;
  poe_str *sout;
  FILE *in;
  int i, argc, inchar, control = 0;
  char otag;
  poe_obj o;
  argv = U->argv_stack_base[U->argv_stack_index];
  if ((argc = U->argc_stack_base[U->argc_stack_index])==0) { // file and str
    if (!pexec_get(U,GLOBAL,"io")) return 0;
    if (!pexec_pushnull(U)) return 0;
    if (!pexec_tabacc(U,"stdin")) return 0;
    o = poe_pop(U,&otag);
    if (otag!=POE_FILE_TAG) {
      printf("io.readf: io.stdin is not a file\n");
      return 0;
    }
    in = o.poe_file;
    sout = poe_make_str(U);
    if (!sout) return 0;
    control = 1;
  } else if (argc==1) { // just str
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&otag);
    if (otag!=POE_FILE_TAG) {
      printf("io.readf: argument 1 is not a file\n");
      return 0;
    }
    in = o.poe_file;
    sout = poe_make_str(U);
    if (!sout) return 0;
    control = 1;
  } else if (argc==2) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&otag);
    if (otag!=POE_FILE_TAG) {
      printf("io.readf: argument 1 is not a file\n");
      return 0;
    }
    in = o.poe_file;
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)1L,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&otag);
    if (otag!=POE_STR_TAG) {
      printf("io.readf: argument 2 is not a string\n");
      return 0;
    }
    sout = o.poe_str;
  }
  i = 0;
  while ((inchar = fgetc(in)) != EOF) {
    if (!poe_pushobj(U,(poe_obj)(char)inchar,POE_CHAR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)sout,POE_STR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arrset(U)) return 0;
    i ++;
  }
  retv = poe_make_arr(U);
  if (!retv) return 0;
  if (!poe_pushobj(U,(poe_obj)sout,POE_STR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arrset(U)) return 0;
  U->retv = retv;
  U->retc = 1;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  if (control)
    if (!poe_gc_control(U,(poe_obj)sout,POE_STR_TAG)) return 0;
  return 1;
}

int pio_write(poe_universe *U)
{
  poe_arr *argv, *retv;
  FILE *out;
  int argc, i;
  poe_obj o;
  char tag;
  argv = U->argv_stack_base[U->argv_stack_index];
  if ((argc = U->argc_stack_base[U->argc_stack_index])<=0) {
    printf("io.write: received 0 arguments\n");
    return 0;
  } 
  if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
  if (!poe_pushobj(U,(poe_obj)0L,POE_INT_TAG)) return 0;
  if (!pexec_arracc(U)) return 0;
  o = poe_pop(U,&tag);
  if (tag!=POE_FILE_TAG) {
    printf("io.write: argument 1 is not a file\n");
    return 0;
  }
  out = o.poe_file;
  for (i=1; i<argc; i++) {
    if (!poe_pushobj(U,(poe_obj)argv,POE_ARR_TAG)) return 0;
    if (!poe_pushobj(U,(poe_obj)(long)i,POE_INT_TAG)) return 0;
    if (!pexec_arracc(U)) return 0;
    o = poe_pop(U,&tag);
    switch (tag) {
    case POE_STR_TAG:
      fwrite(o.poe_str->s,1,o.poe_str->len,out);
      break;
    case POE_CHAR_TAG:
      fputc(o.poe_char, out);
      break;
    case POE_INT_TAG:
      fputc(o.poe_int, out);
      break;
    default:
      printf("io.write: argument %d is not a string, char, or int\n",i);
      return 0;
    }
  }
  retv = poe_make_arr(U);
  if (!retv) return 0;
  U->retv = retv;
  U->retc = 0;
  if (!poe_gc_control(U,(poe_obj)retv,POE_ARR_TAG)) return 0;
  return 1;
}
