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



//Camera defaults
#define EXPOSURE_INIT 0x002200
#define EXPOSURE_STEP 0x100
#define GAIN_INIT 0x280
#define GAIN_STEP 0x040
#define DEFAULT_LEVEL 3

#define MIPI_REG_PHYClkCtl		0x0056
#define MIPI_REG_PHYData0Ctl	0x0058
#define MIPI_REG_PHYData1Ctl	0x005A
#define MIPI_REG_PHYData2Ctl	0x005C
#define MIPI_REG_PHYData3Ctl	0x005E
#define MIPI_REG_PHYTimDly		0x0060
#define MIPI_REG_PHYSta			0x0062
#define MIPI_REG_CSIStatus		0x0064
#define MIPI_REG_CSIErrEn		0x0066
#define MIPI_REG_MDLSynErr		0x0068
#define MIPI_REG_FrmErrCnt		0x0080
#define MIPI_REG_MDLErrCnt		0x0090


//Struct

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
int number_observations = 7;


void mipi_clear_error(void){
	MipiBridgeRegWrite(MIPI_REG_CSIStatus,0x01FF); // clear error
	MipiBridgeRegWrite(MIPI_REG_MDLSynErr,0x0000); // clear error
	MipiBridgeRegWrite(MIPI_REG_FrmErrCnt,0x0000); // clear error
	MipiBridgeRegWrite(MIPI_REG_MDLErrCnt, 0x0000); // clear error

  	MipiBridgeRegWrite(0x0082,0x00);
  	MipiBridgeRegWrite(0x0084,0x00);
  	MipiBridgeRegWrite(0x0086,0x00);
  	MipiBridgeRegWrite(0x0088,0x00);
  	MipiBridgeRegWrite(0x008A,0x00);
  	MipiBridgeRegWrite(0x008C,0x00);
  	MipiBridgeRegWrite(0x008E,0x00);
  	MipiBridgeRegWrite(0x0090,0x00);
}

void mipi_show_error_info(void){

	alt_u16 PHY_status, SCI_status, MDLSynErr, FrmErrCnt, MDLErrCnt;

	PHY_status = MipiBridgeRegRead(MIPI_REG_PHYSta);
	SCI_status = MipiBridgeRegRead(MIPI_REG_CSIStatus);
	MDLSynErr = MipiBridgeRegRead(MIPI_REG_MDLSynErr);
	FrmErrCnt = MipiBridgeRegRead(MIPI_REG_FrmErrCnt);
	MDLErrCnt = MipiBridgeRegRead(MIPI_REG_MDLErrCnt);
	printf("PHY_status=%xh, CSI_status=%xh, MDLSynErr=%xh, FrmErrCnt=%xh, MDLErrCnt=%xh\r\n", PHY_status, SCI_status, MDLSynErr,FrmErrCnt, MDLErrCnt);
}

void mipi_show_error_info_more(void){
    printf("FrmErrCnt = %d\n",MipiBridgeRegRead(0x0080));
    printf("CRCErrCnt = %d\n",MipiBridgeRegRead(0x0082));
    printf("CorErrCnt = %d\n",MipiBridgeRegRead(0x0084));
    printf("HdrErrCnt = %d\n",MipiBridgeRegRead(0x0086));
    printf("EIDErrCnt = %d\n",MipiBridgeRegRead(0x0088));
    printf("CtlErrCnt = %d\n",MipiBridgeRegRead(0x008A));
    printf("SoTErrCnt = %d\n",MipiBridgeRegRead(0x008C));
    printf("SynErrCnt = %d\n",MipiBridgeRegRead(0x008E));
    printf("MDLErrCnt = %d\n",MipiBridgeRegRead(0x0090));
    printf("FIFOSTATUS = %d\n",MipiBridgeRegRead(0x00F8));
    printf("DataType = 0x%04x\n",MipiBridgeRegRead(0x006A));
    printf("CSIPktLen = %d\n",MipiBridgeRegRead(0x006E));
}



bool MIPI_Init(void){
	bool bSuccess;


	bSuccess = oc_i2c_init_ex(I2C_OPENCORES_MIPI_BASE, 50*1000*1000,400*1000); //I2C: 400K
	if (!bSuccess)
		printf("failed to init MIPI- Bridge i2c\r\n");

    usleep(50*1000);
    MipiBridgeInit();

    usleep(500*1000);

    MipiCameraInit();
    MIPI_BIN_LEVEL(DEFAULT_LEVEL);

 	usleep(1000);
	return bSuccess;
}


int to_hex_code (int r, int g, int b){
	return (r<<16) + (g<<8) +  b;
}


struct observation* initialize_observations(){

	struct observation* arr = malloc(number_observations * sizeof(struct observation));

	//Initialize green ball parameters
	arr[0].code = 'G';
	arr[0].mask_bottom = to_hex_code(50,80,80);
	arr[0].mask_top = to_hex_code(40,255,255);
	arr[0].threshold = 192;


	//Initialize yellow ball parameters
	arr[1].code = 'Y';
	arr[1].mask_bottom = to_hex_code(20,90,60);
	arr[1].mask_top = to_hex_code(40,255,255);
	arr[1].threshold = 192;

