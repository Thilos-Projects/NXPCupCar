#include "ControlConfigs.h"
#include "ControlConfigStruct.h"

void controllConfig(uint8_t configIndex, ControlConfig* controlConfigs){
    controlConfigs[configIndex].timePerFrame = 17;
    controlConfigs[configIndex].servoSteeringOffset = -0.1f;
    controlConfigs[configIndex].pixyLedColorR = 255;
    controlConfigs[configIndex].pixyLedColorG = 127;
    controlConfigs[configIndex].pixyLedColorB = 0;
    controlConfigs[configIndex].pixyLamps = 0x0101;
    controlConfigs[configIndex].cameraProgram = "video";
    controlConfigs[configIndex].steeringPotentialFactor = 2.0f;
    controlConfigs[configIndex].steeringDerivativeFactor = 0.0f;
    controlConfigs[configIndex].steeringHoldframesAfterTurn = 0;
    controlConfigs[configIndex].brakeFrameCount = 3;
    controlConfigs[configIndex].brakeFrameCooldown = 0;
    controlConfigs[configIndex].brakeRowDistance = 2;
    controlConfigs[configIndex].brakeSpeed = 0.25f;
    controlConfigs[configIndex].straightSpeed = 0.25f;
    controlConfigs[configIndex].turnSpeed = 0.25f;

    controlConfigs[configIndex].rowConfigLength = 6;
    controlConfigs[configIndex].rowConfigs = (RowConfig*)malloc(sizeof(RowConfig) * controlConfigs[configIndex].rowConfigLength);
    // First Row Config
    controlConfigs[configIndex].rowConfigs[0].row = 150;
    controlConfigs[configIndex].rowConfigs[0].rowClose = 150;
    controlConfigs[configIndex].rowConfigs[0].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[0].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[0].maxEdgeWidth = 12;
    controlConfigs[configIndex].rowConfigs[0].centerPixel = 158;
    controlConfigs[configIndex].rowConfigs[0].minThickness = 4;
    controlConfigs[configIndex].rowConfigs[0].maxCenterDifferenceForTurn = 20;
    controlConfigs[configIndex].rowConfigs[0].weight = 10.0f;
    // Second Row Config
    controlConfigs[configIndex].rowConfigs[1].row = 118;
    controlConfigs[configIndex].rowConfigs[1].rowClose = 118;
    controlConfigs[configIndex].rowConfigs[1].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[1].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[1].maxEdgeWidth = 5;
    controlConfigs[configIndex].rowConfigs[1].centerPixel= 158;
    controlConfigs[configIndex].rowConfigs[1].minThickness = 4;
    controlConfigs[configIndex].rowConfigs[1].maxCenterDifferenceForTurn = 20;
    controlConfigs[configIndex].rowConfigs[1].weight = 8.0f;
    // Third Row Config
    controlConfigs[configIndex].rowConfigs[2].row = 96;
    controlConfigs[configIndex].rowConfigs[2].rowClose = 96;
    controlConfigs[configIndex].rowConfigs[2].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[2].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[2].maxEdgeWidth = 5;
    controlConfigs[configIndex].rowConfigs[2].centerPixel= 158;
    controlConfigs[configIndex].rowConfigs[2].minThickness = 3;
    controlConfigs[configIndex].rowConfigs[2].maxCenterDifferenceForTurn = 15;
    controlConfigs[configIndex].rowConfigs[2].weight = 6.0f;
    // Fourth Row Config
    controlConfigs[configIndex].rowConfigs[3].row = 82;
    controlConfigs[configIndex].rowConfigs[3].rowClose = 77;
    controlConfigs[configIndex].rowConfigs[3].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[3].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[3].maxEdgeWidth = 4;
    controlConfigs[configIndex].rowConfigs[3].centerPixel= 158;
    controlConfigs[configIndex].rowConfigs[3].minThickness = 2;
    controlConfigs[configIndex].rowConfigs[3].maxCenterDifferenceForTurn = 15;
    controlConfigs[configIndex].rowConfigs[3].weight = 4.0f;
    // Fifth Row Config
    controlConfigs[configIndex].rowConfigs[4].row = 74;
    controlConfigs[configIndex].rowConfigs[4].rowClose = 69;
    controlConfigs[configIndex].rowConfigs[4].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[4].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[4].maxEdgeWidth = 4;
    controlConfigs[configIndex].rowConfigs[4].centerPixel= 158;
    controlConfigs[configIndex].rowConfigs[4].minThickness = 1;
    controlConfigs[configIndex].rowConfigs[4].maxCenterDifferenceForTurn = 10;
    controlConfigs[configIndex].rowConfigs[4].weight = 2.0f;
    // Sixth Row Config
    controlConfigs[configIndex].rowConfigs[5].row = 58;
    controlConfigs[configIndex].rowConfigs[5].rowClose = 53;
    controlConfigs[configIndex].rowConfigs[5].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[5].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[5].maxEdgeWidth = 3;
    controlConfigs[configIndex].rowConfigs[5].centerPixel= 158;
    controlConfigs[configIndex].rowConfigs[5].minThickness = 0;
    controlConfigs[configIndex].rowConfigs[5].maxCenterDifferenceForTurn = 10;
    controlConfigs[configIndex].rowConfigs[5].weight = 1.0f;

    // Battery - SORTED!
    controlConfigs[configIndex].batteryLevelCheckInterval = 1000;
    controlConfigs[configIndex].batteryLevelLookupLength = 3;
    controlConfigs[configIndex].batteryLevelLookup = (BatteryLevelLookupEntry*)malloc(sizeof(BatteryLevelLookupEntry) * controlConfigs[configIndex].batteryLevelLookupLength);
    controlConfigs[configIndex].batteryLevelLookup[0].batteryLevel = 13.0f;
    controlConfigs[configIndex].batteryLevelLookup[0].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[1].batteryLevel = 8.5f;
    controlConfigs[configIndex].batteryLevelLookup[1].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[2].batteryLevel = 7.3f;
    controlConfigs[configIndex].batteryLevelLookup[2].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[3].batteryLevel = 6.9f;
    controlConfigs[configIndex].batteryLevelLookup[3].accelerationFactor = 1.0f;

}

