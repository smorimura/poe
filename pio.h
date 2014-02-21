/* pio.h
 * 
 */

#include "pdata.h"
#include "puniverse.h"
#include <stdio.h>

#ifndef PIO_H
#define PIO_H

int poe_loadio(poe_universe *);
int pio_close(poe_universe *);
int pio_flush(poe_universe *);
int pio_open(poe_universe *);
int pio_read(poe_universe *);
int pio_readc(poe_universe *);
int pio_readf(poe_universe *);
int pio_readi(poe_universe *);
int pio_readl(poe_universe *);
int pio_tostr(poe_universe *);
int pio_write(poe_universe *);

#endif
