//Struct

#include "system.h"
#include "support.h"
#include <stdio.h>
#include "definitions.h"

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


int to_hex_code (int r, int g, int b){
	return (r<<16) + (g<<8) +  b;
}


struct observation* initialize_observations(){

	struct observation* arr = malloc(number_observations * sizeof(struct observation));

//Initialize Red low ball parameters --
	arr[1].code = 'R';
	arr[1].mask_bottom = to_hex_code(0,140,150);
	arr[1].mask_top = to_hex_code(8,255,250);
	arr[1].threshold = 192;

//	//Initialize Red high ball parameters --
//	arr[1].code = 'S';
//	arr[1].mask_bottom = to_hex_code(175,140,150);
//	arr[1].mask_top = to_hex_code(180,255,250);
//	arr[1].threshold = 192;

	//Initialize lime ball parameters +
	arr[2].code = 'L';
	arr[2].mask_bottom = to_hex_code(55,60,100);
	arr[2].mask_top = to_hex_code(75,160,250);
	arr[2].threshold = 192;

	//Initialize blue ball parameters --
	arr[3].code = 'B';
	arr[3].mask_bottom = to_hex_code(105,80,50);
	arr[3].mask_top = to_hex_code(125,255,128);
	arr[3].threshold = 192;

	//Initialize yellow ball parameters
	arr[4].code = 'Y';
	arr[4].mask_bottom = to_hex_code(8,100,180);
	arr[4].mask_top = to_hex_code(25,140,240);
	arr[4].threshold = 192;

	//Initialize green ball parameters
	arr[5].code = 'G';
	arr[5].mask_bottom = to_hex_code(80,60,50);
	arr[5].mask_top = to_hex_code(90,128, 128);
	arr[5].threshold = 192;


	//Initialize pink ball parameters +
	arr[6].code = 'P';
//	arr[6].mask_bottom = to_hex_code(160,80,150);
//	arr[6].mask_top = to_hex_code(180,128,255);
	arr[6].mask_bottom = to_hex_code(0,80,150);
	arr[6].mask_top = to_hex_code(10,128,250);
	arr[6].threshold = 192;

	//Initialize white ball parameters
	arr[7].code = 'W';
	arr[7].mask_bottom = to_hex_code(0,00,200);
	arr[7].mask_top = to_hex_code(180,80, 250);
	arr[7].threshold = 192;


	//Initialize black ball parameters --
	arr[8].code = 'K';
	arr[8].mask_bottom = to_hex_code(0,0,0);
	arr[8].mask_top = to_hex_code(180,60,40);
	arr[8].threshold = 192;

	return arr;
}

void merge_observations (struct observation* base, struct observation* merge){
	base->com_x = (base->com_x * base-> mass +  merge->com_x * merge->mass )/ (base->mass + merge->mass) ;
	base->com_y = (base->com_y * base-> mass +  merge->com_y * merge->mass )/ (base->mass + merge->mass) ;
	base->mass = base->mass + merge->mass;

	base->bb_left = (base->bb_left < merge ->bb_left )? base->bb_left : merge ->bb_left;
	base->bb_top = (base->bb_top < merge ->bb_top )? base->bb_top : merge ->bb_top;
	base->bb_right = (base->bb_right > merge ->bb_right )? base->bb_right : merge ->bb_right;
	base->bb_bottom = (base->bb_bottom > merge ->bb_bottom )? base->bb_bottom : merge ->bb_bottom;
}


#define UART_DEBUG 1

