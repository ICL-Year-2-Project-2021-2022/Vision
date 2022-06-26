#ifndef AUTO
#define AUTO

#include "definitions.h"
#include "alt_types.h"
#include "io.h"
#include "filters_buffers.h"
#include "global.h"


void auto_gain(int target, int epsilon);
void auto_wb(int target, int epsilon);

#endif
