//muss sein weil nur c fähig
extern "C"
{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

//hier die SD-card
//BuildError: #include "sdcard/sdmmc_config.h"

#include "Modules/mSpi.h"
#include "Modules/mDac.h"
#include "Modules/mAccelMagneto.h"
#include "Modules/mGyro.h"
#include "Modules/mTimer.h"
#include "Modules/mCpu.h"
#include "Modules/mSwitch.h"
#include "Modules/mLeds.h"
#include "Modules/mAd.h"
#include "Modules/mDelay.h"
#include "Modules/mRS232.h"
#include "Modules/mVL6180x.h"

#include "Applications/gInput.h"
#include "Applications/gCompute.h"
#include "Applications/gOutput.h"
}

#include <TFT_Modules/Sceduler.h>
#include <ThilosAddons/MotorControll.h>

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
				MotorControl::setSpeed(0.0f, 0.0f);
				MotorControl::setServo(0);
			},
			//fahre 2 sec vor
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				MotorControl::setSpeed(0.4f, 0.4f);
				MotorControl::setServo(0);
			},
			//warete 2 sec
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				MotorControl::setSpeed(0.0f, 0.0f);
				MotorControl::setServo(0);
			},
			//fahre 2 sec rück
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				MotorControl::setSpeed(-0.4f, -0.4f);
				MotorControl::setServo(0);
			},
			//warte 10 sec
			[warteHandler10Sec, &mState](){
				setTimerActive(warteHandler10Sec);
				MotorControl::setSpeed(0.0f, 0.0f);
				MotorControl::setServo(0);
			},
			//fahre kurve 5 sec rechts
			[warteHandler05Sec, &mState](){
				setTimerActive(warteHandler05Sec);
				MotorControl::setSpeed(0.4f, 0.4f);
				MotorControl::setServo(1);
			},
			//warte 5 sec
			[warteHandler05Sec, &mState](){
				setTimerActive(warteHandler05Sec);
				MotorControl::setSpeed(0.0f, 0.0f);
				MotorControl::setServo(0);
			},
			//fahre kurve 2 sec links
			[warteHandler05Sec, &mState](){
				setTimerActive(warteHandler05Sec);
				MotorControl::setSpeed(0.4f, 0.4f);
				MotorControl::setServo(-1);
			},
			//warte 10 sec
			[warteHandler10Sec, &mState](){
				setTimerActive(warteHandler10Sec);
				MotorControl::setSpeed(0.0f, 0.0f);
				MotorControl::setServo(0);
			},
			//fahre 2 sec vor schnell
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				MotorControl::setSpeed(0.4f, 0.4f);
				MotorControl::setServo(0);
			},
			//fahre kurve 2 sec rechts langsam
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				MotorControl::setSpeed(0.4f, 0.4f);
				MotorControl::setServo(1);
			},
			//fahre 2 sec vor schnell
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				MotorControl::setSpeed(0.4f, 0.4f);
				MotorControl::setServo(0);
			},
			//fahre kurve 2 sec rechts langsam
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				MotorControl::setSpeed(0.4f, 0.4f);
				MotorControl::setServo(1);
			},
			//repeat
			[warteHandler02Sec, &mState](){
				setTimerActive(warteHandler02Sec);
				MotorControl::setSpeed(0.0f, 0.0f);
				MotorControl::setServo(0);
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

	mSpi_Setup();
	mSpi_Open();

	mDac_Setup();
	mDac_Open();

	mAccelMagneto_Setup();
	mAccelMagneto_Open();

	mGyro_Setup();
	mGyro_Open();

	mTimer_Setup();
	mTimer_Open();

	mSwitch_Setup();
	mSwitch_Open();

	mLeds_Setup();
	mLeds_Open();

	mAd_Setup();

	mDelay_Setup();
	mDelay_Open();

	mRs232_Setup();
	mRs232_Open();

	mVL6180x_Setup();
	mVL6180x_Open();

	gInput_Setup();
	gCompute_Setup();
	gOutput_Setup();

	mTimer_EnableHBridge();

	Sceduler::Setup();

	MotorControl::Setup();

	//Sceduler::getTaskHandle(&DemoFahrt, 1000, true, true);
	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		if(mTimer_GetFaultMoteurLeft()) mLeds_Write(kMaskLed1, kLedOn);
		if(mTimer_GetFaultMoteurRight()) mLeds_Write(kMaskLed2, kLedOn);
	}, 2000, true, true);
	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		ledBlink(kMaskLed3);
	}, 2000, true, true);
	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		float a = mAd_Read(ADCInputEnum::kUBatt);
		float b = mAd_Read(ADCInputEnum::kPot1);
		float c = mAd_Read(ADCInputEnum::kPot2);
		float d = mAd_Read(ADCInputEnum::kIHBridgeLeft);
		float e = mAd_Read(ADCInputEnum::kIHBridgeRight);
		printf("ADC Read: \tBatteryVoltage: %e \tPotis: %e, %e \tHBridge: %e, %e\n", (int16_t)a, (int16_t)b, (int16_t)c, (int16_t)d, (int16_t)e);
	}, 2000, true, true);
	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		float a,b;
		MotorControl::getSpeed(&a, &b);
		printf("Speed %d, %d\n", (int32_t)(a*100), (int32_t)(b*100));
	}, 500, true, true);

	//MotorControl::setServo(1);
	MotorControl::setSpeed(0.55f, 0.0f);

	for(;;){
		//Sceduler frameCall
		Sceduler::Update();
	}


	return 0;
}
