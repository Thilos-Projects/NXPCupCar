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

//hier die SD-card
//BuildError: #include "sdcard/sdmmc_config.h"

#include "Modules/mSpi.h"
#include "Modules/mTimer.h"
#include "Modules/mCpu.h"
#include "Modules/mLeds.h"
#include "Modules/mAd.h"
}
#include <TFT_Modules/Scheduler.h>
#include <TFT_Modules/CameraAnalysis.h>

#include "Pixy/Pixy2SPI_SS.h"

#define TIME_PER_FRAME 17

#define SERVO_STEERING_OFFSET -0.2f
#define SPEED_MIN 0.4f
#define SPEED_MAX 0.6f
#define SPEED_ADJUST_TIME 2000.0f
#define MAX_CENTER_DIFF_FOR_SPEED_UP 5

//Lokale Definitionen
Pixy2SPI_SS pixy;
CameraAnalysis::SingleRowAnalysis singleRowAnalysis_180;
CameraAnalysis::SingleRowAnalysis singleRowAnalysis_50;

//Task Definitionen
Scheduler::taskHandle* t_blinkLED;
Scheduler::taskHandle* t_motorStop;
Scheduler::taskHandle* t_generalCamera;

Scheduler::taskHandle* t_cameraAlgorithm;
Scheduler::taskHandle* t_speedControl;



//Bennenungen für Programmstruktur
void pixySetup();
void cameraRowsSetup();
int16_t getBestTrackIndexFromMultipleTracks(CameraAnalysis::SingleRowAnalysis* singleRow);
bool currentRowAnalysis_160(float* steeringAngle);

float destinationSpeed = 0.0f;


void Setup() {
	mCpu_Setup();

	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	mSpi_Setup();
	mSpi_Open();

	mAd_Setup();
	mAd_Open();

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
	singleRowAnalysis_180.Setup(&pixy, 180, 40, 0, 6, 158);
	singleRowAnalysis_50.Setup(&pixy, 50, 40, 0, 6, 158);
}


//Auslesen der Kamera, Sobel und Kanten der übergebenen Reihen!
void generalCameraTask(CameraAnalysis::SingleRowAnalysis** rowsToDo, uint8_t length) {
	for(uint8_t i = 0; i<length; i++) {
		rowsToDo[i]->getImageRow();
		rowsToDo[i]->calculateSobelRow();
		rowsToDo[i]->findBlankArea();
		//rowsToDo[i]->calculateEdges();
	}
}


void lenkung() {
	//mLeds_Write(kMaskLed2,kLedOff);

	singleRowAnalysis_180.findBlankArea();
	singleRowAnalysis_50.findBlankArea();

	float steeringAngle = (float)singleRowAnalysis_180.trackCenter - (float)singleRowAnalysis_180.centerPixel;
	steeringAngle /= 79.0f;
	steeringAngle *= steeringAngle;

	float steeringFactor = 3.0f + (destinationSpeed - 0.4f) * ((mAd_Read(ADCInputEnum::kPot2) + 1) / 2) * 30.0f;

	steeringAngle *= steeringFactor;

	static float minSteeringAngle = 9000.0f;
	static float maxSteeringAngle = -9000.0f;


	if(singleRowAnalysis_180.trackCenter < singleRowAnalysis_180.centerPixel) {
		mTimer_SetServoDuty(0, -steeringAngle + SERVO_STEERING_OFFSET);

		if (-steeringAngle > maxSteeringAngle) {
			maxSteeringAngle = -steeringAngle;
		}
		if (-steeringAngle < minSteeringAngle) {
			minSteeringAngle = -steeringAngle;
		}
	}
	else {
		mTimer_SetServoDuty(0, steeringAngle + SERVO_STEERING_OFFSET);

		if (steeringAngle > maxSteeringAngle) {
			maxSteeringAngle = steeringAngle;
		}
		if (steeringAngle < minSteeringAngle) {
			minSteeringAngle = steeringAngle;
		}
	}

}

void adjustSpeed() {
	singleRowAnalysis_180.calculateTrackDifferences();

	int16_t blubb = (int16_t)(((mAd_Read(ADCInputEnum::kPot1) + 1) / 2) * 130.0f) + 20;

	if (abs((int16_t)singleRowAnalysis_180.trackCenter - (int16_t)singleRowAnalysis_180.centerPixel) < blubb) {
		// Increase speed on straight tracks
		destinationSpeed += (TIME_PER_FRAME / SPEED_ADJUST_TIME) * (SPEED_MAX-SPEED_MIN); // TIME_PER_FRAME * MAXIMUM_RAISE_PER_SECOND
		if (destinationSpeed > SPEED_MAX) {
			destinationSpeed = SPEED_MAX;
		}
	} else {
		// Reset to "turn speed" in turns
		destinationSpeed = SPEED_MIN;
	}
}


void defineTasks() {

	t_blinkLED = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		static bool state = false;
		mLeds_Write(kMaskLed1,state ? kLedOff : kLedOn);
		state = !state;
	}, 500, false, false);

	t_motorStop = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		mTimer_SetMotorDuty(0, 0);
		self->active = false;
	}, 90000, false, false);


	t_generalCamera = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		static CameraAnalysis::SingleRowAnalysis* usedCameraRows[] =  {
			&singleRowAnalysis_180,
			&singleRowAnalysis_50
		};
		generalCameraTask(usedCameraRows, 1);
	}, min(17, TIME_PER_FRAME));


	t_cameraAlgorithm = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		lenkung();
		adjustSpeed();
		mTimer_SetMotorDuty(destinationSpeed, destinationSpeed);
	}, TIME_PER_FRAME);

}

int main(){
	printf("Hello Car\n");

	Setup();
	defineTasks();

	for(UInt32 i = 0; true; i++){
		Scheduler::Update();
	}

	return 0;
}
