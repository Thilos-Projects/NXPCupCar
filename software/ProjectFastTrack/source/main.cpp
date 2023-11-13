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
#include "Modules/mTimer.h"
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
	mCpu_Setup();
	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	mLeds_Write(kMaskLed1,kLedOn);

	mTimer_EnableHBridge();

	mTimer_SetMotorDuty(0.4f, 0.4f);

	//bool up = true;
	//float test = 0;

	for(UInt32 i = 0; true; i++){

		if(mTimer_GetFaultMoteurLeft())
			printf("motor left fault");
		if(mTimer_GetFaultMoteurRight())
			printf("motor right fault");
		mLeds_Write(kMaskLed1,kLedOff);
		for(UInt32 j = 0; j < 2500000; j++);
		mLeds_Write(kMaskLed1,kLedOn);
		for(UInt32 j = 0; j < 2500000; j++);

		//if(up)
			//test+=0.1f;
		//else
			//test-=0.1f;

		//if(up && test > 0.9f)
		//	up = false;
		//if(!up && test < -0.9f)
		//	up = true;

		//mTimer_SetMotorDuty(test, 3/8.0f);

	}

	printf("Tshöööööööö\n");

	return 0;
}
