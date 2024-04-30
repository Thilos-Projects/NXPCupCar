#include "ControlConfigs.h"
#include "ControlConfigStruct.h"

void makeStandardRowConfig(RowConfig** configs, uint8_t* configLength){
	*configLength = 5;
	*configs = (RowConfig*)malloc(sizeof(RowConfig) * *configLength);

	((*configs)[0]).row = 150;
	((*configs)[0]).rowClose = 150;
	((*configs)[0]).edgeThreshold = 40;
	((*configs)[0]).minEdgeWidth = 0;
	((*configs)[0]).maxEdgeWidth = 12;
	((*configs)[0]).minThickness = 4;
	((*configs)[0]).maxCenterDifferenceForTurn = 20;
	((*configs)[0]).maxTrackWidth = 316;
	((*configs)[0]).weight = 5.0f;
	// Second Row Config
	((*configs)[1]).row = 118;
	((*configs)[1]).rowClose = 118;
	((*configs)[1]).edgeThreshold = 40;
	((*configs)[1]).minEdgeWidth = 0;
	((*configs)[1]).maxEdgeWidth = 5;
	((*configs)[1]).minThickness = 4;
	((*configs)[1]).maxCenterDifferenceForTurn = 20;
	((*configs)[1]).maxTrackWidth = 316;
	((*configs)[1]).weight = 4.0f;
	// Third Row Config
	((*configs)[2]).row = 96;
	((*configs)[2]).rowClose = 96;
	((*configs)[2]).edgeThreshold = 40;
	((*configs)[2]).minEdgeWidth = 0;
	((*configs)[2]).maxEdgeWidth = 5;
	((*configs)[2]).minThickness = 3;
	((*configs)[2]).maxCenterDifferenceForTurn = 15;
	((*configs)[2]).maxTrackWidth = 316;
	((*configs)[2]).weight = 3.0f;
	// Fourth Row Config
	((*configs)[3]).row = 82;
	((*configs)[3]).rowClose = 77;
	((*configs)[3]).edgeThreshold = 40;
	((*configs)[3]).minEdgeWidth = 0;
	((*configs)[3]).maxEdgeWidth = 4;
	((*configs)[3]).minThickness = 2;
	((*configs)[3]).maxCenterDifferenceForTurn = 15;
	((*configs)[3]).maxTrackWidth = 316;
	((*configs)[3]).weight = 2.0f;
	// Fifth Row Config
	((*configs)[4]).row = 74;
	((*configs)[4]).rowClose = 69;
	((*configs)[4]).edgeThreshold = 40;
	((*configs)[4]).minEdgeWidth = 0;
	((*configs)[4]).maxEdgeWidth = 4;
	((*configs)[4]).minThickness = 1;
	((*configs)[4]).maxCenterDifferenceForTurn = 10;
	((*configs)[4]).maxTrackWidth = 316;
	((*configs)[4]).weight = 1.0f;
}

void createDefaultBrakeConfig(BrakeLookupEntry** configs, uint8_t* configLength){
	*configLength = 7;
	*configs = (BrakeLookupEntry*)malloc(sizeof(BrakeLookupEntry) * *configLength);

	((*configs) + 0)->minSpeedDifference = -0.0f;
	((*configs) + 0)->frameCount = 5;
	((*configs) + 0)->acceleration = -0.0f;

	((*configs) + 1)->minSpeedDifference = -3.0f;
	((*configs) + 0)->frameCount = 5;
	((*configs) + 1)->acceleration = -0.2f;

	((*configs) + 2)->minSpeedDifference = -6.0f;
	((*configs) + 0)->frameCount = 10;
	((*configs) + 2)->acceleration = -0.3f;

	((*configs) + 3)->minSpeedDifference = -10.0f;
	((*configs) + 0)->frameCount = 10;
	((*configs) + 3)->acceleration = -0.7f;

	((*configs) + 4)->minSpeedDifference = -15.0f;
	((*configs) + 0)->frameCount = 10;
	((*configs) + 4)->acceleration = -0.7f;

	((*configs) + 5)->minSpeedDifference = -20.0f;
	((*configs) + 0)->frameCount = 15;
	((*configs) + 5)->acceleration = -1.0f;

	((*configs) + 6)->minSpeedDifference = -30.0f;
	((*configs) + 0)->frameCount = 20;
	((*configs) + 6)->acceleration = -1.0f;
}

