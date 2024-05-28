#include "ControlConfigs.h"
#include "ControlConfigStruct.h"

void createDefaultRowConfig(RowConfig** configs, uint8_t* configLength){
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

void createDefaultColumnConfig(uint8_t configIndex, ControlConfig* controlConfigs) {
    controlConfigs[configIndex].columnConfig.column = 158;
    controlConfigs[configIndex].columnConfig.edgeThreshold = 15;
    controlConfigs[configIndex].columnConfig.minEdgeWidth = 0;
    controlConfigs[configIndex].columnConfig.maxEdgeWidth = 10;
    controlConfigs[configIndex].columnConfig.minThickness = 0;
}

/* Should be consired "safe" when making it 10 laps */
void safeConfig(uint8_t configIndex, ControlConfig* controlConfigs) {
    controlConfigs[configIndex].pixyLedColorR = 255;
    controlConfigs[configIndex].pixyLedColorG = 255;
    controlConfigs[configIndex].pixyLedColorB = 31;

    controlConfigs[configIndex].steeringPotentialFactor = 1.6f;
    controlConfigs[configIndex].steeringPotentialFactorPerSpeed = 0.4f;
    controlConfigs[configIndex].steeringPotentialFactorSpeedIncrements = 35;
    controlConfigs[configIndex].steeringDerivativeFactor = 0.3f;

    controlConfigs[configIndex].brakeRowDistance = 3;
    controlConfigs[configIndex].straightSpeed = 8.888889f;
    controlConfigs[configIndex].turnSpeed = 6.666667f;
	controlConfigs[configIndex].speedDerivate = 0.2f;
	controlConfigs[configIndex].slowdownAcceleration = 0.25f;
	controlConfigs[configIndex].linearAcceleration = 0.35f;
	controlConfigs[configIndex].linearBrake = -0.3f;

	createDefaultRowConfig(&controlConfigs[configIndex].rowConfigs, &controlConfigs[configIndex].rowConfigLength);

    // Column Config
    createDefaultColumnConfig(configIndex, controlConfigs);

	// Finish Line Config
    controlConfigs[configIndex].finishLineDetection = true;
    controlConfigs[configIndex].switchConfigAfterFinishLineDetection = true;
    controlConfigs[configIndex].configAfterFinishLineDetected = 4;
    controlConfigs[configIndex].switchConfigAfterFinishLineTimeout = 200;
}

void middleConfig(uint8_t configIndex, ControlConfig* controlConfigs) {
    controlConfigs[configIndex].pixyLedColorR = 255;
    controlConfigs[configIndex].pixyLedColorG = 255;
    controlConfigs[configIndex].pixyLedColorB = 31;

    controlConfigs[configIndex].steeringPotentialFactor = 1.9f;
    controlConfigs[configIndex].steeringPotentialFactorPerSpeed = 0.4f;
    controlConfigs[configIndex].steeringPotentialFactorSpeedIncrements = 35;
    controlConfigs[configIndex].steeringDerivativeFactor = 0.25f;

    controlConfigs[configIndex].brakeRowDistance = 3;
    controlConfigs[configIndex].straightSpeed = 16.0f;
    controlConfigs[configIndex].turnSpeed = 7.777777778f;
	controlConfigs[configIndex].speedDerivate = 0.2f;
	controlConfigs[configIndex].slowdownAcceleration = 0.25f;
	controlConfigs[configIndex].linearAcceleration = 0.4f;
	controlConfigs[configIndex].linearBrake = -0.575f;

	createDefaultRowConfig(&controlConfigs[configIndex].rowConfigs, &controlConfigs[configIndex].rowConfigLength);

    // Column Config
    createDefaultColumnConfig(configIndex, controlConfigs);

	// Finish Line Config
    controlConfigs[configIndex].finishLineDetection = true;
    controlConfigs[configIndex].switchConfigAfterFinishLineDetection = true;
    controlConfigs[configIndex].configAfterFinishLineDetected = 4;
    controlConfigs[configIndex].switchConfigAfterFinishLineTimeout = 200;
}

void fastConfig(uint8_t configIndex, ControlConfig* controlConfigs) {
    controlConfigs[configIndex].pixyLedColorR = 255;
    controlConfigs[configIndex].pixyLedColorG = 255;
    controlConfigs[configIndex].pixyLedColorB = 31;

    controlConfigs[configIndex].steeringPotentialFactor = 1.9f;
    controlConfigs[configIndex].steeringPotentialFactorPerSpeed = 0.4f;
    controlConfigs[configIndex].steeringPotentialFactorSpeedIncrements = 35;
    controlConfigs[configIndex].steeringDerivativeFactor = 0.25f;

    controlConfigs[configIndex].brakeRowDistance = 2;
    controlConfigs[configIndex].straightSpeed = 40.0f;
    controlConfigs[configIndex].turnSpeed = 8.5f;
	controlConfigs[configIndex].speedDerivate = 0.2f;
	controlConfigs[configIndex].slowdownAcceleration = 0.25f;
	controlConfigs[configIndex].linearAcceleration = 1.0f;
	controlConfigs[configIndex].linearBrake = -1.0f;

	createDefaultRowConfig(&controlConfigs[configIndex].rowConfigs, &controlConfigs[configIndex].rowConfigLength);

    // Column Config
    createDefaultColumnConfig(configIndex, controlConfigs);

	// Finish Line Config
    controlConfigs[configIndex].finishLineDetection = true;
    controlConfigs[configIndex].switchConfigAfterFinishLineDetection = true;
    controlConfigs[configIndex].configAfterFinishLineDetected = 4;
    controlConfigs[configIndex].switchConfigAfterFinishLineTimeout = 200;
}

void middleConfigObstacleDetection(uint8_t configIndex, ControlConfig* controlConfigs){
    controlConfigs[configIndex].pixyLedColorR = 255;
    controlConfigs[configIndex].pixyLedColorG = 255;
    controlConfigs[configIndex].pixyLedColorB = 31;

    controlConfigs[configIndex].steeringPotentialFactor = 1.6f;
    controlConfigs[configIndex].steeringPotentialFactorPerSpeed = 0.4f;
    controlConfigs[configIndex].steeringPotentialFactorSpeedIncrements = 35;
    controlConfigs[configIndex].steeringDerivativeFactor = 0.3f;

    controlConfigs[configIndex].brakeRowDistance = 3;
    controlConfigs[configIndex].straightSpeed = 3.0f;
    controlConfigs[configIndex].turnSpeed = 3.0f;
	controlConfigs[configIndex].speedDerivate = 0.2f;
	controlConfigs[configIndex].slowdownAcceleration = 0.25f;
	controlConfigs[configIndex].linearAcceleration = 0.3f;
	controlConfigs[configIndex].linearBrake = -0.5f;

	createDefaultRowConfig(&controlConfigs[configIndex].rowConfigs, &controlConfigs[configIndex].rowConfigLength);

    // Column Config
    createDefaultColumnConfig(configIndex, controlConfigs);

	// Obstacle Detection
	controlConfigs[configIndex].obstacleDetection = true;
	// TODO: Calibrate
	controlConfigs[configIndex].minObstacleRow = 130;
}

void safeConfigWithoutFinishLineDetection(uint8_t configIndex, ControlConfig* controlConfigs) {
	safeConfig(configIndex, controlConfigs);
    controlConfigs[configIndex].finishLineDetection = false;
};

void middleConfigWithoutFinishLineDetection(uint8_t configIndex, ControlConfig* controlConfigs) {
	middleConfig(configIndex, controlConfigs);
    controlConfigs[configIndex].finishLineDetection = false;
};

void fastConfigWithoutFinishLineDetection(uint8_t configIndex, ControlConfig* controlConfigs) {
	fastConfig(configIndex, controlConfigs);
    controlConfigs[configIndex].finishLineDetection = false;
};

void loadControlConfigs(uint8_t* controlConfigsLength, ControlConfig* controlConfigs) {
    *controlConfigsLength = 4;

    for (uint8_t i = 0; i < *controlConfigsLength; i++) {
        controlConfigs[i] = ControlConfig();
    }

    safeConfig(0, controlConfigs);
    middleConfig(1, controlConfigs);
    fastConfig(2, controlConfigs);
	// safeConfigWithoutFinishLineDetection(0, controlConfigs);
	// middleConfigWithoutFinishLineDetection(1, controlConfigs);
	// fastConfigWithoutFinishLineDetection(2, controlConfigs);

    middleConfigObstacleDetection(3, controlConfigs);
    middleConfigObstacleDetection(4, controlConfigs);
}
