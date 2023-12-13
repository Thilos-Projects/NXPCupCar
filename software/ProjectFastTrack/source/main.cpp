void pixyUsleep(int useconds) {
	int i = 0;
	int j = 0;
	for (i = 0; i < useconds; i++)
	{
		j++;
	}
}


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
#include "TFT_Modules/Sceduler.h"
#include "TFT_Modules/TrackAnalyse.h"

#include "Pixy/Pixy2SPI_SS.h"

Pixy2SPI_SS pixy;

void Setup(){
	pixy.init();
	pixy.getVersion();
	pixy.version->print();
	printf("HellO World: %ld\n",clock());
	pixy.setLED(0, 255, 0);
	//pixy.setLamp(1, 1);
	pixy.changeProg("video");
}

void cameraAlgorythmus_2(TrackAnalys::TrackAnalyse* analysMethod){

	analysMethod->callAll();

	if(analysMethod->lines[0].isEmpty()) {
		// TODO: NO Track border -> do nothing?
	} else if(analysMethod->lines[1].isEmpty()) {
		// Exactly 1 TrackBorder
		// TODO: Move to config / Constant
		uint16_t car_center = 316 / 2;
		uint16_t destination_center = 0;
		uint16_t expected_track_width = 250;
		if (analysMethod->lines[0].centerIndex < car_center) {
			// TODO
			destination_center = analysMethod->lines[0].centerIndex + (expected_track_width / 2);
		} else {
			// TODO
			destination_center = analysMethod->lines[0].centerIndex - (expected_track_width / 2);
		}
		if(destination_center != 0){
			float stellwinkel = destination_center;
			stellwinkel /= 158.0f;
			stellwinkel -= 1.0f;
			printf("Neuer Stellwinkel: %f", stellwinkel * 6);
		}
	} else {
		uint16_t center = 0;

		for(int i = 1; i < 317; i++){
			uint16_t index = 158 + (i/2) * (i%2==0?-1:1);
			if(!analysMethod->tracks[index].isEmpty()){
				center = index;
				break;
			}
		}
		if(center != 0){
			float stellwinkel = center;
			stellwinkel /= 158.0f;
			stellwinkel -= 1.0f;
			printf("Neuer Stellwinkel: %f", stellwinkel * 2);
		}
	}
}

TrackAnalys::TrackAnalyse trackAnalys_0;

int main(){
	printf("Hello Car\n");

	mCpu_Setup();

	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	mSpi_Setup();
	mSpi_Open();

	Setup();
	trackAnalys_0.Setup(&pixy, 160, 20, 5, 15, 0, 6, 130, 240);

	mLeds_Write(kMaskLed1,kLedOn);

	mTimer_EnableHBridge();

	mTimer_SetMotorDuty(0.4f, 0.4f);

	Sceduler::Setup();

	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		static bool state = false;
		mLeds_Write(kMaskLed1,state ? kLedOff : kLedOn);
		state = !state;
	}, 500);

	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		mTimer_SetMotorDuty(0, 0);
		self->active = false;
	}, 30000, true, false);

	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){cameraAlgorythmus_2(&trackAnalys_0); }, 100);

	for(UInt32 i = 0; true; i++){
		Sceduler::Update();
	}

	return 0;
}
