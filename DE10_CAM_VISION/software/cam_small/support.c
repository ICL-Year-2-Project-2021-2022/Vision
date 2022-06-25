//Struct
#include "support.h"



int to_hex_code (int r, int g, int b){
	return (r<<16) + (g<<8) +  b;
}


struct observation* initialize_observations(){

	struct observation* arr = malloc(number_observations * sizeof(struct observation));

//Initialize Red low ball parameters --
	arr[0].code = 'R';
	arr[0].mask_bottom = to_hex_code(1,130,130);
	arr[0].mask_top = to_hex_code(8,255,240);
	arr[0].threshold = 192;

	//Initialize lime ball parameters +
	arr[1].code = 'L';
	arr[1].mask_bottom = to_hex_code(55,60,100);
	arr[1].mask_top = to_hex_code(75,160,240);
	arr[1].threshold = 192;

	//Initialize blue ball parameters --
	arr[2].code = 'B';
	arr[2].mask_bottom = to_hex_code(105,80,50);
	arr[2].mask_top = to_hex_code(125,255,128);
	arr[2].threshold = 192;

	//Initialize yellow ball parameters
	arr[3].code = 'Y';
	arr[3].mask_bottom = to_hex_code(10,100,160);
	arr[3].mask_top = to_hex_code(30,140,240);
	arr[3].threshold = 192;

	//Initialize green ball parameters
	arr[4].code = 'G';
	arr[4].mask_bottom = to_hex_code(77,60,50);
	arr[4].mask_top = to_hex_code(93,140, 140);
	arr[4].threshold = 192;


	//Initialize pink ball parameters +
	arr[5].code = 'P';
	arr[5].mask_bottom = to_hex_code(170,80,150);
	arr[5].mask_top = to_hex_code(180,160,240);
	arr[5].threshold = 192;

	//Initialize Building 1 parameters
	arr[6].code = 'W';
	arr[6].mask_bottom = to_hex_code(0,00,0);
	arr[6].mask_top = to_hex_code(255, 255, 255);
	arr[6].threshold = 64;


	//Initialize Building 2 parameters --
	arr[7].code = 'X';
	arr[7].mask_bottom = to_hex_code(0,0,0);
	arr[7].mask_top = to_hex_code(255, 255, 255);
	arr[7].threshold = 64;

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


void print_observations (struct observation* obs) {
	static char observation_buf[64];

	for (int i = 0; i < number_observations-1 ; i++){


		sprintf(observation_buf, "%c %01x %x %x %x %x %x %x", obs[i].code, obs[i].com_x,
				obs[i].com_y, obs[i].mass, obs[i].bb_left, obs[i].bb_top,
				obs[i].bb_right, obs[i].bb_bottom);


		printf ("%s", observation_buf);

		//Last iteration
		if(i == number_observations -2 ){
			printf ("\n");
		} else{
			printf (" ,");

		}
		while (alt_getchar() != 'A'){
			usleep(1);
		}
	}
}



void print_as_rgb(alt_u32 val, alt_u32 hsv){
	if(print){
		printf("RGB: %x, HSV: %x\n", val, hsv);
		usleep(100000);
	}
}



void frame_isr(){
	static int count = 0;
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

void wait_sop(){
	usleep(20000);
}
void wait_eop(){
	usleep(20000);
}


