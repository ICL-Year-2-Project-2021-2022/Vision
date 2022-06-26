#include "pipeline.h"

void one_pipeline(){
	if (slow == 1) usleep(1000000);
		  	  if (!halt){

				code = observations[observation_idx].code;

				//Apply HSV transform
				IOWR(RGB_TO_HSV_BASE, HSV_ENABLED, 0x1);



				//Color filter setup
				if (observations[observation_idx].code == 'W' || observations[observation_idx].code == 'X'){
					IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, 0);

					if (observations[observation_idx].code == 'W' ){
						fir_load_sobel(FIR_0_0_BASE);
						fir_load_vertical(FIR_0_1_BASE);
					}else if (observations[observation_idx].code == 'X'){
						fir_load_inv_sobel(FIR_0_0_BASE);
						fir_load_vertical(FIR_0_1_BASE);
					}

					//COM counter setup
					IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, 1);
					IOWR(COM_COUNTER_0_BASE, THRESH_Y, 200);
					IOWR(COM_COUNTER_0_BASE, THRESH_X, 10);
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

					//fir_load_avg(FIR_0_0_BASE);
					fir_load_unit(FIR_0_0_BASE);


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
}


void three_pipelines(){
	if (slow == 1) usleep(1000000);
		  	  if (!halt){
		  		static const int pipe0[2] = {6, 7};
		  		static int pipe0_idx = 0;
		  		static const int pipe1[3] = {0, 1, 2};
		  		static int pipe1_idx = 0;
		  		static const int pipe2[3] = {3, 4, 5};
		  		static int pipe2_idx = 0;


				//Apply HSV transform no matter what
				IOWR(RGB_TO_HSV_BASE, HSV_ENABLED, 0x1);



				//Pipeline 0 setup
				IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, 0);

				if (observations[pipe0[pipe0_idx]].code == 'W' ){
					fir_load_sobel(FIR_0_0_BASE);
					fir_load_vertical(FIR_0_1_BASE);
				}else if (observations[pipe0[pipe0_idx]].code == 'X'){
					fir_load_inv_sobel(FIR_0_0_BASE);
					fir_load_vertical(FIR_0_1_BASE);
				}

				//COM counter setup
				IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, 1);
				IOWR(COM_COUNTER_0_BASE, THRESH_Y, 200);
				IOWR(COM_COUNTER_0_BASE, THRESH_X, 10);
				IOWR(COM_COUNTER_0_BASE, THRESHOLD_GATE, observations[pipe0[pipe0_idx]].threshold);
				usleep(100000);

				 //Pipeline 0 collect

				//Collect results
				observations[pipe0[pipe0_idx]].mass = IORD(COM_COUNTER_0_BASE, COM_MASS);
				observations[pipe0[pipe0_idx]].com_x = IORD(COM_COUNTER_0_BASE, COM_X)/observations[pipe0[pipe0_idx]].mass;
				observations[pipe0[pipe0_idx]].com_y = IORD(COM_COUNTER_0_BASE, COM_Y)/observations[pipe0[pipe0_idx]].mass;

				observations[pipe0[pipe0_idx]].bb_left = IORD(COM_COUNTER_0_BASE, BB_LOW_X);
				observations[pipe0[pipe0_idx]].bb_top = IORD(COM_COUNTER_0_BASE, BB_LOW_Y);
				observations[pipe0[pipe0_idx]].bb_right = IORD(COM_COUNTER_0_BASE, BB_HIGH_X);
				observations[pipe0[pipe0_idx]].bb_bottom = IORD(COM_COUNTER_0_BASE, BB_HIGH_Y);

				if (observations[pipe0[pipe0_idx]].code == 'W' ){
					for (int i = 0; i < 20; i++){
						histogram[i] = IORD(EDGE_BINS_0_BASE, i);
					}
				}else if (observations[pipe0[pipe0_idx]].code == 'X'){
					for (int i = 0; i < 20; i++){
						histogram_processed[i] = histogram + IORD(EDGE_BINS_0_BASE, i);
					}

					merge_observations(&observations[pipe0[pipe0_idx]-1], &observations[pipe0[pipe0_idx]]);
				}

				//Increment pipe0 idx
				pipe0_idx++;
				if(pipe0_idx >= (sizeof(pipe0)/sizeof(int))){ pipe0_idx = 0;}





				//Pipeline 1 setup
				IOWR(COLOR_FILTER_1_BASE, APPLY_MASK, 1);
				printf("MAsk bottom: %x\n", observations[pipe1[pipe1_idx]].mask_bottom);
				IOWR(COLOR_FILTER_1_BASE, THRESHOLD_LOW, observations[pipe1[pipe1_idx]].mask_bottom);
				printf("MAsk bottom: %x\n", IORD(COLOR_FILTER_1_BASE, THRESHOLD_LOW));
				printf("MAsk top: %x\n", observations[pipe1[pipe1_idx]].mask_top);
				IOWR(COLOR_FILTER_1_BASE, THRESHOLD_HIGH, observations[pipe1[pipe1_idx]].mask_top);

				fir_load_unit(FIR_1_BASE);

				IOWR(COM_COUNTER_1_BASE, THRESH_ENABLED, 1);
				IOWR(COM_COUNTER_1_BASE, THRESH_Y, 200);
				IOWR(COM_COUNTER_1_BASE, THRESH_X, 10);
				IOWR(COM_COUNTER_1_BASE, THRESHOLD_GATE, observations[pipe1[pipe1_idx]].threshold);

			  	usleep(100000);

				//Pipeline 1 collect
				observations[pipe1[pipe1_idx]].mass = IORD(COM_COUNTER_1_BASE, COM_MASS);
				observations[pipe1[pipe1_idx]].com_x = IORD(COM_COUNTER_1_BASE, COM_X)/observations[pipe1[pipe1_idx]].mass;
				observations[pipe1[pipe1_idx]].com_y = IORD(COM_COUNTER_1_BASE, COM_Y)/observations[pipe1[pipe1_idx]].mass;

				observations[pipe1[pipe1_idx]].bb_left = IORD(COM_COUNTER_1_BASE, BB_LOW_X);
				observations[pipe1[pipe1_idx]].bb_top = IORD(COM_COUNTER_1_BASE, BB_LOW_Y);
				observations[pipe1[pipe1_idx]].bb_right = IORD(COM_COUNTER_1_BASE, BB_HIGH_X);
				observations[pipe1[pipe1_idx]].bb_bottom = IORD(COM_COUNTER_1_BASE, BB_HIGH_Y);


				printf("COM1 mass: %x", IORD(COM_COUNTER_1_BASE, COM_MASS));

				pipe1_idx++;
				if(pipe1_idx >= (sizeof(pipe1)/sizeof(int))) pipe1_idx = 0;


				//Pipeline 2 setup
				IOWR(COLOR_FILTER_2_BASE, APPLY_MASK, 1);
				IOWR(COLOR_FILTER_2_BASE, THRESHOLD_LOW, observations[pipe2[pipe2_idx]].mask_bottom);
				IOWR(COLOR_FILTER_2_BASE, THRESHOLD_HIGH, observations[pipe2[pipe2_idx]].mask_top);

				fir_load_avg(FIR_2_BASE);


				//COM counter setup
				IOWR(COM_COUNTER_2_BASE, THRESH_ENABLED, 1);
				IOWR(COM_COUNTER_2_BASE, THRESH_Y, 200);
				IOWR(COM_COUNTER_2_BASE, THRESH_X, 10);
				IOWR(COM_COUNTER_2_BASE, THRESHOLD_GATE, observations[pipe2[pipe2_idx]].threshold);


				//Wait two frame times
			  	usleep(100000);

				//Pipeline 2 collect
				observations[pipe2[pipe2_idx]].mass = IORD(COM_COUNTER_2_BASE, COM_MASS);
				observations[pipe2[pipe2_idx]].com_x = IORD(COM_COUNTER_2_BASE, COM_X)/observations[pipe2[pipe2_idx]].mass;
				observations[pipe2[pipe2_idx]].com_y = IORD(COM_COUNTER_2_BASE, COM_Y)/observations[pipe2[pipe2_idx]].mass;

				observations[pipe2[pipe2_idx]].bb_left = IORD(COM_COUNTER_2_BASE, BB_LOW_X);
				observations[pipe2[pipe2_idx]].bb_top = IORD(COM_COUNTER_2_BASE, BB_LOW_Y);
				observations[pipe2[pipe2_idx]].bb_right = IORD(COM_COUNTER_2_BASE, BB_HIGH_X);
				observations[pipe2[pipe2_idx]].bb_bottom = IORD(COM_COUNTER_2_BASE, BB_HIGH_Y);

				pipe2_idx++;
				if(pipe2_idx >= (sizeof(pipe2)/sizeof(int))) pipe2_idx = 0;

		   }
}