void guteConfig0(uint8_t configIndex, ControlConfig* controlConfigs){
    controlConfigs[configIndex].timePerFrame = 17;
    controlConfigs[configIndex].servoSteeringOffset = -0.0f;
    controlConfigs[configIndex].pixyLedColorR = 255;
    controlConfigs[configIndex].pixyLedColorG = 127;
    controlConfigs[configIndex].pixyLedColorB = 0;
    controlConfigs[configIndex].pixyLamps = 0x0101;
    controlConfigs[configIndex].cameraProgram = "video";
    controlConfigs[configIndex].steeringPotentialFactor = 3.0f;
    controlConfigs[configIndex].steeringDerivativeFactor = 1.0f;
    controlConfigs[configIndex].steeringHoldframesAfterTurn = 0;
    controlConfigs[configIndex].brakeFrameCount = 6;
    controlConfigs[configIndex].brakeFrameCooldown = 60;
    controlConfigs[configIndex].brakeRowDistance = 3;
    controlConfigs[configIndex].brakeSpeed = -0.5f;
    controlConfigs[configIndex].straightSpeed = 0.55f;
    controlConfigs[configIndex].turnSpeed = 0.42f;

    controlConfigs[configIndex].rowConfigLength = 6;
    controlConfigs[configIndex].rowConfigs = (RowConfig*)malloc(sizeof(RowConfig) * controlConfigs[configIndex].rowConfigLength);
    // First Row Config
    controlConfigs[configIndex].rowConfigs[0].row = 150;
    controlConfigs[configIndex].rowConfigs[0].rowClose = 145;
    controlConfigs[configIndex].rowConfigs[0].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[0].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[0].maxEdgeWidth = 6;
    controlConfigs[configIndex].rowConfigs[0].centerPixel = 158;
    controlConfigs[configIndex].rowConfigs[0].minThickness = 4;
    controlConfigs[configIndex].rowConfigs[0].maxCenterDifferenceForTurn = 30;
    // Second Row Config
    controlConfigs[configIndex].rowConfigs[1].row = 118;
    controlConfigs[configIndex].rowConfigs[1].rowClose = 113;
    controlConfigs[configIndex].rowConfigs[1].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[1].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[1].maxEdgeWidth = 5;
    controlConfigs[configIndex].rowConfigs[1].centerPixel= 158;
    controlConfigs[configIndex].rowConfigs[1].minThickness = 4;
    controlConfigs[configIndex].rowConfigs[1].maxCenterDifferenceForTurn = 30;
    // Third Row Config
    controlConfigs[configIndex].rowConfigs[2].row = 96;
    controlConfigs[configIndex].rowConfigs[2].rowClose = 91;
    controlConfigs[configIndex].rowConfigs[2].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[2].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[2].maxEdgeWidth = 5;
    controlConfigs[configIndex].rowConfigs[2].centerPixel= 158;
    controlConfigs[configIndex].rowConfigs[2].minThickness = 3;
    controlConfigs[configIndex].rowConfigs[2].maxCenterDifferenceForTurn = 15;
    // Fourth Row Config
    controlConfigs[configIndex].rowConfigs[3].row = 82;
    controlConfigs[configIndex].rowConfigs[3].rowClose = 77;
    controlConfigs[configIndex].rowConfigs[3].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[3].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[3].maxEdgeWidth = 4;
    controlConfigs[configIndex].rowConfigs[3].centerPixel= 158;
    controlConfigs[configIndex].rowConfigs[3].minThickness = 2;
    controlConfigs[configIndex].rowConfigs[3].maxCenterDifferenceForTurn = 15;
    // Fifth Row Config
    controlConfigs[configIndex].rowConfigs[4].row = 74;
    controlConfigs[configIndex].rowConfigs[4].rowClose = 69;
    controlConfigs[configIndex].rowConfigs[4].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[4].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[4].maxEdgeWidth = 4;
    controlConfigs[configIndex].rowConfigs[4].centerPixel= 158;
    controlConfigs[configIndex].rowConfigs[4].minThickness = 1;
    controlConfigs[configIndex].rowConfigs[4].maxCenterDifferenceForTurn = 10;
    // Sixth Row Config
    controlConfigs[configIndex].rowConfigs[5].row = 58;
    controlConfigs[configIndex].rowConfigs[5].rowClose = 53;
    controlConfigs[configIndex].rowConfigs[5].edgeThreshold = 40;
    controlConfigs[configIndex].rowConfigs[5].minEdgeWidth = 0;
    controlConfigs[configIndex].rowConfigs[5].maxEdgeWidth = 3;
    controlConfigs[configIndex].rowConfigs[5].centerPixel= 158;
    controlConfigs[configIndex].rowConfigs[5].minThickness = 0;
    controlConfigs[configIndex].rowConfigs[5].maxCenterDifferenceForTurn = 10;

    // Battery - SORTED!
    controlConfigs[configIndex].batteryLevelCheckInterval = 1000;
    controlConfigs[configIndex].batteryLevelLookupLength = 2;
    controlConfigs[configIndex].batteryLevelLookup = (BatteryLevelLookupEntry*)malloc(sizeof(BatteryLevelLookupEntry) * controlConfigs[configIndex].batteryLevelLookupLength);
    controlConfigs[configIndex].batteryLevelLookup[0].batteryLevel = 8.5f;
    controlConfigs[configIndex].batteryLevelLookup[0].accelerationFactor = 1.0f;
    controlConfigs[configIndex].batteryLevelLookup[1].batteryLevel = 6.9f;
    controlConfigs[configIndex].batteryLevelLookup[1].accelerationFactor = 1.0f;
}

