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

#include <ThilosAddons/Sceduler.h>

void blinkLed0(Sceduler::taskHandle* self){
	static bool modus = false;
	mLeds_Write(kMaskLed1, modus ? kLedOn : kLedOff);
	modus = !modus;
}
void blinkLed1(Sceduler::taskHandle* self){
	static bool modus = false;
	mLeds_Write(kMaskLed2, modus ? kLedOn : kLedOff);
	modus = !modus;
}

int main(){
	printf("Hello Car\n");
	mCpu_Setup();
	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	//enable Motor
	mTimer_EnableHBridge();
	//set both motors to 0.4 drive Voltage
	mTimer_SetMotorDuty(0.4f, 0.4f);

	//start sceduler
	Sceduler::Setup();

	//Task Definitionen für Led Blinken
	Sceduler::getTaskHandle(&blinkLed0, 1000);
	Sceduler::getTaskHandle(&blinkLed1, 500);

	//motor aus nach 60 sekunden
	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		mTimer_SetMotorDuty(0.0f, 0.0f);
		self->isFree = true;
	}, 60000, true, false);

	for(;;){
		//Sceduler frameCall
		Sceduler::Update();
	}


	return 0;
}
