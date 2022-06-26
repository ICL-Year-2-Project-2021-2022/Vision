#ifndef support
#define support

#include "definitions.h"
#include "terasic_includes.h"
#include "alt_types.h"
#include "global.h"
#include <stdio.h>
#include "definitions.h"
#include "system.h"

int number_observations, frames_elapsed;
struct observation;

int to_hex_code (int r, int g, int b);
struct observation* initialize_observations();
void merge_observations (struct observation* base, struct observation* merge);
void print_observations (struct observation* obs);
void print_as_rgb(alt_u32 val, alt_u32 hsv);
void frame_isr();
void wait_frame(int number);
void wait_sop();
void wait_eop();





#endif