void guteConfig1(uint8_t configIndex, ControlConfig* controlConfigs){
	controlConfigs[configIndex].timePerFrame = 17;
	controlConfigs[configIndex].servoSteeringOffset = -0.0f;
	controlConfigs[configIndex].pixyLedColorR = 255;
	controlConfigs[configIndex].pixyLedColorG = 127;
	controlConfigs[configIndex].pixyLedColorB = 0;
	controlConfigs[configIndex].pixyLamps = 0x0101;
	controlConfigs[configIndex].cameraProgram = "video";
	controlConfigs[configIndex].steeringPotentialFactor = 3.0f;
	controlConfigs[configIndex].steeringDerivativeFactor = 1.0f;
    controlConfigs[configIndex].steeringHoldframesAfterTurn = 0;
	controlConfigs[configIndex].brakeFrameCount = 6;
	controlConfigs[configIndex].brakeFrameCooldown = 30;		//changed 60
	controlConfigs[configIndex].brakeRowDistance = 2;			//changed 3
	controlConfigs[configIndex].brakeSpeed = -0.5f;
	controlConfigs[configIndex].straightSpeed = 0.6f;			//chnaged 0.55
	controlConfigs[configIndex].turnSpeed = 0.42f;				//changed 4.2

	controlConfigs[configIndex].rowConfigLength = 6;
	controlConfigs[configIndex].rowConfigs = (RowConfig*)malloc(sizeof(RowConfig) * controlConfigs[configIndex].rowConfigLength);
	// First Row Config
	controlConfigs[configIndex].rowConfigs[0].row = 150;
	controlConfigs[configIndex].rowConfigs[0].rowClose = 145;
	controlConfigs[configIndex].rowConfigs[0].edgeThreshold = 40;
	controlConfigs[configIndex].rowConfigs[0].minEdgeWidth = 0;
	controlConfigs[configIndex].rowConfigs[0].maxEdgeWidth = 6;
	controlConfigs[configIndex].rowConfigs[0].centerPixel = 158;
	controlConfigs[configIndex].rowConfigs[0].minThickness = 4;
	controlConfigs[configIndex].rowConfigs[0].maxCenterDifferenceForTurn = 30;
	// Second Row Config
	controlConfigs[configIndex].rowConfigs[1].row = 118;
	controlConfigs[configIndex].rowConfigs[1].rowClose = 113;
	controlConfigs[configIndex].rowConfigs[1].edgeThreshold = 40;
	controlConfigs[configIndex].rowConfigs[1].minEdgeWidth = 0;
	controlConfigs[configIndex].rowConfigs[1].maxEdgeWidth = 5;
	controlConfigs[configIndex].rowConfigs[1].centerPixel= 158;
	controlConfigs[configIndex].rowConfigs[1].minThickness = 4;
	controlConfigs[configIndex].rowConfigs[1].maxCenterDifferenceForTurn = 30;
	// Third Row Config
	controlConfigs[configIndex].rowConfigs[2].row = 96;
	controlConfigs[configIndex].rowConfigs[2].rowClose = 91;
	controlConfigs[configIndex].rowConfigs[2].edgeThreshold = 40;
	controlConfigs[configIndex].rowConfigs[2].minEdgeWidth = 0;
	controlConfigs[configIndex].rowConfigs[2].maxEdgeWidth = 5;
	controlConfigs[configIndex].rowConfigs[2].centerPixel= 158;
	controlConfigs[configIndex].rowConfigs[2].minThickness = 3;
	controlConfigs[configIndex].rowConfigs[2].maxCenterDifferenceForTurn = 15;
	// Fourth Row Config
	controlConfigs[configIndex].rowConfigs[3].row = 82;
	controlConfigs[configIndex].rowConfigs[3].rowClose = 77;
	controlConfigs[configIndex].rowConfigs[3].edgeThreshold = 40;
	controlConfigs[configIndex].rowConfigs[3].minEdgeWidth = 0;
	controlConfigs[configIndex].rowConfigs[3].maxEdgeWidth = 4;
	controlConfigs[configIndex].rowConfigs[3].centerPixel= 158;
	controlConfigs[configIndex].rowConfigs[3].minThickness = 2;
	controlConfigs[configIndex].rowConfigs[3].maxCenterDifferenceForTurn = 15;
	// Fifth Row Config
	controlConfigs[configIndex].rowConfigs[4].row = 74;
	controlConfigs[configIndex].rowConfigs[4].rowClose = 69;
	controlConfigs[configIndex].rowConfigs[4].edgeThreshold = 40;
	controlConfigs[configIndex].rowConfigs[4].minEdgeWidth = 0;
	controlConfigs[configIndex].rowConfigs[4].maxEdgeWidth = 4;
	controlConfigs[configIndex].rowConfigs[4].centerPixel= 158;
	controlConfigs[configIndex].rowConfigs[4].minThickness = 1;
	controlConfigs[configIndex].rowConfigs[4].maxCenterDifferenceForTurn = 10;
	// Sixth Row Config
	controlConfigs[configIndex].rowConfigs[5].row = 58;
	controlConfigs[configIndex].rowConfigs[5].rowClose = 53;
	controlConfigs[configIndex].rowConfigs[5].edgeThreshold = 40;
	controlConfigs[configIndex].rowConfigs[5].minEdgeWidth = 0;
	controlConfigs[configIndex].rowConfigs[5].maxEdgeWidth = 3;
	controlConfigs[configIndex].rowConfigs[5].centerPixel= 158;
	controlConfigs[configIndex].rowConfigs[5].minThickness = 0;
	controlConfigs[configIndex].rowConfigs[5].maxCenterDifferenceForTurn = 10;

	// Battery - SORTED!
	controlConfigs[configIndex].batteryLevelCheckInterval = 1000;
	controlConfigs[configIndex].batteryLevelLookupLength = 2;
	controlConfigs[configIndex].batteryLevelLookup = (BatteryLevelLookupEntry*)malloc(sizeof(BatteryLevelLookupEntry) * controlConfigs[configIndex].batteryLevelLookupLength);
	controlConfigs[configIndex].batteryLevelLookup[0].batteryLevel = 8.5f;
	controlConfigs[configIndex].batteryLevelLookup[0].accelerationFactor = 1.0f;
	controlConfigs[configIndex].batteryLevelLookup[1].batteryLevel = 6.9f;
	controlConfigs[configIndex].batteryLevelLookup[1].accelerationFactor = 1.0f;
}

void loadControlConfigs(uint8_t* controlConfigsLength, ControlConfig* controlConfigs) {
    *controlConfigsLength = 3;

    guteConfig1(2, controlConfigs);
    guteConfig0(1, controlConfigs);
    controllConfig(0, controlConfigs);
}