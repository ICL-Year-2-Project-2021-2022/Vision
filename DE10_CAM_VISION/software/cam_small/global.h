#ifndef GLOBAL
#define GLOBAL


int number_observations;
int frames_elapsed;
int histogram[20];
int histogram_processed[20];
int debug;
int print;
int delay;
int halt;

struct observation {
	char code;
	//Per color parameters
	int mask_bottom;
	int mask_top;
	int threshold;

	//Up most up to date data
	int com_x;
	int com_y;
	int mass;
	int bb_left;
	int bb_top;
	int bb_right;
	int bb_bottom;

};

struct observation* observations;

#endif
