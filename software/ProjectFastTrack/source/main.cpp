//muss sein weil nur c fähig

void pixyUsleep(int useconds)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < useconds; i++)
	{
		j++;
	}
}

extern "C"
{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "MK64F12.h"
//#include "fsl_debug_console.h"

//hier die SD-card
//BuildError: #include "sdcard/sdmmc_config.h"

#include "Modules/mSpi.h"
//#include "Modules/mDac.h"
//#include "Modules/mAccelMagneto.h"
//#include "Modules/mGyro.h"
#include "Modules/mTimer.h"
#include "Modules/mCpu.h"
//#include "Modules/mSwitch.h"
#include "Modules/mLeds.h"
//#include "Modules/mAd.h"
//#include "Modules/mDelay.h"
//#include "Modules/mRS232.h"
//#include "Modules/mVL6180x.h"

//#include "Applications/gInput.h"
//#include "Applications/gCompute.h"
//#include "Applications/gOutput.h"
}

#include <TFT_Modules/Sceduler.h>
#include "Pixy/Pixy2SPI_SS.h"

int main(){
	printf("Hello Car\n");

	mCpu_Setup();

	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	mSpi_Setup();
	mSpi_Open();

	Pixy2SPI_SS pixy;
	pixy.init();
	pixy.getVersion();
	pixy.version->print();
	printf("HellO World: %ld\n",clock());
	pixy.setLED(0, 255, 0);
	//pixy.setLamp(1, 1);
	pixy.changeProg("video");

	Pixy2Video<Link2SPI_SS> pixyVid =  pixy.video;

	mLeds_Write(kMaskLed1,kLedOn);

	mTimer_EnableHBridge();

	mTimer_SetMotorDuty(0.4f, 0.4f);

	uint8_t colorDataBuffer[256];

	for(UInt32 i = 0; true; i++){
		mLeds_Write(kMaskLed1,kLedOff);
		for(UInt32 j = 0; j < 2500000; j++);
		mLeds_Write(kMaskLed1,kLedOn);
		for(UInt32 j = 0; j < 2500000; j++);

		for(int j = 1; j < 16; j++){
			pixyVid.getGrayRect(1, j, 16, j+1, 1, 1, colorDataBuffer, false);
			printf("%d", colorDataBuffer[0]);
			for(int j = 1; j < min(15, 255); j++) printf(",\t%d", colorDataBuffer[j]);
			printf("\n");
		}
		printf("\n\n\n\n");
	}

	return 0;
}