	//Initialize pink ball parameters
	arr[2].code = 'P';
	arr[2].mask_bottom = to_hex_code(155,70,80);
	arr[2].mask_top = to_hex_code(165,255,255);
	arr[2].threshold = 192;

	//Initialize white ball parameters
	arr[3].code = 'W';
	arr[3].mask_bottom = to_hex_code(0,180,100);
	arr[3].mask_top = to_hex_code(180,255,255);
	arr[3].threshold = 192;

	//Initialize black ball parameters
	arr[4].code = 'K';
	arr[4].mask_bottom = to_hex_code(0,0,0);
	arr[4].mask_top = to_hex_code(120,120,120);
	arr[4].threshold = 192;

	//Initialize Red low ball parameters
	arr[5].code = 'R';
	arr[5].mask_bottom = to_hex_code(0,80,80);
	arr[5].mask_top = to_hex_code(10,255,255);
	arr[5].threshold = 192;

	//Initialize Red high ball parameters
	arr[6].code = 'S';
	arr[6].mask_bottom = to_hex_code(170,80,80);
	arr[6].mask_top = to_hex_code(180,255,255);
	arr[6].threshold = 192;

	return arr;
}

char observation_buf[65];
#define UART_DEBUG 1

void print_observations (FILE* fp, struct observation* obs) {

	for (int i = 0; i < number_observations-1 ; i++){
		sprintf(observation_buf, "%c %x %x %x %x %x %x %x ,", obs[i].code, obs[i].com_x,
				obs[i].com_y, obs[i].mass, obs[i].bb_left, obs[i].bb_top,
				obs[i].bb_right, obs[i].bb_bottom);
		if (UART_DEBUG){
			printf ("%s", observation_buf);
			while (getchar() != 'A'){
				usleep(1);
			}
		} else{
			fwrite (observation_buf, strlen(observation_buf), 1, fp);

			while (getc(fp) != 'A'){
				usleep(1);
			}
		}
	}

	//Final observation
	sprintf(observation_buf, "%c %x %x %x %x %x %x %x\n", obs[number_observations-1].com_x,
			obs[number_observations-1].com_y, obs[number_observations-1].mass,
			obs[number_observations-1].bb_left, obs[number_observations-1].bb_top,
			obs[number_observations-1].bb_right, obs[number_observations-1].bb_bottom);

			if (UART_DEBUG){
				printf (observation_buf);
				while (getchar() != 'A'){
					usleep(1);
				}
			} else{
				fwrite (observation_buf, strlen(observation_buf), 1, fp);

				while (getc(fp) != 'A'){
					usleep(1);
				}
			}

}


int main()
{

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


//////////////////////////////////////////////////////////
	alt_u8  manual_focus_step = 10;
	alt_u16  current_focus = 300;
	alt_u32 exposureTime = EXPOSURE_INIT;
	alt_u16 gain = GAIN_INIT;

	OV8865SetExposure(exposureTime);
	OV8865SetGain(gain);
	Focus_Init();


////////////////////////////////////////////////////////////////
	//Color filter setup
  //  IOWR(COLOR_FILTER_BASE, THRESHOLD_LOW, 0x323050);
  //  IOWR(COLOR_FILTER_BASE, THRESHOLD_HIGH, 0x46ffff);
	IOWR(COLOR_FILTER_0_BASE, THRESHOLD_LOW, 0x0000b0);
	IOWR(COLOR_FILTER_0_BASE, THRESHOLD_HIGH, 0xffffff);
	IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, 1);

	//Linear blue disable
	IOWR(LINEAR_BLUR_0_BASE, BLUR_ENABLED, 1);

	//COM counter setup
	IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, 1);
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
	int observation_idx = 0;

  while(1){

       // touch KEY0 to trigger Auto focus
	   if((IORD(KEY_BASE,0)&0x03) == 0x02){

    	   current_focus = Focus_Window(320,240);
       }

	   //Update observation values

	   //Update setting for frame
		IOWR(COLOR_FILTER_0_BASE, THRESHOLD_LOW, observations[observation_idx].mask_bottom);
		IOWR(COLOR_FILTER_0_BASE, THRESHOLD_HIGH, observations[observation_idx].mask_top);
		IOWR(COM_COUNTER_0_BASE, THRESHOLD_GATE, observations[observation_idx].threshold);

		//Wait two frame times
		usleep(1000000);
		printf("Collecting %c \n", observations[observation_idx].code );

		//Collect results
		observations[observation_idx].com_x = IORD(COM_COUNTER_0_BASE, COM_X);
		observations[observation_idx].com_y = IORD(COM_COUNTER_0_BASE, COM_Y);
		observations[observation_idx].mass = IORD(COM_COUNTER_0_BASE, COM_MASS);
		observations[observation_idx].bb_left = IORD(COM_COUNTER_0_BASE, BB_LOW_X);
		observations[observation_idx].bb_top = IORD(COM_COUNTER_0_BASE, BB_LOW_Y);
		observations[observation_idx].bb_right = IORD(COM_COUNTER_0_BASE, BB_HIGH_X);
		observations[observation_idx].bb_bottom = IORD(COM_COUNTER_0_BASE, BB_HIGH_Y);

	   observation_idx++;
	   if (observation_idx == number_observations)observation_idx = 0;


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
