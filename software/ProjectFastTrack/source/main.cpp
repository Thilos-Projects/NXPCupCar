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

#include "ThilosAddons/TaskManager.h"
}


void blinkLed0(){
	static bool modus = false;
	mLeds_Write(kMaskLed1, modus ? kLedOn : kLedOff);
	modus = !modus;
	printf("Modus in LED0: %d\n",modus);
}
void blinkLed1(){
	static bool modus = false;
	mLeds_Write(kMaskLed2, modus ? kLedOn : kLedOff);
	modus = !modus;

	printf("Modus in LED1: %d\n",modus);
}

int main(){
	printf("Hello Car\n");
	mCpu_Setup();
	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	mTimer_EnableHBridge();

	mTimer_SetMotorDuty(0.4f, 0.4f);

	Setup();

	taskHandle* ledHandle0 = getTaskHandle();
	ledHandle0->delay = 1000 / minTaskTimeInMs;
	ledHandle0->functionToCall = &blinkLed0;
	ledHandle0->active = true;

	taskHandle* ledHandle1 = getTaskHandle();
	ledHandle1->delay = 500 / minTaskTimeInMs;
	ledHandle1->functionToCall = &blinkLed1;
	ledHandle1->active = true;

	for(UInt32 i = 0; true; i++){
		Update();
	}

	printf("Tshöööööööö\n");

	return 0;
}