void print_observations (FILE* fp, struct observation* obs) {
	static char observation_buf[64];

	for (int i = 1; i < number_observations-2 ; i++){

//				if (obs[i].code == 'R'){
//					merge_observations(&obs[i], &obs[i+1]);
//					i++;
//				}

		sprintf(observation_buf, "%c %x %x %x %x %x %x %x", obs[i].code, obs[i].com_x,
				obs[i].com_y, obs[i].mass, obs[i].bb_left, obs[i].bb_top,
				obs[i].bb_right, obs[i].bb_bottom);

		if (UART_DEBUG){

			printf ("%s", observation_buf);

			//Last iteration
			if(i == number_observations-3 ){
				printf ("\n");
			} else{
				printf (" ,");

			}
			while (getchar() != 'A'){
				usleep(1);
			}
		} else{
			//Content write
			fwrite (observation_buf, strlen(observation_buf), 1, fp);

			//Ending write
			if(i == number_observations-3 ){
				fwrite (&"\n", 1, 1, fp);
			} else{
				fwrite (&" ,", 2, 1, fp);

			}
			while (getc(fp) != 'A'){
				usleep(1);
			}
		}
	}



}

void auto_gain(int target, int epsilon){
	int currentgain = 0x280;
	OV8865SetGain(currentgain);
	usleep(100000);

	int value = OV8865ReadAvg();
	int delta = target - value;
	int kp = 1;

	while ( delta > epsilon || delta < -epsilon){
		currentgain = currentgain + delta*kp;
		if (currentgain < 0) currentgain = 0;
		OV8865SetGain(currentgain);
		usleep(100000);

		value = OV8865ReadAvg();
		delta = target - value;

		printf("Delta: %d, gain: %d\n", delta, currentgain);
	}

}

void auto_wb(int target, int epsilon){
	int delay = 100000;
	int kp = 1;
	alt_u32 value2;
	//Red Balance
	int currentgain = 0x400;
	OV8865SetRedGain(currentgain);
	usleep(delay);
	 alt_u32 value = ((IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE)) & 0x00FF0000) >> 16;
	int delta = target - value;

	while ( delta > epsilon || delta < -epsilon){
		currentgain = currentgain + delta/4;
		if (currentgain < 0) currentgain = 0;
		OV8865SetRedGain(currentgain);
		usleep(delay);

		value = ((IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE)) & 0x00FF0000) >> 16;

		usleep(delay);

		value2 = ((IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE)) & 0x00FF0000) >> 16;

		delta = target - (value+value2)/2;

		printf("Red Delta: %d, gain: %d\n", delta, currentgain);
	}

	//Green Balance
	currentgain = 0x400;
	OV8865SetGreenGain(currentgain);
	usleep(delay);
	value = ((IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE)) & 0x0000FF00) >> 8;
	delta = target - value;

	while ( delta > epsilon || delta < -epsilon){
		currentgain = currentgain + delta/2;
		if (currentgain < 0) currentgain = 0;
		OV8865SetGreenGain(currentgain);
		usleep(delay);

		value = ((IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE)) & 0x0000FF00) >> 8;

		usleep(delay);

		value2 = ((IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE)) & 0x0000FF00) >> 8;

		delta = target - (value+value2)/2;

		printf("Green Delta: %d, gain: %d\n", delta, currentgain);
	}

	//Blue Balance
	currentgain = 0x400;
	OV8865SetBlueGain(currentgain);
	usleep(delay);
	value = ((IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE)) & 0x000000FF);
	delta = target - value;

	while ( delta > epsilon || delta < -epsilon){
		currentgain = currentgain + delta/2;
		if (currentgain < 0) currentgain = 0;
		OV8865SetBlueGain(currentgain);
		usleep(delay);

		value = ((IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE)) & 0x000000FF);

		usleep(delay);

		value2 = ((IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE)) & 0x000000FF);

		delta = target - (value+value2)/2;

		printf("Blue Delta: %d, gain: %d\n", delta, currentgain);
		}

}

void print_as_rgb(alt_u32 val, alt_u32 hsv){
	//printf("RGB: %d, %d, %d\n", (val>>16)&0x0000ff, (val>>8)&0x0000ff, (val>>0)&0x0000ff);
	printf("RGB: %x, HSV: %x\n", val, hsv);
}



void frame_isr(){
	static int count;
	if (count == 59){
		count = 0;
		printf("One second");
	}
	frames_elapsed++;
}

void wait_frame(int number){
	int current_frame = frames_elapsed;
	while(current_frame > frames_elapsed - number){}
	return;
}
