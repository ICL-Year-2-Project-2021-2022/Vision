#ifndef FILTER
#define FILTER

#include "definitions.h"
#include "alt_types.h"
#include "io.h"


void fir_load_unit(int base);
void fir_load_avg(int base);
void fir_load_sobel(int base);
void fir_load_inv_sobel(int base);
void fir_load_vertical(int base);
void print_image();
void print_wb_buffer();

int red_average();
int green_average();
int blue_average();

#endif
