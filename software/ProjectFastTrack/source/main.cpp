
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
#include <TFT_Modules/Scheduler.h>
#include <TFT_Modules/CameraAnalysis.h>

#include "Pixy/Pixy2SPI_SS.h"

//Lokale Definitionen
Pixy2SPI_SS pixy;
CameraAnalysis::SingleRowAnalysis singleRowAnalysis_160;

//Task Definitionen
Scheduler::taskHandle* t_blinkLED;
Scheduler::taskHandle* t_motorStop;
Scheduler::taskHandle* t_cameraAlgorithm;

//Bennenungen für Programmstruktur
void pixySetup();
void cameraRowsSetup();

void Setup() {
	mCpu_Setup();

	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	mSpi_Setup();
	mSpi_Open();

	pixySetup();

	Scheduler::Setup();

	cameraRowsSetup();

	//Motor Setup (Motor Enable)
	mTimer_EnableHBridge();
}

void pixySetup(){
	pixy.init();
	pixy.getVersion();
	pixy.version->print();
	printf("HellO World: %ld\n",clock());
	pixy.setLED(0, 255, 0);
	//pixy.setLamp(1, 1);
	pixy.changeProg("video");
}

//Eine / Mehrere Zeilen können definiert + gewählt werden
void cameraRowsSetup() {
	singleRowAnalysis_160.Setup(&pixy, 160, 20, 5, 15, 0, 6, 130, 240);
	//ToDo: Hier können weitere Reihen analysiert werden
}

void cameraAlgorythmus_2(CameraAnalysis::SingleRowAnalysis* analysMethod){

	analysMethod->getImageRow();
	analysMethod->calculateSobelRow();
	analysMethod->calculateEdges();
	analysMethod->calculateTrackLines();
	analysMethod->calculateValidTracks();

	analysMethod->printTrackLines();

	if(analysMethod->lines[0].isEmpty()) {
		// TODO: NO Track border -> do nothing?
	} else if(analysMethod->lines[1].isEmpty()) {
		// Exactly 1 TrackBorder
		// TODO: Move to config / Constant
		uint16_t car_center = 316 / 2 + 7;
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
			stellwinkel *= 8.0f;
			printf("Neuer Stellwinkel: %f", stellwinkel);
			mTimer_SetServoDuty(0, stellwinkel);
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
			stellwinkel *= 2.0f;
			printf("Neuer Stellwinkel: %f", stellwinkel);
			mTimer_SetServoDuty(0, stellwinkel);
		}
	}
}


void defineTasks() {

	t_blinkLED = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		static bool state = false;
		mLeds_Write(kMaskLed1,state ? kLedOff : kLedOn);
		state = !state;
	}, 500);

	t_motorStop = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		mTimer_SetMotorDuty(0, 0);
		self->active = false;
	}, 30000, true, false);

	t_cameraAlgorithm = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){cameraAlgorythmus_2(&singleRowAnalysis_160); }, 100);

}

int main(){
	printf("Hello Car\n");

	Setup();
	defineTasks();

	//ToDo: Geschwindigkeitssteuerung muss noch richtig gesteuert werden!
	mTimer_SetMotorDuty(0.4f, 0.4f);


	for(UInt32 i = 0; true; i++){
		Scheduler::Update();
	}

	return 0;
}
