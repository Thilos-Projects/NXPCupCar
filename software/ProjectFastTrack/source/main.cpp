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
#include "ThilosAddons/MotorControl.h"

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
Scheduler::taskHandle* t_dipSwitchConfig;
Scheduler::taskHandle* t_spedRead;
Scheduler::taskHandle* t_finishLineGrace;

//Bennenungen für Programmstruktur
void pixySetup();

float destinationSpeed = 0.0f;
bool motorEnabled = false;
bool steeringDiabled = false;
bool finishLineGraceTimer = false;
bool allowActiveBrake = false;

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

	MotorControl::Setup();

	//Motor Setup (Motor Enable)
	mTimer_EnableHBridge();

	// Load Config
	uint8_t switchmode = 0;
	switchmode += mSwitch_ReadSwitch(SwitchEnum::kSw1);
	switchmode += mSwitch_ReadSwitch(SwitchEnum::kSw2) * 2;
	currentConfig = &controlConfigs[switchmode];
	pixy.setLED(currentConfig->pixyLedColorR, currentConfig->pixyLedColorG, currentConfig->pixyLedColorB);
	pixy.setLamp((uint8_t)(currentConfig->pixyLamps>>8), (uint8_t)(currentConfig->pixyLamps>>0));
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

void controlCar() {
	// Setup
	static CameraAnalysis::SingleRowAnalysis currentRowAnalysis;
	static CameraAnalysis::SingleColumnAnalysis columnAnalysis;
	static CameraAnalysis::PartialColumnAnalysis partColumnAnalysis;
	static float lastSteeringAngle = 0.0f;
	static uint8_t brakeAppliedFor = 0;
	static int16_t trackCenterDifferences[6]; // Size: Length of Rows
	static uint16_t prevTrackCenters[6] = { 158, 158, 158, 158, 158, 158 }; // Size: Length of Rows
	static bool trackWidthOverThreshold[7]; // Size: Length of Rows +1!
	static bool stop = false;
	static bool switchConfig = false;
	static uint8_t switchConfigToIndex;
	static uint32_t switchConfigAfterTime = 0;
	uint8_t currentRowIndex;
	uint8_t countStraightTracks = 0;
	uint8_t countTurnTracks = 0;
	uint8_t countCrossingTracks = 0;
	uint8_t lastRow = 0;

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
			prevTrackCenters[currentRowIndex],
			currentRowConfig->minThickness
		);

		// Analyze Row Image
		currentRowAnalysis.getImageRow();
		currentRowAnalysis.calculateSobelRow();
		currentRowAnalysis.findBlankArea();
		prevTrackCenters[currentRowIndex] = currentRowAnalysis.trackCenter;

		// Detect Turn / Crossing / Straight
		int16_t trackCenterDifference = (int16_t)currentRowAnalysis.trackCenter - 158;
		trackWidthOverThreshold[currentRowIndex] = currentRowAnalysis.trackWidth > currentRowConfig->maxTrackWidth;
		if (abs(trackCenterDifference) > currentRowConfig->maxCenterDifferenceForTurn) {
			// Update Row
			currentRowAnalysis.row = currentRowConfig->rowClose;
			
			// Analyze Row Image
			currentRowAnalysis.getImageRow();
			currentRowAnalysis.calculateSobelRow();
			currentRowAnalysis.findBlankArea();
			
			int16_t trackCenterCloseDifference = (int16_t)currentRowAnalysis.trackCenter - 158;

			if ((trackCenterCloseDifference < 0) == (trackCenterDifference < 0)) { // Turn Track
				countTurnTracks++;
				trackCenterDifferences[currentRowIndex] = trackCenterDifference;
				currentRowIndex++;
				for (uint8_t resetRowIndex = currentRowIndex; resetRowIndex < currentConfig->rowConfigLength; resetRowIndex++) {
					prevTrackCenters[resetRowIndex] = 158;
				}
				break;
			} else { // Crossing Strack
				countCrossingTracks++;
				trackCenterDifferences[currentRowIndex] = (trackCenterCloseDifference + trackCenterDifference) / 2;
			}
		} else { // Straight Track
			countStraightTracks++;
			trackCenterDifferences[currentRowIndex] = trackCenterDifference;
		}
		
		lastRow = currentRowConfig->row;
	}

	// Finish Line detection
	if (currentConfig->finishLineDetection && finishLineGraceTimer) {
		static bool finishLineDetectedLeft = false, finishLineDetectedRight = false, finishLineDetectedCenter = false;
		static uint8_t secondPosLeft = 0, secondPosRight = 0, secondPosCenter = 0;

		// TODO: Refactor: Duplication left <> right
		// Left - 131
		partColumnAnalysis.Setup(&pixy, prevTrackCenters[0] - currentConfig->finishLineLeftOffset, 120, 207, currentConfig->columnConfig.edgeThreshold,
			currentConfig->columnConfig.minEdgeWidth, currentConfig->columnConfig.maxEdgeWidth, currentConfig->columnConfig.minThickness);
		partColumnAnalysis.getImageColumn();
		partColumnAnalysis.calculateSobel();
		finishLineDetectedLeft = partColumnAnalysis.detectFinishline();
		secondPosLeft = partColumnAnalysis.secondPos;

		// Right - 190
		partColumnAnalysis.Setup(&pixy, prevTrackCenters[0] + currentConfig->finishLineRightOffset, 120, 207, currentConfig->columnConfig.edgeThreshold,
			currentConfig->columnConfig.minEdgeWidth, currentConfig->columnConfig.maxEdgeWidth, currentConfig->columnConfig.minThickness);
		partColumnAnalysis.getImageColumn();
		partColumnAnalysis.calculateSobel();
		finishLineDetectedRight = partColumnAnalysis.detectFinishline();
		secondPosRight = partColumnAnalysis.secondPos;

		// Validate if real finish line
		if (finishLineDetectedLeft && finishLineDetectedRight) {
			// Center
			partColumnAnalysis.Setup(&pixy, prevTrackCenters[0], 120, 207, currentConfig->columnConfig.edgeThreshold,
				currentConfig->columnConfig.minEdgeWidth, currentConfig->columnConfig.maxEdgeWidth, currentConfig->columnConfig.minThickness);
			partColumnAnalysis.getImageColumn();
			partColumnAnalysis.calculateSobel();
			finishLineDetectedCenter = partColumnAnalysis.detectFinishline();
			secondPosCenter = partColumnAnalysis.secondPos;

			if (!finishLineDetectedCenter) { // FINISH!
				if (currentConfig->switchConfigAfterFinishLineDetection) {
					switchConfig = true;
					switchConfigToIndex = currentConfig->configAfterFinishLineDetected;
					switchConfigAfterTime = Scheduler::getMillis() + currentConfig->switchConfigAfterFinishLineTimeout;
				}
			}
		}
	}

	// Obstacle Detection
	if (currentConfig->obstacleDetection){
		if (lastRow != 0) {
			// Column detection
			columnAnalysis.Setup(&pixy, prevTrackCenters[0], currentConfig->columnConfig.edgeThreshold,
				currentConfig->columnConfig.minEdgeWidth, currentConfig->columnConfig.maxEdgeWidth, currentConfig->columnConfig.minThickness);
			columnAnalysis.getImageColumn();
			columnAnalysis.calculateSobel();
			bool foundObstacle = columnAnalysis.detectObstacle(lastRow);

			// TODO: Comment Debug
			columnAnalysis.printLines();
			columnAnalysis.printSobleColumn();
			
			if (foundObstacle) {
				if (columnAnalysis.obstacleBottomEdge > currentConfig->minObstacleRow) {
					stop = true;
				}
				
				mLeds_Write(LedMaskEnum::kMaskLed1, LedStateEnum::kLedOn);
			} else {
				mLeds_Write(LedMaskEnum::kMaskLed1, LedStateEnum::kLedOff);
			}
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
	
	float leftSpeed, rightSpeed;
	MotorControl::getSpeed(&leftSpeed, &rightSpeed);
	float currentSpeed = max(leftSpeed, rightSpeed);

	// Steering
	float steeringAngle = avgTrackCenterDifference;
	steeringAngle /= 79.0f;
	steeringAngle *= steeringAngle;

	if (avgTrackCenterDifference < 0) {
		steeringAngle *= -1;
	}

	/*float steeringAngleFactor = currentConfig->steeringPotentialFactor;
	steeringAngleFactor += (((leftSpeed + rightSpeed) / 2) / currentConfig->steeringPotentialFactorSpeedIncrements) * currentConfig->steeringPotentialFactorPerSpeed;

	steeringAngle *= steeringAngleFactor;*/

	float steeringAngleFactor = currentConfig->steeringPotentialFactor;

	float steeringAngleDerivative = (lastSteeringAngle - steeringAngle) * currentConfig->steeringDerivativeFactor;

	steeringAngle += steeringAngleDerivative;

	lastSteeringAngle = steeringAngle;
	if (!steeringDiabled) {
		MotorControl::setServo(steeringAngle, currentConfig->servoSteeringOffset);
	} else {
		MotorControl::setServo(0.0f, currentConfig->servoSteeringOffset);
	}

	// Speed
	if (stop) {
		allowActiveBrake = true;
		destinationSpeed = 0.0f;
	} else { // Normal Speed Control
		uint8_t maxRowForSpeedCalculation = currentRowIndex;
		trackWidthOverThreshold[6] = trackWidthOverThreshold[5]; // Prevent NullPointerException
		for (; maxRowForSpeedCalculation > 1 && trackWidthOverThreshold[maxRowForSpeedCalculation] ; maxRowForSpeedCalculation--);
		if (maxRowForSpeedCalculation < currentConfig->brakeRowDistance) { // Break or Turn
			allowActiveBrake = true;
			destinationSpeed = currentConfig->turnSpeed;
		} else { // Straight
			allowActiveBrake = false;
			brakeAppliedFor = 0;
			destinationSpeed = currentConfig->straightSpeed;
		}
	}

	// Switch Config?
	if (switchConfig && Scheduler::getMillis() > switchConfigAfterTime) {
		currentConfig = &controlConfigs[switchConfigToIndex];
		switchConfig = false;
	}
}

float U0OfN(float n) {
	float U0 = 0.176f * n;
	if (n > 0.0f) {
		return U0 + 0.008;
	} else if(n < 0.0f) {
		return U0 - 0.008;
	}
	return U0 + 0.008;
}

/*
* Inspired by Rüders
*/
float calculateAcceleration(float currentSpeed, float destSpeed) {
	float factor;
	//hier die dynamisch lineare Methode
	if (destSpeed>=(currentSpeed+2))      factor = currentConfig->linearAcceleration;
	else if (destSpeed>=currentSpeed)     factor = currentConfig->linearAcceleration * 0.5f * (destSpeed - currentSpeed); //dieser Koeffizient muss immer halb so gross sein, wie der obige
	else if (destSpeed>=(currentSpeed-2)) factor = currentConfig->linearBrake * -0.5f * (destSpeed - currentSpeed); //dieser Koeffizient muss immer halb so gross sein, wie der untige
	else                                  factor = currentConfig->linearBrake;

	float destU0 = (U0OfN(currentSpeed)/8.3+factor);

	float minAcc = 0.17f;

	if (destSpeed < -0.0000000000001f) {
		return destU0 - minAcc;
	} else if (destSpeed > 0.000000001f) {
		return destU0 + minAcc;
	} else {
		return destU0;
	}

}

void controlMotor() {
	static float lastAcceleration = 0.0f;
	if (motorEnabled) {

		// TODO: Move this to configuration
		float accMultiplierLeft = 1.0f; //1.05f; //mAd_Read(ADCInputEnum::kPot1) + 2;
		float accMultiplierRight = 1.0f; //1.100616f; //mAd_Read(ADCInputEnum::kPot2) + 2;
		// printf("SL: %d\tSR: %d", (int32_t)(accMultiplierLeft * 1000000.0f), (int32_t)(accMultiplierRight * 1000000.0f));
		
		float currentSpeedLeft, currentSpeedRight;
		MotorControl::getSpeed(&currentSpeedLeft, &currentSpeedRight);

		float currentSpeed = (currentSpeedLeft + currentSpeedRight) / 2.0f;

		float acceleration = calculateAcceleration(currentSpeed, destinationSpeed);

		if (abs(destinationSpeed - currentSpeed) < 1.0f) {
			mLeds_Write(LedMaskEnum::kMaskLed3, LedStateEnum::kLedOn);
		} else {
			mLeds_Write(LedMaskEnum::kMaskLed3, LedStateEnum::kLedOff);
		}

		// Derivate
		if (acceleration >= 0.0f) {
			if (lastAcceleration == 0.0f)
				lastAcceleration = acceleration;
			float accelerationDerivate = (lastAcceleration - acceleration) * currentConfig->speedDerivate;
			acceleration += accelerationDerivate;
			lastAcceleration = acceleration;
		} else {
			lastAcceleration = 0.0f;
		}

		MotorControl::setSpeed(acceleration * accMultiplierLeft, acceleration * accMultiplierRight);
	} else {
		lastAcceleration = 0.0f;
		MotorControl::setSpeed(0, 0);
	}
}


void defineTasks() {
	t_cameraAlgorithm = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		controlCar();
		controlMotor();
	}, currentConfig->timePerFrame);

	t_dipSwitchConfig = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		// Disable Steering
		steeringDiabled = !mSwitch_ReadSwitch(SwitchEnum::kSw3);

		motorEnabled = mSwitch_ReadSwitch(SwitchEnum::kSw4);
		if(!motorEnabled)
			MotorControl::setSpeed(0, 0);
		else
			Scheduler::SetActive(t_finishLineGrace);
	}, 1000);

	t_spedRead = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		float a,b;
		MotorControl::getSpeed(&a, &b);
		printf("Speed %d, %d\n", (int32_t)(a*1000.0f), (int32_t)(b*1000.0f));
	}, 500, true, true);

	finishLineGraceTimer = false;
	t_finishLineGrace = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		finishLineGraceTimer = true;
		mLeds_Write(LedMaskEnum::kMaskLed4, LedStateEnum::kLedOff);
	}, currentConfig->startFinishLineDetectionAfter, false, false);
}

int main(){
	loadControlConfigs(&controlConfigsLength, controlConfigs);
	currentConfig = &controlConfigs[0];

	printf("Hello Car\n");

	Setup();
	defineTasks();

	mLeds_Write(LedMaskEnum::kMaskLed4, LedStateEnum::kLedOn);

	for(UInt32 i = 0; true; i++){
		Scheduler::Update();
	}

	return 0;
}
