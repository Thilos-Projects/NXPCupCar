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
//Task Definitionen
Scheduler::taskHandle* t_testMotorButton;
Scheduler::taskHandle* t_cameraAlgorithm;
Scheduler::taskHandle* t_batteryLevelMonitor;
Scheduler::taskHandle* t_dipSwitchConfig;

//Bennenungen für Programmstruktur
void pixySetup();

float destinationSpeed = 0.0f;
bool motorEnabled = false;
float batteryLevel = 0.0f;
float batteryAccelerationFactor = 1.0f;

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

float speedBattery(float destAcceleration) { // Speed Berry with Destination Excel
	return destAcceleration * batteryAccelerationFactor;
}

void controlCar() {
	// Setup
	static CameraAnalysis::SingleRowAnalysis currentRowAnalysis;
	static float lastSteeringAngle = 0.0f;
	static uint8_t brakeAppliedFor = 0;
	static uint8_t brakeCooledDownFor = 0;
	int16_t trackCenterDifferences[6]; 
	uint8_t currentRowIndex;
	uint8_t countStraightTracks = 0;
	uint8_t countTurnTracks = 0;
	uint8_t countCrossingTracks = 0;

	mLeds_Write(LedMaskEnum::kMaskLed1, LedStateEnum::kLedOff);
	mLeds_Write(LedMaskEnum::kMaskLed2, LedStateEnum::kLedOff);
	mLeds_Write(LedMaskEnum::kMaskLed3, LedStateEnum::kLedOff);
	mLeds_Write(LedMaskEnum::kMaskLed4, LedStateEnum::kLedOff);

	// Loop
	for (currentRowIndex = 0; currentRowIndex < currentConfig->rowConfigLength; currentRowIndex++)
	{
		// Load current Row
		RowConfig* currentRowConfig = &currentConfig->rowConfigs[currentRowIndex];
		currentRowAnalysis.Setup(
			&pixy,
			currentRowConfig->row,
			currentRowConfig->edgeThreshold,
			currentRowConfig->minEdgeWidth,
			currentRowConfig->maxEdgeWidth,
			currentRowConfig->centerPixel,
			currentRowConfig->minThickness
		);

		// Analyze Row Image
		currentRowAnalysis.getImageRow();
		currentRowAnalysis.calculateSobelRow();
		currentRowAnalysis.findBlankArea();

		// Detect Turn / Crossing / Straight
		int16_t trackCenterDifference = (int16_t)currentRowAnalysis.trackCenter - (int16_t)currentRowAnalysis.centerPixel;
		if (abs(trackCenterDifference) > currentRowConfig->maxCenterDifferenceForTurn) {
			// Update Row
			currentRowAnalysis.row = currentRowConfig->rowClose;
			
			// Analyze Row Image
			currentRowAnalysis.getImageRow();
			currentRowAnalysis.calculateSobelRow();
			currentRowAnalysis.findBlankArea();
			
			int16_t trackCenterCloseDifference = (int16_t)currentRowAnalysis.trackCenter - (int16_t)currentRowAnalysis.centerPixel;

			if ((trackCenterCloseDifference < 0) == (trackCenterDifference < 0)) { // Turn Track
				mLeds_Write(LedMaskEnum::kMaskLed1, LedStateEnum::kLedOn);
				countTurnTracks++;
				trackCenterDifferences[currentRowIndex] = trackCenterDifference;
				currentRowIndex++;
				break;
			} else { // Crossing Strack
				mLeds_Write(LedMaskEnum::kMaskLed2, LedStateEnum::kLedOn);
				countCrossingTracks++;
				trackCenterDifferences[currentRowIndex] = (trackCenterCloseDifference + trackCenterDifference) / 2;
			}
		} else { // Straight Track
			mLeds_Write(LedMaskEnum::kMaskLed3, LedStateEnum::kLedOn);
			countStraightTracks++;
			trackCenterDifferences[currentRowIndex] = trackCenterDifference;
		}
	}

	// Control Car
	float avgTrackCenterDifference = 0;
	float weightSumm = 0;
	for (uint8_t i = 0; i < currentRowIndex; i++)
	{
		//printf("trackCenterDifferences[%d]: %d, %d\t", i, (int32_t)(trackCenterDifferences[i]), (int32_t)(currentConfig->rowConfigs[i].weight * 1));
		avgTrackCenterDifference += trackCenterDifferences[i] * currentConfig->rowConfigs[i].weight;
		weightSumm += currentConfig->rowConfigs[i].weight;
	}

	avgTrackCenterDifference /= currentRowIndex * weightSumm;
	//avgTrackCenterDifference = trackCenterDifferences[currentRowIndex-1];	//changed
	
	//printf("avgTrackCenterDifference: %d, %d, %d\n", (int32_t)(avgTrackCenterDifference * 1), currentRowIndex, (int32_t)weightSumm);

	// Steering
	float steeringAngle = avgTrackCenterDifference;
	steeringAngle /= 79.0f;
	steeringAngle *= steeringAngle;

	steeringAngle *= (currentConfig->steeringPotentialFactor / currentConfig->rowConfigLength) * (currentConfig->rowConfigLength - max(0,countStraightTracks - 1));

	//float steeringAngleDerivative = ((lastSteeringAngle - steeringAngle) /*currentConfig->timePerFrame*/) * currentConfig->steeringDerivativeFactor;

	//steeringAngle += steeringAngleDerivative;

	if (avgTrackCenterDifference < 0) {
		lastSteeringAngle = -steeringAngle + currentConfig->servoSteeringOffset;
		mTimer_SetServoDuty(0, -steeringAngle + currentConfig->servoSteeringOffset);
	}
	else {
		lastSteeringAngle = steeringAngle + currentConfig->servoSteeringOffset;
		mTimer_SetServoDuty(0, steeringAngle + currentConfig->servoSteeringOffset);
	}

	// Speed
	if (currentRowIndex < currentConfig->brakeRowDistance) { // Break or Turn
		if (brakeAppliedFor < currentConfig->brakeFrameCount && brakeCooledDownFor == currentConfig->brakeFrameCooldown) {
			destinationSpeed = currentConfig->brakeSpeed;
			brakeAppliedFor++;
		} else {
			destinationSpeed = currentConfig->turnSpeed;
			brakeCooledDownFor = 0;
		}
	} else { // Straight
		brakeAppliedFor = 0;
		brakeCooledDownFor++;
		if(brakeCooledDownFor > currentConfig->brakeFrameCooldown)
			brakeCooledDownFor = currentConfig->brakeFrameCooldown;
		destinationSpeed = currentConfig->straightSpeed;
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

	t_cameraAlgorithm = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		controlCar();

		if(motorEnabled) {
			float speed = speedBattery(destinationSpeed);
			mTimer_SetMotorDuty(speed * 1.05f + 0.05f*speed/abs(speed), speed); //Änderung: Motoren Gleich Schnell fahren lassen
		} else
			mTimer_SetMotorDuty(0, 0);
	}, currentConfig->timePerFrame);

	t_batteryLevelMonitor = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		batteryLevel = mAd_Read(ADCInputEnum::kUBatt);
		printf("Batterie Level: %d\n", (int32_t)(batteryLevel*1000));
		for (uint8_t i = 1; i < currentConfig->batteryLevelLookupLength; i++)
		{
			if (batteryLevel > currentConfig->batteryLevelLookup[i].batteryLevel) {
				BatteryLevelLookupEntry currentEntry = currentConfig->batteryLevelLookup[i];
				BatteryLevelLookupEntry previousEntry = currentConfig->batteryLevelLookup[i - 1];
				float scaler = (batteryLevel - currentEntry.batteryLevel) / (previousEntry.batteryLevel - currentEntry.batteryLevel);
				batteryAccelerationFactor = scaler * (previousEntry.accelerationFactor - currentEntry.accelerationFactor) + currentEntry.accelerationFactor;
				break;
			}
		}
		
	}, currentConfig->batteryLevelCheckInterval);

	t_dipSwitchConfig = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		uint8_t switchmode = 0;
		switchmode += mSwitch_ReadSwitch(SwitchEnum::kSw1);
		switchmode += mSwitch_ReadSwitch(SwitchEnum::kSw2) * 2;
		switchmode += mSwitch_ReadSwitch(SwitchEnum::kSw3) * 4;
		switchmode += mSwitch_ReadSwitch(SwitchEnum::kSw4) * 8;
		if(switchmode < controlConfigsLength)
			currentConfig = &controlConfigs[switchmode];
	}, 1000);
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
