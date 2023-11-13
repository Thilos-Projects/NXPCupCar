//muss sein weil nur c fähig
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

//#include "Modules/mSpi.h"
//#include "Modules/mDac.h"
//#include "Modules/mAccelMagneto.h"
//#include "Modules/mGyro.h"
//#include "Modules/mTimer.h"
#include "Modules/mCpu.h"		//mCpu_Setup
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

int main(){
	printf("Hello Car\n");

	printf("Tshöööööööö\n");
	mCpu_Setup();
	mLeds_Setup();

	mLeds_Write(kMaskLed1,kLedOn);

	for(UInt32 i = 0; true; i++){

		mLeds_Write(kMaskLed1,kLedOff);
		for(UInt32 j = 0; j < 10000000; j++);
		mLeds_Write(kMaskLed1,kLedOn);
		for(UInt32 j = 0; j < 10000000; j++);
	}





	return 0;
}
