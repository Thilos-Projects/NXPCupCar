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
#include "Modules/mSwitch.h"

}
#include <TFT_Modules/Scheduler.h>
#include <TFT_Modules/CameraAnalysis.h>
#include "TFT_Modules/ControlConfigStruct.h"

uint8_t controlConfigsLength;
ControlConfig controlConfigs[10];
ControlConfig* currentConfig;
#include "TFT_Modules/ControlConfigs.h"

#include "Pixy/Pixy2SPI_SS.h"

//Lokale Definitionen
Pixy2SPI_SS pixy;
uint8_t rowAnalysisLength = 0;
CameraAnalysis::SingleRowAnalysis singleRowAnalysis[2]; // Should be maximum size of row configs

//Task Definitionen
Scheduler::taskHandle* t_testMotorButton;
Scheduler::taskHandle* t_motorStop;
Scheduler::taskHandle* t_generalCamera;

Scheduler::taskHandle* t_cameraAlgorithm;
Scheduler::taskHandle* t_speedControl;

//Bennenungen für Programmstruktur
void pixySetup();
void cameraRowsSetup();
int16_t getBestTrackIndexFromMultipleTracks(CameraAnalysis::SingleRowAnalysis* singleRow);
// TODO: Refactor
bool currentRowAnalysis_160(float* steeringAngle);

float destinationSpeed = 0.0f;
bool motorEnabled = false;

void Setup() {
	mCpu_Setup();

	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	mSpi_Setup();
	mSpi_Open();

	mAd_Setup();
	mAd_Open();

	mSwitch_Setup();
	mSwitch_Open();

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
	pixy.setLED(currentConfig->pixyLedColorR, currentConfig->pixyLedColorG, currentConfig->pixyLedColorB);
	pixy.setLamp((uint8_t)(currentConfig->pixyLamps>>8), (uint8_t)(currentConfig->pixyLamps>>0));
	pixy.changeProg(currentConfig->cameraProgram);
}

//Eine / Mehrere Zeilen können definiert + gewählt werden
void cameraRowsSetup() {
	rowAnalysisLength = currentConfig->rowConfigLength;
	for (uint8_t i = 0; i < rowAnalysisLength; i++)
	{
		RowConfig* currentRow = &currentConfig->rowConfigs[i];
		singleRowAnalysis[i].Setup(
			&pixy,
			currentRow->row,
			currentRow->edgeThreshold,
			currentRow->minEdgeWidth,
			currentRow->maxEdgeWidth,
			currentRow->centerPixel,
			currentRow->minThickness
		);
	}
}

//Auslesen der Kamera, Sobel und Kanten der übergebenen Reihen!
void generalCameraTask(CameraAnalysis::SingleRowAnalysis* rowsToDo, uint8_t length) {
	for(uint8_t i = 0; i<length; i++) {
		rowsToDo[i].getImageRow();
		rowsToDo[i].calculateSobelRow();
		//rowsToDo[i]->findBlankArea();
		//rowsToDo[i]->calculateEdges();
	}
}


void lenkung() {
	CameraAnalysis::SingleRowAnalysis* steeringRowAnalysis = &singleRowAnalysis[0];

	steeringRowAnalysis->findBlankArea();

	//mLeds_Write(kMaskLed2,kLedOff);

	// printf("Centers %d / %d\n", singleRowAnalysis_180.trackCenter, singleRowAnalysis_150.trackCenter);

	float steeringAngle = (float)steeringRowAnalysis->trackCenter - (float)steeringRowAnalysis->centerPixel;
	steeringAngle /= 79.0f;
	steeringAngle *= steeringAngle;

	steeringAngle *= currentConfig->steeringFactor;

	static float minSteeringAngle = 9000.0f;
	static float maxSteeringAngle = -9000.0f;


	if(steeringRowAnalysis->trackCenter < steeringRowAnalysis->centerPixel) {
		mTimer_SetServoDuty(0, -steeringAngle + currentConfig->servoSteeringOffset);

		if (-steeringAngle > maxSteeringAngle) {
			maxSteeringAngle = -steeringAngle;
		}
		if (-steeringAngle < minSteeringAngle) {
			minSteeringAngle = -steeringAngle;
		}
	}
	else {
		mTimer_SetServoDuty(0, steeringAngle + currentConfig->servoSteeringOffset);

		if (steeringAngle > maxSteeringAngle) {
			maxSteeringAngle = steeringAngle;
		}
		if (steeringAngle < minSteeringAngle) {
			minSteeringAngle = steeringAngle;
		}
	}

}

void adjustSpeed() {
	CameraAnalysis::SingleRowAnalysis* speedRowAnalysis = &singleRowAnalysis[1];
	speedRowAnalysis->calculateTrackDifferences();

	// TODO: Use Config value instead of Poti
	int16_t speedUpThresholdBlubb = (int16_t)(((mAd_Read(ADCInputEnum::kPot1) + 1) / 2) * 130.0f);

	if (abs((int16_t)speedRowAnalysis->trackCenter - (int16_t)speedRowAnalysis->centerPixel) < speedUpThresholdBlubb) {
		// Increase speed on straight tracks
		destinationSpeed += (currentConfig->timePerFrame / currentConfig->speedAdjustTIme) * (currentConfig->speedMax - currentConfig->speedMin); // TIME_PER_FRAME * MAXIMUM_RAISE_PER_SECOND
		if (destinationSpeed > currentConfig->speedMax) {
			destinationSpeed = currentConfig->speedMax;
		}
	} else {
		// Reset to "turn speed" in turns
		destinationSpeed = currentConfig->speedMin;
	}
}


void defineTasks() {
	t_testMotorButton = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		static bool buttonState = false;

		bool pressed = mSwitch_ReadPushBut(kPushButSW1);

		if(buttonState && !pressed){
			buttonState = false;
			motorEnabled = !motorEnabled;
			if(!motorEnabled)
				mTimer_SetMotorDuty(0, 0);
		}
		if(!buttonState && pressed){
			buttonState = true;
		}
	}, 250, true, false);

	t_motorStop = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		mTimer_SetMotorDuty(0, 0);
		self->active = false;
	}, 90000, false, false);

	t_generalCamera = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		generalCameraTask(singleRowAnalysis, rowAnalysisLength);
	}, min(17, currentConfig->timePerFrame));

	t_cameraAlgorithm = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		lenkung();
		adjustSpeed();

		if(motorEnabled)
			mTimer_SetMotorDuty(destinationSpeed, destinationSpeed);
		else
			mTimer_SetMotorDuty(0, 0);
	}, currentConfig->timePerFrame);

}

int main(){
	loadControlConfigs(&controlConfigsLength, controlConfigs);
	currentConfig = &controlConfigs[0];

	printf("Hello Car\n");

	Setup();
	defineTasks();

	for(UInt32 i = 0; true; i++){
		Scheduler::Update();
	}

	return 0;
}
