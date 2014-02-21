/* pgc.h
 * Definitions for facilitating garbage collection
 */

#ifndef PGC_H_
#define PGC_H_

#include "pdata.h"
#include "pmem.h"
#include "puniverse.h"

enum poe_colors {
  POE_WHITE = 0, POE_GRAY = 1, POE_BLACK = 2, POE_PITCH = 3
};

enum poe_phases {
  POE_RESIZE_PHASE = 0, POE_MARK_WHITE_PHASE = 1, POE_MARK_BLACK_PHASE = 2,
  POE_FREE_WHITE_PHASE = 3, POE_ORIENT_PHASE = 4, POE_BUFFER_PHASE = 5
};

enum poe_cycle_modes {
  POE_NORMAL = 0, POE_EMERGENCY = 1
};


#define poe_max(a,b) (a)<(b)?(b):(a)
#define POE_MIN_LOAD_FACTOR 0.3
#define POE_DEFAULT_MAJ_OFFSET 5
#define POE_DEFAULT_TIMER 50
#define POE_DEFAULT_STEP_FACTOR 5.0
#define POE_DEFAULT_MIN_STEP 30

void pgc_stop(poe_universe *);
void pgc_resume(poe_universe *);
void pgc_set_stepfactor(poe_universe *, double);
int pgc_set_timer(poe_universe *, int);
void pgc_set_offset(poe_universe *, int);

void pgc_raw_begin(poe_universe *);
void pgc_fix_buf_stack(poe_universe *);

void poe_emergency_gc(poe_universe *);
int poe_gc_control(poe_universe *, poe_obj, char);
void pgc_step(poe_universe *);
void pgc_countdown(poe_universe *);
void pgc_cycle(poe_universe *, int, int);
void pgc_complete_cycle(poe_universe *, int);
void pgc_do_cycle(poe_universe *, int);
void pgc_force_major_cycle(poe_universe *);
void pgc_cleanout_factories(poe_universe *);

void pgc_blacken_table(poe_table *);
void pgc_blacken_arr(poe_arr *);
void pgc_blacken_str(poe_str *);
void pgc_blacken_pfunc(poe_pfunc *);
void pgc_blacken_code(poe_code *);

void pgc_traverse_table(poe_table *);
void pgc_traverse_arr(poe_arr *);
void pgc_traverse_str(poe_str *);
void pgc_traverse_pfunc(poe_pfunc *);

int pgc_resize_objects(poe_universe *, int);
int pgc_phase0_step(poe_universe *, int);
void pgc_phase1_step(poe_universe *, int);
void pgc_phase2_step(poe_universe *, int);
void pgc_phase3_step(poe_universe *, int, int);
void pgc_phase4_step(poe_universe *, int);
int pgc_phase5_step(poe_universe *, int);

#endif