void createDefaultAccelerationConfig(AccelerationLookupEntry** configs, uint8_t* configLength) {
	*configLength = 7;
	*configs = (AccelerationLookupEntry*)malloc(sizeof(AccelerationLookupEntry) * *configLength);

	((*configs) + 0)->minSpeedDifference = 0.0f;
	((*configs) + 0)->acceleration = 0.2f;

	((*configs) + 1)->minSpeedDifference = 3.0f;
	((*configs) + 1)->acceleration = 0.2f;

	((*configs) + 2)->minSpeedDifference = 6.0f;
	((*configs) + 2)->acceleration = 0.2f;

	((*configs) + 3)->minSpeedDifference = 10.0f;
	((*configs) + 3)->acceleration = 0.3f;

	((*configs) + 4)->minSpeedDifference = 15.0f;
	((*configs) + 4)->acceleration = 0.4f;

	((*configs) + 5)->minSpeedDifference = 20.0f;
	((*configs) + 5)->acceleration = 0.7f;

	((*configs) + 6)->minSpeedDifference = 30.0f;
	((*configs) + 6)->acceleration = 1.0f;
}

void fastConfig(uint8_t configIndex, ControlConfig* controlConfigs){
	controlConfigs[configIndex].timePerFrame = 17;
	controlConfigs[configIndex].servoSteeringOffset = -0.0f;
	controlConfigs[configIndex].pixyLedColorR = 255;
	controlConfigs[configIndex].pixyLedColorG = 255;
	controlConfigs[configIndex].pixyLedColorB = 31;
	controlConfigs[configIndex].pixyLamps = 0x0101;
	controlConfigs[configIndex].cameraProgram = "video";
	controlConfigs[configIndex].steeringPotentialFactor = 1.6f;
	controlConfigs[configIndex].steeringPotentialFactorPerSpeed = 0.4f;
	controlConfigs[configIndex].steeringPotentialFactorSpeedIncrements = 35;
	controlConfigs[configIndex].steeringDerivativeFactor = 0.0f;
	controlConfigs[configIndex].steeringHoldframesAfterTurn = 0;
	controlConfigs[configIndex].brakeRowDistance = 1;
	controlConfigs[configIndex].straightSpeed = 0.3f;
	controlConfigs[configIndex].turnSpeed = 0.2f;

	createDefaultBrakeConfig(&controlConfigs[configIndex].brakeLookupEntries, &controlConfigs[configIndex].brakeLookupEntryCount);
	createDefaultAccelerationConfig(&controlConfigs[configIndex].acceleatationLookupEntries, &controlConfigs[configIndex].acceleatationLookupEntryCount);

	makeStandardRowConfig(&controlConfigs[configIndex].rowConfigs, &controlConfigs[configIndex].rowConfigLength);

	// Column Config
    controlConfigs[configIndex].columnConfig.column = 158;
    controlConfigs[configIndex].columnConfig.edgeThreshold = 18;
    controlConfigs[configIndex].columnConfig.minEdgeWidth = 0;
    controlConfigs[configIndex].columnConfig.maxEdgeWidth = 10;
    controlConfigs[configIndex].columnConfig.minThickness = 0;

	// Finish Line Config
    controlConfigs[configIndex].finishLineDetection = true;
    controlConfigs[configIndex].switchConfigAfterFinishLineDetection = true;
    controlConfigs[configIndex].configAfterFinishLineDetected = 4;
    controlConfigs[configIndex].switchConfigAfterFinishLineTimeout = 200;

	// Battery - SORTED!
	controlConfigs[configIndex].batteryLevelCheckInterval = 1000;
	controlConfigs[configIndex].batteryLevelLookupLength = 10;
	controlConfigs[configIndex].batteryLevelLookup = (BatteryLevelLookupEntry*)malloc(sizeof(BatteryLevelLookupEntry) * controlConfigs[configIndex].batteryLevelLookupLength);
	// Bigger battery
	controlConfigs[configIndex].batteryLevelLookup[0].batteryLevel = 13.0f;
	controlConfigs[configIndex].batteryLevelLookup[0].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[1].batteryLevel = 12.5f;
	controlConfigs[configIndex].batteryLevelLookup[1].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[2].batteryLevel = 12.0f;
	controlConfigs[configIndex].batteryLevelLookup[2].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[3].batteryLevel = 11.5f;
	controlConfigs[configIndex].batteryLevelLookup[3].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[4].batteryLevel = 11.0f;
	controlConfigs[configIndex].batteryLevelLookup[4].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[5].batteryLevel = 10.0f;
	controlConfigs[configIndex].batteryLevelLookup[5].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[5].disableWhenLower = true;
	// Smaller battery
	controlConfigs[configIndex].batteryLevelLookup[6].batteryLevel = 9.0f;
	controlConfigs[configIndex].batteryLevelLookup[6].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[7].batteryLevel = 8.5f;
	controlConfigs[configIndex].batteryLevelLookup[7].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[8].batteryLevel = 7.3f;
	controlConfigs[configIndex].batteryLevelLookup[8].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[9].batteryLevel = 6.9f;
	controlConfigs[configIndex].batteryLevelLookup[9].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[9].disableWhenLower = true;
}

