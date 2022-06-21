//System.h definitions not catching


//Common
#define PIXEL_GRABBER_RGB_BASE 0x32100
#define PIXEL_BUFFER_0_BASE 0x0
#define PIXEL_BUFFER_WB_0_BASE 0x30800
#define RGB_TO_HSV_BASE 0x32140
#define PIXEL_GRABBER_HSV_BASE 0x320c0


//Pipeline 0
#define COLOR_FILTER_0_BASE 0x321c0
#define FIR_0_0_BASE 0x31800
//#define FIR_0_1_BASE 0x41000
#define COM_COUNTER_0_BASE 0x32180
#define EDGE_BINS_0_BASE 0x32000
#define OBSTACLE_DIST_0_BASE 0x32080


//Pipeline 1
#define COLOR_FILTER_1_BASE 0x43140
#define FIR_1_BASE 0x41800
#define COM_COUNTER_1_BASE 0x43100

//Pipeline 2
#define COLOR_FILTER_2_BASE 0x430c0
#define FIR_2_BASE 0x41000
#define COM_COUNTER_2_BASE 0x43080


//HSV converter
#define HSV_ENABLED 0

//Color mask offsets
#define THRESHOLD_LOW 0
#define THRESHOLD_HIGH 1
#define APPLY_MASK 2


//Linear Blur
#define BLUR_ENABLED 0

//Com counter
#define THRESHOLD_GATE 0
#define COM_X 1
#define COM_Y 2
#define COM_MASS 3
#define BB_LOW_X 4
#define BB_LOW_Y 5
#define BB_HIGH_X 6
#define BB_HIGH_Y 7
#define THRESH_ENABLED 8
#define THRESH_Y 9
#define THRESH_X 10

//Pixel grabber
#define GRAB_POINT 0
#define GRAB_VALUE 1
#define GRAB_INDICATOR 2




//Camera defaults
#define EXPOSURE_INIT 0x002200
#define EXPOSURE_STEP 0x100
#define GAIN_INIT 0x180
#define GAIN_STEP 0x040
#define DEFAULT_LEVEL 3


