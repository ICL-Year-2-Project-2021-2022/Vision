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


#include "definitions.h"
#include "mipi_setup.h"
#include "support.h"
#include "global.h"

int number_observations = 9;
int frames_elapsed  = 0;
int histogram[20];
int histogram_processed[20];




void fir_load_unit(){
	//Clear filter coefficients
	for (int i = 7; i < 7+9; i++){
		IOWR(ALT_VIP_CL_2DFIR_0_BASE, i, 0);
	}
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 11, 128);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 6, 1);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 0, 1);
}

void fir_load_avg(){
	for (int i = 7; i < 7+9; i++){
		IOWR(ALT_VIP_CL_2DFIR_0_BASE, i, 14);
	}
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 6, 1);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 0, 1);
}


void fir_load_sobel(){
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 7, -128);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 8, 0);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 9, 128);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 10, -128);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 11, 0);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 12, 128);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 13, -128);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 14, 0);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 15, 128);

	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 6, 1);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 0, 1);

}

void fir_load_inv_sobel(){
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 7, 128);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 8, 0);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 9, -128);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 10, 128);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 11, 0);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 12, -128);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 13, 128);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 14, 0);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 15, -128);

	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 6, 1);
	IOWR(ALT_VIP_CL_2DFIR_0_BASE, 0, 1);

}

int uart_rready(){
	return (IORD(UART_0_BASE,2)& 0x080);
}

void get_building_hist(){
	fir_load_sobel();
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

void print_image(int dest){
	int x = 0;
	int y = 0;
	int burst = 0;

	for(int i = 0; i <60*80; i++){
		IOWR(PIXEL_BUFFER_0_BASE, 5000, 1);
		alt_u32 pixel =(IORD(PIXEL_BUFFER_0_BASE, 64*x + y));
		char upper = (pixel & 0xf0)>>4;
		char lower = (pixel & 0x0f);

		alt_putchar(97 + upper);
		alt_putchar(97 + lower);

		burst++;
		if (burst == 30){
			burst = 0;
			if (y == 59 && x > 50){
				alt_putchar('\n');
			}else {
				alt_putchar(',');
			}
//			while(alt_getchar() != 'A'){
//				usleep(10);
//			}
			IOWR(PIXEL_BUFFER_0_BASE, 5000, 1);
		}



		//XY indexing
		x = x+1;
		if (x == 80){
			x = 0;
			y = y + 1;
		}
	}
	return;
}


int main()
{
	int rgb, hsv;
	//Sets stdin to nonblocking
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

  printf("Imperial College EEE2 Project version\n");
  IOWR(MIPI_PWDN_N_BASE, 0x00, 0x00);
  IOWR(MIPI_RESET_N_BASE, 0x00, 0x00);

  usleep(2000);
  IOWR(MIPI_PWDN_N_BASE, 0x00, 0xFF);
  usleep(2000);
  IOWR(MIPI_RESET_N_BASE, 0x00, 0xFF);

  usleep(2000);


  // MIPI Init
   if (!MIPI_Init()){
	  printf("MIPI_Init Init failed!\r\n");
  }else{
	  printf("MIPI_Init Init successfully!\r\n");
  }

	mipi_clear_error();
	usleep(50*1000);
	mipi_clear_error();
	usleep(1000*1000);
	mipi_show_error_info();
	printf("\n");

//Camera parameters initialization
//////////////////////////////////////////////////////////

	OV8865SetExposure(EXPOSURE_INIT);

	//Manual calibration
	OV8865SetGain(926);
	OV8865SetRedGain(930);
	OV8865SetGreenGain(800);
	OV8865SetBlueGain(1222);

	//Autocalibration
//	Focus_Init();
//	auto_gain(160,2);
//	//auto_wb(210 ,2);
//	auto_gain(160,2);
//	Focus_Init();


////////////////////////////////////////////////////////////////

	//Apply HSV transform
	IOWR(RGB_TO_HSV_BASE, HSV_ENABLED, 0x0);

	//Color filter setup
	IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, 0);

	//COM counter setup
	IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, 0);
	IOWR(COM_COUNTER_0_BASE, THRESH_Y, 200);

	//Force disable center dots
	IOWR(PIXEL_GRABBER_HSV_BASE, GRAB_INDICATOR, 0);
	IOWR(PIXEL_GRABBER_RGB_BASE, GRAB_INDICATOR, 0);

	fir_load_unit();
	//fir_load_avg();
	//fir_load_sobel();

	usleep(100000);
	print_image(0);

	get_building_hist();


/////////////////////////////////////////////////////////////////////
//	//Open UART for arduino communication
//	FILE* fp = fopen("/dev/uart_0", "r+");
//	if(fp){
//		printf("Opened UART\n");
//	} else {
//		printf("Failed to open UART\n");
//		while (1);
//	}




	//Initialize observation data structures
	observations = initialize_observations();
	int observation_idx = 2;

	//Register frame interrupt
	//alt_irq_register(2, 0, frame_isr);
	//alt_ic_isr_register(2, 0, frame_isr, 0, 0);
	//alt_ic_irq_enable (0, 2);
	//alt_ic_isr_register(0,2, frame_isr,0,0);




  while(1){
	   //Read Pixel Grab
	   rgb = IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE);
	   hsv = IORD(PIXEL_GRABBER_HSV_BASE, GRAB_VALUE);
	   print_as_rgb(rgb, hsv);



	   //Update observation parameters

	   	printf("Collecting %c \n", observations[observation_idx].code );

	   	//Apply HSV transform
	   	IOWR(RGB_TO_HSV_BASE, HSV_ENABLED, 0x1);

		//Color filter setup
		IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, 1);
		IOWR(COLOR_FILTER_0_BASE, THRESHOLD_LOW, observations[observation_idx].mask_bottom);
		IOWR(COLOR_FILTER_0_BASE, THRESHOLD_HIGH, observations[observation_idx].mask_top);

		//Load filter
		if (observations[observation_idx].code == 'W' ){
			fir_load_sobel();
		}else if (observations[observation_idx].code == 'X'){
			fir_load_inv_sobel();
		} else {
			fir_load_avg();
		}

		//COM counter setup
		IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, 1);
		IOWR(COM_COUNTER_0_BASE, THRESH_Y, 200);
		IOWR(COM_COUNTER_0_BASE, THRESHOLD_GATE, observations[observation_idx].threshold);


		//Wait two frame times
		usleep(100000);


		//Collect results
		observations[observation_idx].mass = IORD(COM_COUNTER_0_BASE, COM_MASS);
		observations[observation_idx].com_x = IORD(COM_COUNTER_0_BASE, COM_X)/observations[observation_idx].mass;
		observations[observation_idx].com_y = IORD(COM_COUNTER_0_BASE, COM_Y)/observations[observation_idx].mass;

		observations[observation_idx].bb_left = IORD(COM_COUNTER_0_BASE, BB_LOW_X);
		observations[observation_idx].bb_top = IORD(COM_COUNTER_0_BASE, BB_LOW_Y);
		observations[observation_idx].bb_right = IORD(COM_COUNTER_0_BASE, BB_HIGH_X);
		observations[observation_idx].bb_bottom = IORD(COM_COUNTER_0_BASE, BB_HIGH_Y);


		if (observations[observation_idx].code == 'X'){
			//merge
		}

		//Merge together
	   observation_idx++;
	   if (observation_idx == number_observations)observation_idx = 1;



	   //For normal usar operation
//	   if (uart_rready() && getc(fp) == 'R') {
//
//		   printf("Got rready\n");
//		   print_observations(fp, observations);
//	   }

	   process_input();



  }
}