void safeConfig(uint8_t configIndex, ControlConfig* controlConfigs){
	controlConfigs[configIndex].timePerFrame = 17;
	controlConfigs[configIndex].servoSteeringOffset = -0.0f;
	controlConfigs[configIndex].pixyLedColorR = 63;
	controlConfigs[configIndex].pixyLedColorG = 255;
	controlConfigs[configIndex].pixyLedColorB = 63;
	controlConfigs[configIndex].pixyLamps = 0x0101;
	controlConfigs[configIndex].cameraProgram = "video";
	controlConfigs[configIndex].steeringPotentialFactor = 2.0f;
	controlConfigs[configIndex].steeringDerivativeFactor = 0.0f;
	controlConfigs[configIndex].steeringHoldframesAfterTurn = 0;
	controlConfigs[configIndex].brakeRowDistance = 2;
	controlConfigs[configIndex].straightSpeed = 0.23f;
	controlConfigs[configIndex].turnSpeed = 0.2f;

	createDefaultBrakeConfig(&controlConfigs[configIndex].brakeLookupEntries, &controlConfigs[configIndex].brakeLookupEntryCount);
	createDefaultAccelerationConfig(&controlConfigs[configIndex].acceleatationLookupEntries, &controlConfigs[configIndex].acceleatationLookupEntryCount);

	makeStandardRowConfig(&controlConfigs[configIndex].rowConfigs, &controlConfigs[configIndex].rowConfigLength);

    // Column Config
    controlConfigs[configIndex].columnConfig.column = 158;
    controlConfigs[configIndex].columnConfig.edgeThreshold = 18;
    controlConfigs[configIndex].columnConfig.minEdgeWidth = 0;
    controlConfigs[configIndex].columnConfig.maxEdgeWidth = 10;
    controlConfigs[configIndex].columnConfig.minThickness = 0;

	// Finish Line Config
    controlConfigs[configIndex].finishLineDetection = true;
    controlConfigs[configIndex].switchConfigAfterFinishLineDetection = true;
    controlConfigs[configIndex].configAfterFinishLineDetected = 4;
    controlConfigs[configIndex].switchConfigAfterFinishLineTimeout = 200;

	// Battery - SORTED!
	controlConfigs[configIndex].batteryLevelCheckInterval = 1000;
	controlConfigs[configIndex].batteryLevelLookupLength = 10;
	controlConfigs[configIndex].batteryLevelLookup = (BatteryLevelLookupEntry*)malloc(sizeof(BatteryLevelLookupEntry) * controlConfigs[configIndex].batteryLevelLookupLength);
	// Bigger battery
	controlConfigs[configIndex].batteryLevelLookup[0].batteryLevel = 13.0f;
	controlConfigs[configIndex].batteryLevelLookup[0].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[1].batteryLevel = 12.5f;
	controlConfigs[configIndex].batteryLevelLookup[1].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[2].batteryLevel = 12.0f;
	controlConfigs[configIndex].batteryLevelLookup[2].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[3].batteryLevel = 11.5f;
	controlConfigs[configIndex].batteryLevelLookup[3].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[4].batteryLevel = 11.0f;
	controlConfigs[configIndex].batteryLevelLookup[4].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[5].batteryLevel = 10.0f;
	controlConfigs[configIndex].batteryLevelLookup[5].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[5].disableWhenLower = true;
	// Smaller battery
	controlConfigs[configIndex].batteryLevelLookup[6].batteryLevel = 9.0f;
	controlConfigs[configIndex].batteryLevelLookup[6].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[7].batteryLevel = 8.5f;
	controlConfigs[configIndex].batteryLevelLookup[7].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[8].batteryLevel = 7.3f;
	controlConfigs[configIndex].batteryLevelLookup[8].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[9].batteryLevel = 6.9f;
	controlConfigs[configIndex].batteryLevelLookup[9].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[9].disableWhenLower = true;
}

