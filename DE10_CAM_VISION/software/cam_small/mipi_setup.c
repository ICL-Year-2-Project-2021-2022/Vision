
#include "mipi_setup.h"


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
	if (print){
	printf("PHY_status=%xh, CSI_status=%xh, MDLSynErr=%xh, FrmErrCnt=%xh, MDLErrCnt=%xh\r\n", PHY_status, SCI_status, MDLSynErr,FrmErrCnt, MDLErrCnt);
	}
}

void mipi_show_error_info_more(void){
	if (print){
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
}



bool MIPI_Init(void){
	bool bSuccess;


	bSuccess = oc_i2c_init_ex(I2C_OPENCORES_MIPI_BASE, 50*1000*1000,400*1000); //I2C: 400K
	if (!bSuccess & print)
		printf("failed to init MIPI- Bridge i2c\r\n");

    usleep(50*1000);
    MipiBridgeInit();

    usleep(500*1000);

    MipiCameraInit();
    MIPI_BIN_LEVEL(DEFAULT_LEVEL);

 	usleep(1000);
	return bSuccess;
}
