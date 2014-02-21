/* pmain.c
 * functions for interpreter
 */

#include "pdata.h"
#include "puniverse.h"
#include "pbc.h"
#include "pexec.h"
#include "pmem.h"
#include "pstd.h"
#include "pio.h"
#include "pmath.h"
#include "pstring.h"
#include "pmain.h"

extern unsigned char POE_STD_PBC[];

/* prototype usage: ./pint source output */
int main(int argc, char *argv[])
{
  char *is, *fname, *cname = NULL, *loc;
  poe_universe *U;
  int flen, U_argc;
  poe_arr* U_argv;

  /* 1. COMPILATION */
  if (argc==1) {
    printf("pint usage:\n\tpint filename.poe\nOR\n\tpint filename.pbc\n");
    exit(1);
  }
  else fname = strdup(argv[1]);
  flen = strlen(fname);
  if (flen<=4) {
    printf("poe: the filename must be at least 5 characters long,\n");
    printf("ending in .poe or .pbc\n");
    exit(1);
  }
  if (!strncmp(fname+flen-4,".poe",4)) { // case: source code 
    cname = strdup(fname);
    loc = strstr(cname,".poe");
    memcpy(loc,".pbc",4);
    if (!pbc_compiles(fname,cname)) {
      fprintf(stderr,"compile failure\n");
      exit(1);
    }
    free(fname);
    fname = cname;
    cname = NULL;
  } else if (!strncmp(fname+flen-4,".pbc",4)) {
  } else {
    printf("poe: the filename must be at least 5 characters long,\n");
    printf("ending in .poe or .pbc\n");
    exit(1);
  }

  /* 2. UNIVERSE CREATION */
  U = poe_big_bang();
  if (!U) {
    fprintf(stderr,"big bang error\n");
    exit(1);
  }

  /* 3. LOAD STANDARD LIBRARIES */
  if (!poe_loadglobs(U)) {
    fprintf(stderr,"std library loading error\n");
    exit(1);  
  }
  if (!poe_loadio(U)) {
    fprintf(stderr,"io library loading error\n");
    exit(1);  
  }
  if (!poe_loadmath(U)) {
    fprintf(stderr,"math library loading error\n");
    exit(1);
  }
  if (!poe_loadstrlib(U)) {
    fprintf(stderr,"string library loading error\n");
    exit(1);
  }
  poe_exec_code(U,POE_STD_PBC,NULL); // load standard library functions written in Poe
  is = pint_load_file(fname);
  if (!is) {
    fprintf(stderr,"file load error\n");
    exit(1);
  }

  /* 4. CREATE ARGV */
  U_argv = poe_make_arr(U);
  if (!U_argv) {
    fprintf(stderr,"Error creating U_argv\n");
    exit(1);
  }
  for (U_argc=0; U_argc+2<argc; U_argc++) {
    if (!pexec_newstr(U)) return 0;
    if (!pexec_writestr(U,argv[U_argc+2],NULL)) return 0;
    if (!poe_pushobj(U,(poe_obj)U_argv,POE_ARR_TAG)) return 0;
    if (!pexec_pushint(U,U_argc)) return 0;
    if (!pexec_arrset(U)) return 0;
  }
  if (!poe_pushobj(U,(poe_obj)U_argv,POE_ARR_TAG)) return 0;
  if (!pexec_pushargv(U)) return 0;
  if (!poe_pushobj(U,(poe_obj)(long)U_argc,POE_INT_TAG)) return 0;
  if (!pexec_pushargc(U)) return 0;
  if (!poe_gc_control(U,(poe_obj)U_argv,POE_ARR_TAG)) return 0;

  /* 5. EXECUTE CODE */
  poe_exec_code(U,is,NULL);

  /* 6. CLEANUP */
  poe_heat_death(U);
  free(fname);
  free(is);
  return 0;
}
