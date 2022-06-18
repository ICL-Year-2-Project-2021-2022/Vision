#ifndef support
#define support

#include "definitions.h"
#include "terasic_includes.h"
#include "alt_types.h"

int number_observations, frames_elapsed;
struct observation;

int to_hex_code (int r, int g, int b);
struct observation* initialize_observations();
void merge_observations (struct observation* base, struct observation* merge);
void print_observations (FILE* fp, struct observation* obs);
void auto_gain(int target, int epsilon);
void auto_wb(int target, int epsilon);
void print_as_rgb(alt_u32 val, alt_u32 hsv);
void frame_isr();
void wait_frame(int number);





#endif
