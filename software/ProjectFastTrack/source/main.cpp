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
#include "Modules/mAd.h"
//#include "Modules/mDelay.h"
//#include "Modules/mRS232.h"
//#include "Modules/mVL6180x.h"

//#include "Applications/gInput.h"
//#include "Applications/gCompute.h"
//#include "Applications/gOutput.h"
}

#include <TFT_Modules/Sceduler.h>

void DemoFahrt(Sceduler::taskHandle* self){
	typedef void (*stateFunc)(void);

	static uint8_t mState = 0;
	static bool warteDone = true;
	static Sceduler::taskHandle* warteHandler02Sec = Sceduler::getTaskHandle([&warteDone](Sceduler::taskHandle* self){warteDone = true; self->active = false;}, 2000, false, false);
	static Sceduler::taskHandle* warteHandler05Sec = Sceduler::getTaskHandle([&warteDone](Sceduler::taskHandle* self){warteDone = true; self->active = false;}, 5000, false, false);
	static Sceduler::taskHandle* warteHandler10Sec = Sceduler::getTaskHandle([&warteDone](Sceduler::taskHandle* self){warteDone = true; self->active = false;}, 10000, false, false);

	static stateFunc mStates[14] = {
			//warte 5 sec
			[warteHandler05Sec, &mState](){
				setTimerActive(warteHandler05Sec);
				mTimer_SetMotorDuty(0.0f, 0.0f);
				mTimer_SetServoDuty(0, 0);
			},
			//fahre 2 sec vor
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				mTimer_SetMotorDuty(0.4f, 0.4f);
				mTimer_SetServoDuty(0, 0);
			},
			//warete 2 sec
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				mTimer_SetMotorDuty(0.0f, 0.0f);
				mTimer_SetServoDuty(0, 0);
			},
			//fahre 2 sec rück
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				mTimer_SetMotorDuty(-0.4f, -0.4f);
				mTimer_SetServoDuty(0, 0);
			},
			//warte 10 sec
			[warteHandler10Sec, &mState](){
				setTimerActive(warteHandler10Sec);
				mTimer_SetMotorDuty(0.0f, 0.0f);
				mTimer_SetServoDuty(0, 0);
			},
			//fahre kurve 5 sec rechts
			[warteHandler05Sec, &mState](){
				setTimerActive(warteHandler05Sec);
				mTimer_SetMotorDuty(0.4f, 0.4f);
				mTimer_SetServoDuty(0, 1);
			},
			//warte 5 sec
			[warteHandler05Sec, &mState](){
				setTimerActive(warteHandler05Sec);
				mTimer_SetMotorDuty(0.0f, 0.0f);
				mTimer_SetServoDuty(0, 0);
			},
			//fahre kurve 2 sec links
			[warteHandler05Sec, &mState](){
				setTimerActive(warteHandler05Sec);
				mTimer_SetMotorDuty(0.4f, 0.4f);
				mTimer_SetServoDuty(0, -1);
			},
			//warte 10 sec
			[warteHandler10Sec, &mState](){
				setTimerActive(warteHandler10Sec);
				mTimer_SetMotorDuty(0.0f, 0.0f);
				mTimer_SetServoDuty(0, 0);
			},
			//fahre 2 sec vor schnell
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				mTimer_SetMotorDuty(0.4f, 0.4f);
				mTimer_SetServoDuty(0, 0);
			},
			//fahre kurve 2 sec rechts langsam
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				mTimer_SetMotorDuty(0.4f, 0.4f);
				mTimer_SetServoDuty(0, 1);
			},
			//fahre 2 sec vor schnell
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				mTimer_SetMotorDuty(0.4f, 0.4f);
				mTimer_SetServoDuty(0, 0);
			},
			//fahre kurve 2 sec rechts langsam
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				mTimer_SetMotorDuty(0.4f, 0.4f);
				mTimer_SetServoDuty(0, 1);
			},
			//repeat
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				mTimer_SetMotorDuty(0.0f, 0.0f);
				mTimer_SetServoDuty(0, 0);
			}
	};

	if(!warteDone) return;

	mStates[mState]();
	warteDone = false;
	mState++;
	if(mState > 13)
		mState = 0;

	//warte 5 sec
	//fahre 2 sec vor
	//warete 2 sec
	//fahre 2 sec rück
	//warte 10 sec
	//fahre kurve 5 sec rechts
	//warte 5 sec
	//fahre kurve 2 sec links
	//warte 10 sec
	//fahre 2 sec vor schnell
	//fahre kurve 2 sec rechts langsam
	//fahre 2 sec vor schnell
	//fahre kurve 2 sec rechts langsam
	//repeat
}

void ledBlink(LedMaskEnum led){
	static bool led1 = false;
	static bool led2 = false;
	static bool led3 = false;
	static bool led4 = false;
	switch(led){
	case kMaskLed1:
		mLeds_Write(kMaskLed1, led1 ? kLedOn : kLedOff);
		led1 = !led1;
		break;
	case kMaskLed2:
		mLeds_Write(kMaskLed2, led2 ? kLedOn : kLedOff);
		led2 = !led2;
		break;
	case kMaskLed3:
		mLeds_Write(kMaskLed3, led3 ? kLedOn : kLedOff);
		led3 = !led3;
		break;
	case kMaskLed4:
		mLeds_Write(kMaskLed4, led4 ? kLedOn : kLedOff);
		led4 = !led4;
		break;
	}
}

int main(){
	printf("Hello Car\n");
	mCpu_Setup();
	mLeds_Setup();

	mAd_Setup();

	mTimer_Setup();
	mTimer_Open();

	mTimer_EnableHBridge();

	Sceduler::Setup();

	Sceduler::getTaskHandle(&DemoFahrt, 1000, true, true);
	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		if(mTimer_GetFaultMoteurLeft()) mLeds_Write(kMaskLed1, kLedOn);
		if(mTimer_GetFaultMoteurRight()) mLeds_Write(kMaskLed2, kLedOn);
	}, 2000, true, true);
	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		printf("ADC Read: \tBatteryVoltage: %f \tPotis: %f, %f \tHBridge: %f, %f\n",
				mAd_Read(ADCInputEnum::kUBatt),
				mAd_Read(ADCInputEnum::kPot1),
				mAd_Read(ADCInputEnum::kPot2),
				mAd_Read(ADCInputEnum::kIHBridgeLeft),
				mAd_Read(ADCInputEnum::kIHBridgeRight));
	}, 2000, true, true);
	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){ledBlink(LedMaskEnum::kMaskLed3);}, 500, true, true);
	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){ledBlink(LedMaskEnum::kMaskLed4);}, 1000, true, true);

	for(;;){
		//Sceduler frameCall
		Sceduler::Update();
	}


	return 0;
}
