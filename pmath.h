/* pmath.h:
 * standard library math functions 
 */

#ifndef PMATH_H
#define PMATH_H

#include "pdata.h"
#include "puniverse.h"
#include "pgc.h"
#include "pexec.h"
#include <math.h>
#include <stdlib.h>

int poe_loadmath(poe_universe *);
int pmath_abs(poe_universe *);
int pmath_arccos(poe_universe *);
int pmath_arcsin(poe_universe *);
int pmath_arctan(poe_universe *);
int pmath_ceiling(poe_universe *);
int pmath_cos(poe_universe *);
int pmath_cosh(poe_universe *);
int pmath_deg(poe_universe *);
int pmath_exp(poe_universe *);
int pmath_floor(poe_universe *);
int pmath_fmod(poe_universe *);
int pmath_log(poe_universe *);
int pmath_log10(poe_universe *);
int pmath_pow(poe_universe *);
int pmath_rad(poe_universe *);
int pmath_rand(poe_universe *);
int pmath_srand(poe_universe *);
int pmath_sin(poe_universe *);
int pmath_sinh(poe_universe *);
int pmath_sqrt(poe_universe *);
int pmath_tan(poe_universe *);
int pmath_tanh(poe_universe *);
#endif