void middleConfig(uint8_t configIndex, ControlConfig* controlConfigs){
    controlConfigs[configIndex].timePerFrame = 17;
    controlConfigs[configIndex].servoSteeringOffset = -0.0f;
    controlConfigs[configIndex].pixyLedColorR = 255;
    controlConfigs[configIndex].pixyLedColorG = 255;
    controlConfigs[configIndex].pixyLedColorB = 31;
    controlConfigs[configIndex].pixyLamps = 0x0101;
    controlConfigs[configIndex].cameraProgram = "video";
    controlConfigs[configIndex].steeringPotentialFactor = 1.6f;
    controlConfigs[configIndex].steeringPotentialFactorPerSpeed = 0.4f;
    controlConfigs[configIndex].steeringPotentialFactorSpeedIncrements = 35;
    controlConfigs[configIndex].steeringDerivativeFactor = 0.3f;
    controlConfigs[configIndex].steeringHoldframesAfterTurn = 0;
    controlConfigs[configIndex].brakeRowDistance = 3;
    controlConfigs[configIndex].straightSpeed = 22.0f;
    controlConfigs[configIndex].turnSpeed = 11.0f;
	controlConfigs[configIndex].speedDerivate = 0.2f;
	controlConfigs[configIndex].slowdownAcceleration = 0.1f;

	createDefaultBrakeConfig(&controlConfigs[configIndex].brakeLookupEntries, &controlConfigs[configIndex].brakeLookupEntryCount);
	createDefaultAccelerationConfig(&controlConfigs[configIndex].acceleatationLookupEntries, &controlConfigs[configIndex].acceleatationLookupEntryCount);

	makeStandardRowConfig(&controlConfigs[configIndex].rowConfigs, &controlConfigs[configIndex].rowConfigLength);

    // Column Config
    controlConfigs[configIndex].columnConfig.column = 158;
    controlConfigs[configIndex].columnConfig.edgeThreshold = 18;
    controlConfigs[configIndex].columnConfig.minEdgeWidth = 0;
    controlConfigs[configIndex].columnConfig.maxEdgeWidth = 10;
    controlConfigs[configIndex].columnConfig.minThickness = 0;

	// Finish Line Config
    controlConfigs[configIndex].finishLineDetection = true;
    controlConfigs[configIndex].switchConfigAfterFinishLineDetection = true;
    controlConfigs[configIndex].configAfterFinishLineDetected = 4;
    controlConfigs[configIndex].switchConfigAfterFinishLineTimeout = 200;

    // Battery - SORTED!
    controlConfigs[configIndex].batteryLevelCheckInterval = 1000;
    controlConfigs[configIndex].batteryLevelLookupLength = 10;
    controlConfigs[configIndex].batteryLevelLookup = (BatteryLevelLookupEntry*)malloc(sizeof(BatteryLevelLookupEntry) * controlConfigs[configIndex].batteryLevelLookupLength);
    // Bigger battery
    controlConfigs[configIndex].batteryLevelLookup[0].batteryLevel = 13.0f;
    controlConfigs[configIndex].batteryLevelLookup[0].accelerationFactor = 0.85f;
    controlConfigs[configIndex].batteryLevelLookup[1].batteryLevel = 12.5f;
    controlConfigs[configIndex].batteryLevelLookup[1].accelerationFactor = 0.85f;
    controlConfigs[configIndex].batteryLevelLookup[2].batteryLevel = 12.0f;
    controlConfigs[configIndex].batteryLevelLookup[2].accelerationFactor = 0.90f;
    controlConfigs[configIndex].batteryLevelLookup[3].batteryLevel = 11.5f;
    controlConfigs[configIndex].batteryLevelLookup[3].accelerationFactor = 0.95f;
    controlConfigs[configIndex].batteryLevelLookup[4].batteryLevel = 11.0f;
    controlConfigs[configIndex].batteryLevelLookup[4].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[5].batteryLevel = 10.0f;
    controlConfigs[configIndex].batteryLevelLookup[5].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[5].disableWhenLower = true;
    // Smaller battery
    controlConfigs[configIndex].batteryLevelLookup[6].batteryLevel = 9.0f;
    controlConfigs[configIndex].batteryLevelLookup[6].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[7].batteryLevel = 8.5f;
    controlConfigs[configIndex].batteryLevelLookup[7].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[8].batteryLevel = 7.3f;
    controlConfigs[configIndex].batteryLevelLookup[8].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[9].batteryLevel = 6.9f;
    controlConfigs[configIndex].batteryLevelLookup[9].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[9].disableWhenLower = true;
}

