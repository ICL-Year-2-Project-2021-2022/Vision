#include "auto.h"

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


	//Red Balance
	int currentgain = 0x400;
	OV8865SetRedGain(currentgain);
	usleep(delay);
	 alt_u32 value = red_average();
	int delta = target - value;

	while ( delta > epsilon || delta < -epsilon){
		currentgain = currentgain + delta/2;
		if (currentgain < 0) currentgain = 0;
		OV8865SetRedGain(currentgain);
		usleep(delay);

		value = red_average();

		delta = target - value;

		printf("Red Delta: %d, gain: %d\n", delta, currentgain);
	}

	//Green Balance
	currentgain = 0x400;
	OV8865SetGreenGain(currentgain);
	usleep(delay);
	value = green_average();
	delta = target - value;

	while ( delta > epsilon || delta < -epsilon){
		currentgain = currentgain + delta/2;
		if (currentgain < 0) currentgain = 0;
		OV8865SetGreenGain(currentgain);
		usleep(delay);

		value = green_average();

		delta = target - value;

		printf("Green Delta: %d, gain: %d\n", delta, currentgain);
	}

	//Blue Balance
	currentgain = 0x400;
	OV8865SetBlueGain(currentgain);
	usleep(delay);
	value = blue_average();
	delta = target - value;

	while ( delta > epsilon || delta < -epsilon){
		currentgain = currentgain + delta/2;
		if (currentgain < 0) currentgain = 0;
		OV8865SetBlueGain(currentgain);
		usleep(delay);

		value = blue_average();

		delta = target - value;

		printf("Blue Delta: %d, gain: %d\n", delta, currentgain);
		}

}
