#include "input.h"


void process_input(){
	static alt_u8  manual_focus_step = 10;
	static alt_u16  current_focus = 300;
	alt_u32 exposureTime = EXPOSURE_INIT;
	alt_u16 gain = GAIN_INIT;

	// touch KEY0 to trigger Auto focus
		   if((IORD(KEY_BASE,0)&0x03) == 0x02){
			   usleep(100000);
	    	   current_focus = Focus_Window(320,240);
	    	   usleep(100000);
	       }
		   // touch KEY1 with selected routine to run
		   else if ((IORD(KEY_BASE,0)&0x03) == 0x01){
	    	   int sw = (IORD(SW_BASE, 0)& 0x03FF);
	    	   usleep(1000000);

	    	   switch(sw){
				   case 1 : {
					   auto_gain(140 , 2);
					   break;
				   }case 2: {
					   auto_wb(140 , 3);
					   break;
				   }case 4: {
					   IOWR(RGB_TO_HSV_BASE, HSV_ENABLED, ~IORD(RGB_TO_HSV_BASE, HSV_ENABLED));
					   if(print){
						   printf("HSV: %x\n", IORD(RGB_TO_HSV_BASE, HSV_ENABLED));
					   }
					   break;
				   }case 8: {
					   IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, ~IORD(COLOR_FILTER_0_BASE, APPLY_MASK));
					   IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, ~IORD(COM_COUNTER_0_BASE, THRESH_ENABLED));

					   if(print){
						   printf("MASK:  %x\n", IORD(COLOR_FILTER_0_BASE, APPLY_MASK));
						   printf("THRESH:  %x\n", IORD(COM_COUNTER_0_BASE, THRESH_ENABLED));
					   }
					   break;
				   }case 16: {
					   fir_load_unit(FIR_0_0_BASE);
					   fir_load_unit(FIR_0_1_BASE);
					   break;
				   }case 32: {
					   fir_load_avg(FIR_0_0_BASE);
					   fir_load_unit(FIR_0_1_BASE);
					   break;
				   }case 64: {
					   fir_load_sobel(FIR_0_0_BASE);
					   fir_load_unit(FIR_0_1_BASE);
					   break;
				   }case 128: {
					   halt = !halt;
					   break;
				   }case 256: {
					   slow = !slow;
					   break;
				   }case 512: {
					   Focus_Init();
					   break;
				   }case 768: {
					   one = !one;
					   break;
				   }
	    	   }

		   usleep(100000);

	       }

		   //Process input commands
		  int in = alt_getchar();
		  switch (in) {
			   case 'R': {
				   //Process request
				   print_observations(observations);
				   break;}
			   case 'P': {
				   //Process request
				   print_image();
				   break;}

			   case 'H': {
					   for(int i = 0; i<20; i++){
						   alt_printf("%x ", histogram[i]);
						   if( i == 19 ){
							   alt_putchar('\n');
						   }else{
							   alt_putchar(' ');
						   }
					   }
				   break;}
			   case 'D': {
				   printf("%x", IORD(OBSTACLE_DIST_0_BASE,0));
				   break;}
			   case 'S': {
				   halt = !halt;
				   break;}
			   case 'C': {
				   //Autocalibration
				   	Focus_Init();
				   	auto_gain(160,2);
				   	auto_wb(128 ,2);
				   	auto_gain(160,2);
				   	Focus_Init();
					break;}

			   case 'e': {
				   exposureTime += EXPOSURE_STEP;
				   OV8865SetExposure(exposureTime);
				   //printf("\nExposure = %x ", exposureTime);
				   break;}
			   case 'd': {
				   exposureTime -= EXPOSURE_STEP;
				   OV8865SetExposure(exposureTime);
				   //printf("\nExposure = %x ", exposureTime);
				   break;}
			   case 't': {
				   gain += GAIN_STEP;
				   OV8865SetGain(gain);
				   //printf("\nGain = %x ", gain);
				   break;}
			   case 'g': {
				   gain -= GAIN_STEP;
				   OV8865SetGain(gain);
				   //printf("\nGain = %x ", gain);
				   break;}
			   case 'r': {
				   current_focus += manual_focus_step;
				   if(current_focus >1023) current_focus = 1023;
				   OV8865_FOCUS_Move_to(current_focus);
				   //printf("\nFocus = %x ",current_focus);
				   break;}
			   case 'f': {
				   if(current_focus > manual_focus_step) current_focus -= manual_focus_step;
				   OV8865_FOCUS_Move_to(current_focus);
				   //printf("\nFocus = %x ",current_focus);
				   break;}
		  	  };

}