void middleConfigObstacleDetection(uint8_t configIndex, ControlConfig* controlConfigs){
    controlConfigs[configIndex].timePerFrame = 17;
    controlConfigs[configIndex].servoSteeringOffset = -0.0f;
    controlConfigs[configIndex].pixyLedColorR = 255;
    controlConfigs[configIndex].pixyLedColorG = 255;
    controlConfigs[configIndex].pixyLedColorB = 31;
    controlConfigs[configIndex].pixyLamps = 0x0101;
    controlConfigs[configIndex].cameraProgram = "video";
    controlConfigs[configIndex].steeringPotentialFactor = 1.6f;
    controlConfigs[configIndex].steeringPotentialFactorPerSpeed = 0.4f;
    controlConfigs[configIndex].steeringPotentialFactorSpeedIncrements = 35;
    controlConfigs[configIndex].steeringDerivativeFactor = 0.0f;
    controlConfigs[configIndex].steeringHoldframesAfterTurn = 0;
    controlConfigs[configIndex].brakeRowDistance = 2;
    controlConfigs[configIndex].straightSpeed = 0.19f;
    controlConfigs[configIndex].turnSpeed = 0.19f;
    controlConfigs[configIndex].stopBrakeFrameCount = 3;
    controlConfigs[configIndex].stopBrakeSpeed = -0.2f;

	createDefaultBrakeConfig(&controlConfigs[configIndex].brakeLookupEntries, &controlConfigs[configIndex].brakeLookupEntryCount);
	createDefaultAccelerationConfig(&controlConfigs[configIndex].acceleatationLookupEntries, &controlConfigs[configIndex].acceleatationLookupEntryCount);
	// controlConfigs[configIndex].brakeLookupEntries[2].acceleration = 0.0f;
	// controlConfigs[configIndex].brakeLookupEntries[3].acceleration = -0.1f;
	// controlConfigs[configIndex].brakeLookupEntries[4].acceleration = -0.2f;

	makeStandardRowConfig(&controlConfigs[configIndex].rowConfigs, &controlConfigs[configIndex].rowConfigLength);

    // Column Config
    controlConfigs[configIndex].columnConfig.column = 158;
    controlConfigs[configIndex].columnConfig.edgeThreshold = 18;
    controlConfigs[configIndex].columnConfig.minEdgeWidth = 0;
    controlConfigs[configIndex].columnConfig.maxEdgeWidth = 10;
    controlConfigs[configIndex].columnConfig.minThickness = 0;

	// Obstacle Detection
	controlConfigs[configIndex].obstacleDetection = true;
	// TODO: Calibrate
	controlConfigs[configIndex].minObstacleRow = 130;

    // Battery - SORTED!
    controlConfigs[configIndex].batteryLevelCheckInterval = 1000;
    controlConfigs[configIndex].batteryLevelLookupLength = 10;
    controlConfigs[configIndex].batteryLevelLookup = (BatteryLevelLookupEntry*)malloc(sizeof(BatteryLevelLookupEntry) * controlConfigs[configIndex].batteryLevelLookupLength);
    // Bigger battery
    controlConfigs[configIndex].batteryLevelLookup[0].batteryLevel = 13.0f;
    controlConfigs[configIndex].batteryLevelLookup[0].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[1].batteryLevel = 12.5f;
    controlConfigs[configIndex].batteryLevelLookup[1].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[2].batteryLevel = 12.0f;
    controlConfigs[configIndex].batteryLevelLookup[2].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[3].batteryLevel = 11.5f;
    controlConfigs[configIndex].batteryLevelLookup[3].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[4].batteryLevel = 11.0f;
    controlConfigs[configIndex].batteryLevelLookup[4].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[5].batteryLevel = 10.0f;
    controlConfigs[configIndex].batteryLevelLookup[5].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[5].disableWhenLower = true;
    // Smaller battery
    controlConfigs[configIndex].batteryLevelLookup[6].batteryLevel = 9.0f;
    controlConfigs[configIndex].batteryLevelLookup[6].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[7].batteryLevel = 8.5f;
    controlConfigs[configIndex].batteryLevelLookup[7].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[8].batteryLevel = 7.3f;
    controlConfigs[configIndex].batteryLevelLookup[8].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[9].batteryLevel = 6.9f;
    controlConfigs[configIndex].batteryLevelLookup[9].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[9].disableWhenLower = true;
}

void safeConfigWithoutFinishLineDetection(uint8_t configIndex, ControlConfig* controlConfigs) {
	safeConfig(configIndex, controlConfigs);
    controlConfigs[configIndex].finishLineDetection = false;
};

void middleConfigWithoutFinishLineDetection(uint8_t configIndex, ControlConfig* controlConfigs) {
	middleConfig(configIndex, controlConfigs);
    controlConfigs[configIndex].finishLineDetection = false;
};

void loadControlConfigs(uint8_t* controlConfigsLength, ControlConfig* controlConfigs) {
    *controlConfigsLength = 4;

    for (uint8_t i = 0; i < *controlConfigsLength; i++) {
        controlConfigs[i] = ControlConfig();
    }

    safeConfig(0, controlConfigs);
    middleConfig(1, controlConfigs);
	safeConfigWithoutFinishLineDetection(2, controlConfigs);
	middleConfigWithoutFinishLineDetection(3, controlConfigs);
    // fastConfig(2, controlConfigs);

    middleConfigObstacleDetection(4, controlConfigs);
}
