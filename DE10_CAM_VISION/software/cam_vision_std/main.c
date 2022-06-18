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

#include "definitions.h"
#include "mipi_setup.h"
#include "support.h"


int number_observations = 9;
int frames_elapsed  = 0;

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

int uart_rready(){
	return (IORD(UART_0_BASE,2)& 0x080);
}

int main()
{
	int rgb, hsv;

	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

  printf("DE10-LITE D8M VGA Demo\n");
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

//Camera parameters
//////////////////////////////////////////////////////////
	alt_u8  manual_focus_step = 10;
	alt_u16  current_focus = 300;
	alt_u32 exposureTime = EXPOSURE_INIT;
	alt_u16 gain = GAIN_INIT;

	OV8865SetExposure(exposureTime);

//Manual calibration
//OV8865SetGain(503);
//OV8865SetGain(1090);
OV8865SetGain(926);
OV8865SetRedGain(930);
//OV8865SetGreenGain(698);
OV8865SetGreenGain(800);
OV8865SetBlueGain(1222);

	//Autocalibration
//	Focus_Init();
//	auto_gain(160,2);
//	//auto_wb(210 ,2);
//	auto_gain(160,2);
//	Focus_Init();


////////////////////////////////////////////////////////////////
	printf("Got here\n");

	//Apply HSV transform
	IOWR(RGB_TO_HSV_BASE, HSV_ENABLED, 0x1);

	//Color filter setup
    //IOWR(COLOR_FILTER_0_BASE, THRESHOLD_LOW, 0x323050);
    //IOWR(COLOR_FILTER_0_BASE, THRESHOLD_HIGH, 0x46ffff);
//	IOWR(COLOR_FILTER_0_BASE, THRESHOLD_LOW, 0x0000b0);
//	IOWR(COLOR_FILTER_0_BASE, THRESHOLD_HIGH, 0xffffff);
	IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, 1);

	//Linear blue disable there must be bugs in hardware because this causes everything to break.
	//IOWR(LINEAR_BLUR_0_BASE, BLUR_ENABLED, 0);

	//COM counter setup
	IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, 1);
	IOWR(COM_COUNTER_0_BASE, THRESH_Y, 200);

	//Init rgbgrabber
	//IOWR(PIXEL_GRABBER_HSV_BASE, GRAB_POINT, 0x014000f0);
	//int point = IORD(PIXEL_GRABBER_HSV_BASE, GRAB_POINT);
	IOWR(PIXEL_GRABBER_HSV_BASE, GRAB_INDICATOR, 0);
	IOWR(PIXEL_GRABBER_RGB_BASE, GRAB_INDICATOR, 0);
	//point = IORD(PIXEL_GRABBER_HSV_BASE, GRAB_INDICATOR);
	//point = IORD(PIXEL_GRABBER_HSV_BASE, GRAB_INDICATOR);
	printf("Got here\n");
	fir_load_unit();
	//fir_load_avg();
	//fir_load_sobel();
	printf("Got here\n");

/////////////////////////////////////////////////////////////////////
	//Open UART for arduino communication
	FILE* fp = fopen("/dev/uart_0", "r+");
	if(fp){
		printf("Opened UART\n");
	} else {
		printf("Failed to open UART\n");
		while (1);
	}


	//Initialize observation data structures
	struct observation* observations = initialize_observations();
	int observation_idx = 2;

	//Register frame interrupt
	//alt_ic_isr_register(5, 0, frame_isr);
	//alt_ic_isr_register(0,2, frame_isr,0,0);




  while(1){


       // touch KEY0 to trigger Auto focus
	   if((IORD(KEY_BASE,0)&0x03) == 0x02){

    	   current_focus = Focus_Window(320,240);
       }
	   // touch KEY1 with selected routine to run
	   else if ((IORD(KEY_BASE,0)&0x03) == 0x01){
    	   int sw = (IORD(SW_BASE, 0)& 0x03FF);

    	   switch(sw){
			   case 1 : {
				   auto_gain(110,2);
				   break;
			   }case 2: {
				   auto_wb(127 , 3);
				   break;
			   }case 4: {
				   IOWR(RGB_TO_HSV_BASE, HSV_ENABLED, ~IORD(RGB_TO_HSV_BASE, HSV_ENABLED));
				   break;
			   }case 8: {
				   IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, ~IORD(COLOR_FILTER_0_BASE, APPLY_MASK));
				   IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, ~IORD(COM_COUNTER_0_BASE, THRESH_ENABLED));
				   break;
			   }case 16: {
				   fir_load_unit();
				   break;
			   }case 32: {
				   fir_load_avg();
				   break;
			   }case 64: {
				   fir_load_sobel();
				   break;
			   }
    	   }
    	   printf("SW: %x", sw);

       }

	   //Read Pixel Grab
	   rgb = IORD(PIXEL_GRABBER_RGB_BASE, GRAB_VALUE);
	   hsv = IORD(PIXEL_GRABBER_HSV_BASE, GRAB_VALUE);
	   print_as_rgb(rgb, hsv);

	   //Update observation values

	   	printf("Collecting %c \n", observations[observation_idx].code );
	   //Update setting for frame
		IOWR(COLOR_FILTER_0_BASE, THRESHOLD_LOW, observations[observation_idx].mask_bottom);
		IOWR(COLOR_FILTER_0_BASE, THRESHOLD_HIGH, observations[observation_idx].mask_top);
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

	   observation_idx++;
	   if (observation_idx == number_observations-2)observation_idx = 1;

	   //For normal usar operation
	   if (uart_rready() && getc(fp) == 'R') {

		   printf("Got rready\n");
		   print_observations(fp, observations);
	   }


       //Process input commands
       int in = getchar();
       switch (in) {
       	   case 'R': {
       		   //Process request
       		   print_observations(fp, observations);
       	   	   break;}
       	   case 'e': {
			   exposureTime += EXPOSURE_STEP;
			   OV8865SetExposure(exposureTime);
			   printf("\nExposure = %x ", exposureTime);
			   break;}
       	   case 'd': {
       		   exposureTime -= EXPOSURE_STEP;
       		   OV8865SetExposure(exposureTime);
       		   printf("\nExposure = %x ", exposureTime);
       	   	   break;}
       	   case 't': {
       		   gain += GAIN_STEP;
       		   OV8865SetGain(gain);
       		   printf("\nGain = %x ", gain);
       	   	   break;}
       	   case 'g': {
       		   gain -= GAIN_STEP;
       		   OV8865SetGain(gain);
       		   printf("\nGain = %x ", gain);
       	   	   break;}
       	   case 'r': {
        	   current_focus += manual_focus_step;
        	   if(current_focus >1023) current_focus = 1023;
        	   OV8865_FOCUS_Move_to(current_focus);
        	   printf("\nFocus = %x ",current_focus);
       	   	   break;}
       	   case 'f': {
        	   if(current_focus > manual_focus_step) current_focus -= manual_focus_step;
        	   OV8865_FOCUS_Move_to(current_focus);
        	   printf("\nFocus = %x ",current_focus);
       	   	   break;}

       }


	   //Main loop delay
	   usleep(10000);

   };
  return 0;
}
