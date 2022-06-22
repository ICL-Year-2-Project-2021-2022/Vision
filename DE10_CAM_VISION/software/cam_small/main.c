#include <stdio.h>
#include "I2C_core.h"
#include "terasic_includes.h"
#include "mipi_camera_config.h"
#include "mipi_bridge_config.h"
#include "auto_focus.h"
#include "system.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "sys/alt_irq.h"
#include "sys/alt_stdio.h"
#include "mipi_setup.h"


#include "definitions.h"
#include "input.h"
#include "filters_buffers.h"
#include "support.h"
#include "global.h"

int number_observations = 8;
int frames_elapsed  = 0;
int histogram[20];
int histogram_processed[20];

void get_building_hist(){

	fir_load_sobel(FIR_0_0_BASE);
	IOWR(COM_COUNTER_0_BASE, THRESH_Y, 64);
	IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, 0);
	wait_sop();
	wait_eop();

	for (int i = 0; i<20; i++){
		histogram[i] = IORD(EDGE_BINS_0_BASE, i);
		alt_printf("Bin %x: %x \n", i, histogram[i]);
	}

	//fir_load_avg();
	return;
}



int main(){

	print = 0;

	//Sets stdin to nonblocking
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	if (print){
		printf("Imperial College EEE2 Project version\n");
	}
  IOWR(MIPI_PWDN_N_BASE, 0x00, 0x00);
  IOWR(MIPI_RESET_N_BASE, 0x00, 0x00);

  usleep(2000);
  IOWR(MIPI_PWDN_N_BASE, 0x00, 0xFF);
  usleep(2000);
  IOWR(MIPI_RESET_N_BASE, 0x00, 0xFF);

  usleep(2000);


  // MIPI Init
   if (!MIPI_Init() & print){
	  printf("MIPI_Init Init failed!\r\n");
  }

	mipi_clear_error();
	usleep(50*1000);
	mipi_clear_error();
	usleep(1000*1000);
	mipi_show_error_info();
	printf("\n");

	fir_load_unit(FIR_0_0_BASE);

//Camera parameters initialization
//////////////////////////////////////////////////////////

	OV8865SetExposure(EXPOSURE_INIT);

//	//Manual calibration
//	OV8865SetGain(926);
	OV8865SetRedGain(930);
	OV8865SetGreenGain(800);
	OV8865SetBlueGain(1222);

	//Autocalibration
	Focus_Init();
	auto_gain(160,2);
	//auto_wb(128 ,2);
	//auto_gain(160,2);
	//Focus_Init();


////////////////////////////////////////////////////////////////


	//Apply HSV transform
	IOWR(RGB_TO_HSV_BASE, HSV_ENABLED, 0x1);

	//Color filter setup
	IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, 1);

	//COM counter setup
	IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, 1);
	IOWR(COM_COUNTER_0_BASE, THRESH_Y, 200);
	IOWR(COM_COUNTER_0_BASE, THRESH_X, 10);
	IOWR(COM_COUNTER_0_BASE, THRESH_Y_MAX, 470);
	IOWR(COM_COUNTER_0_BASE, THRESH_X_MAX, 620);

	//Force disable center dots
	IOWR(PIXEL_GRABBER_HSV_BASE, GRAB_INDICATOR, 0);
	IOWR(PIXEL_GRABBER_RGB_BASE, GRAB_INDICATOR, 0);


	//fir_load_unit(FIR_0_1_BASE);
	//fir_load_avg();
	//fir_load_sobel();

	usleep(100000);
	//print_image();

	//get_building_hist();

	//Initialize observation data structures
	observations = initialize_observations();
	int observation_idx = 0;

	//Register frame interrupt
	//alt_irq_register(2, 0, frame_isr);
	//alt_ic_isr_register(2, 0, frame_isr, 0, 0);
	//alt_ic_irq_enable (0, 2);
	//alt_ic_isr_register(0,2, frame_isr,0,0);
	//print_wb_buffer();

	int rgb, hsv;
	debug = 0;
	char code;
	delay = 50000;

  while(1){

	   //Read Pixel Grab
	   rgb = IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE);
	   hsv = IORD(PIXEL_GRABBER_HSV_BASE, GRAB_VALUE);
	   print_as_rgb(rgb, hsv);


	   if(!debug){
		   if ((IORD(KEY_BASE,0)&0x03) == 0x01){
		  	    	   debug = 1;
		   }

		   //Update observation parameters
		   	if(print){
		   		printf("Collecting %c \n", observations[observation_idx].code );
		   	}

			code = observations[observation_idx].code;

			//Apply HSV transform
			IOWR(RGB_TO_HSV_BASE, HSV_ENABLED, 0x1);



			//Color filter setup
			if (observations[observation_idx].code == 'W' || observations[observation_idx].code == 'X'){
				IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, 0);

				if (observations[observation_idx].code == 'W' ){
					fir_load_sobel(FIR_0_0_BASE);
					//fir_load_vertical(FIR_0_1_BASE);
				}else if (observations[observation_idx].code == 'X'){
					fir_load_inv_sobel(FIR_0_0_BASE);
					//fir_load_vertical(FIR_0_1_BASE);
				}

				//COM counter setup
				IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, 1);
				IOWR(COM_COUNTER_0_BASE, THRESH_Y, 200);
				IOWR(COM_COUNTER_0_BASE, THRESH_X, 10);
				IOWR(COM_COUNTER_0_BASE, THRESHOLD_GATE, observations[observation_idx].threshold);


				//Wait two frame times
					usleep(delay);


				//Collect results
				observations[observation_idx].mass = IORD(COM_COUNTER_0_BASE, COM_MASS);
				observations[observation_idx].com_x = IORD(COM_COUNTER_0_BASE, COM_X)/observations[observation_idx].mass;
				observations[observation_idx].com_y = IORD(COM_COUNTER_0_BASE, COM_Y)/observations[observation_idx].mass;

				observations[observation_idx].bb_left = IORD(COM_COUNTER_0_BASE, BB_LOW_X);
				observations[observation_idx].bb_top = IORD(COM_COUNTER_0_BASE, BB_LOW_Y);
				observations[observation_idx].bb_right = IORD(COM_COUNTER_0_BASE, BB_HIGH_X);
				observations[observation_idx].bb_bottom = IORD(COM_COUNTER_0_BASE, BB_HIGH_Y);

				if (observations[observation_idx].code == 'W' ){
					for (int i = 0; i < 20; i++){
						histogram[i] = IORD(EDGE_BINS_0_BASE, i);
					}
				}else if (observations[observation_idx].code == 'X'){
					for (int i = 0; i < 20; i++){
						histogram_processed[i] = histogram + IORD(EDGE_BINS_0_BASE, i);
					}

					merge_observations(&observations[observation_idx-1], &observations[observation_idx]);
				}


			}else {
				IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, 1);
				IOWR(COLOR_FILTER_0_BASE, THRESHOLD_LOW, observations[observation_idx].mask_bottom);
				IOWR(COLOR_FILTER_0_BASE, THRESHOLD_HIGH, observations[observation_idx].mask_top);

				fir_load_avg(FIR_0_0_BASE);
				//fir_load_unit(FIR_0_1_BASE);


				//COM counter setup
				IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, 1);
				IOWR(COM_COUNTER_0_BASE, THRESH_Y, 200);
				IOWR(COM_COUNTER_0_BASE, THRESH_X, 10);
				IOWR(COM_COUNTER_0_BASE, THRESHOLD_GATE, observations[observation_idx].threshold);

				//Wait two frame times
				   usleep(delay);


				//Collect results
				observations[observation_idx].mass = IORD(COM_COUNTER_0_BASE, COM_MASS);
				observations[observation_idx].com_x = IORD(COM_COUNTER_0_BASE, COM_X)/observations[observation_idx].mass;
				observations[observation_idx].com_y = IORD(COM_COUNTER_0_BASE, COM_Y)/observations[observation_idx].mass;

				observations[observation_idx].bb_left = IORD(COM_COUNTER_0_BASE, BB_LOW_X);
				observations[observation_idx].bb_top = IORD(COM_COUNTER_0_BASE, BB_LOW_Y);
				observations[observation_idx].bb_right = IORD(COM_COUNTER_0_BASE, BB_HIGH_X);
				observations[observation_idx].bb_bottom = IORD(COM_COUNTER_0_BASE, BB_HIGH_Y);

			}

		   observation_idx++;
		   if (observation_idx == number_observations)observation_idx = 0;
	   }

	   	   IOWR(LED_BASE, 0, debug * 511);


	   //printf("Red %x, Green %x, Blue %x", red_average(), green_average(), blue_average());
	   process_input();
  }
}
