/* pstring.h
 *
 */

#ifndef PSTRING_H
#define PSTRING_H

#include "pdata.h"
#include "puniverse.h"
#include "pexec.h"
#include "pdo1.h"
#include "pdo2.h"
#include <string.h>
#include <stdio.h>

int poe_loadstrlib(poe_universe *);
int pstring_cmp(poe_universe *);
int pstring_ncmp(poe_universe *);

#endif
