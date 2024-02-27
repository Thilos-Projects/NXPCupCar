#include "ControlConfigs.h"
#include "ControlConfigStruct.h"

void loadControlConfigs(uint8_t* controlConfigsLength, ControlConfig* controlConfigs) {
    *controlConfigsLength = 1;

    // *controlConfigs = (ControlConfig*)malloc(sizeof(ControlConfig) * *controlConfigsLength);

    controlConfigs[0].timePerFrame = 17;
    controlConfigs[0].servoSteeringOffset = 0.0f;
    controlConfigs[0].pixyLedColorR = 255;
    controlConfigs[0].pixyLedColorG = 127;
    controlConfigs[0].pixyLedColorB = 0;
    controlConfigs[0].pixyLamps = 0x0101;
    controlConfigs[0].cameraProgram = "video";
    controlConfigs[0].steeringFactor = 3.0f;
    controlConfigs[0].steeringDerivativeFactor = 1.0f;
    controlConfigs[0].brakeFrameCount = 3;
    controlConfigs[0].brakeRowDistance = 2;
    controlConfigs[0].brakeSpeed = -0.3f;
    controlConfigs[0].straightSpeed = 0.55f;
    controlConfigs[0].turnSpeed = 0.4f;

    controlConfigs[0].rowConfigLength = 6;
    controlConfigs[0].rowConfigs = (RowConfig*)malloc(sizeof(RowConfig) * controlConfigs[0].rowConfigLength);
    // First Row Config
    controlConfigs[0].rowConfigs[0].row = 150;
    controlConfigs[0].rowConfigs[0].rowClose = 145;
    controlConfigs[0].rowConfigs[0].edgeThreshold = 40;
    controlConfigs[0].rowConfigs[0].minEdgeWidth = 0;
    controlConfigs[0].rowConfigs[0].maxEdgeWidth = 6;
    controlConfigs[0].rowConfigs[0].centerPixel = 158;
    controlConfigs[0].rowConfigs[0].minThickness = 4;
    controlConfigs[0].rowConfigs[0].maxCenterDifferenceForTurn = 20;
    // Second Row Config
    controlConfigs[0].rowConfigs[1].row = 118;
    controlConfigs[0].rowConfigs[1].rowClose = 113;
    controlConfigs[0].rowConfigs[1].edgeThreshold = 40;
    controlConfigs[0].rowConfigs[1].minEdgeWidth = 0;
    controlConfigs[0].rowConfigs[1].maxEdgeWidth = 5;
    controlConfigs[0].rowConfigs[1].centerPixel= 158;
    controlConfigs[0].rowConfigs[1].minThickness = 4;
    controlConfigs[0].rowConfigs[1].maxCenterDifferenceForTurn = 20;
    // Third Row Config
    controlConfigs[0].rowConfigs[2].row = 96;
    controlConfigs[0].rowConfigs[2].rowClose = 91;
    controlConfigs[0].rowConfigs[2].edgeThreshold = 40;
    controlConfigs[0].rowConfigs[2].minEdgeWidth = 0;
    controlConfigs[0].rowConfigs[2].maxEdgeWidth = 5;
    controlConfigs[0].rowConfigs[2].centerPixel= 158;
    controlConfigs[0].rowConfigs[2].minThickness = 3;
    controlConfigs[0].rowConfigs[2].maxCenterDifferenceForTurn = 15;
    // Fourth Row Config
    controlConfigs[0].rowConfigs[3].row = 82;
    controlConfigs[0].rowConfigs[3].rowClose = 77;
    controlConfigs[0].rowConfigs[3].edgeThreshold = 40;
    controlConfigs[0].rowConfigs[3].minEdgeWidth = 0;
    controlConfigs[0].rowConfigs[3].maxEdgeWidth = 4;
    controlConfigs[0].rowConfigs[3].centerPixel= 158;
    controlConfigs[0].rowConfigs[3].minThickness = 2;
    controlConfigs[0].rowConfigs[3].maxCenterDifferenceForTurn = 15;
    // Fifth Row Config
    controlConfigs[0].rowConfigs[4].row = 74;
    controlConfigs[0].rowConfigs[4].rowClose = 69;
    controlConfigs[0].rowConfigs[4].edgeThreshold = 40;
    controlConfigs[0].rowConfigs[4].minEdgeWidth = 0;
    controlConfigs[0].rowConfigs[4].maxEdgeWidth = 4;
    controlConfigs[0].rowConfigs[4].centerPixel= 158;
    controlConfigs[0].rowConfigs[4].minThickness = 1;
    controlConfigs[0].rowConfigs[4].maxCenterDifferenceForTurn = 10;
    // Sixth Row Config
    controlConfigs[0].rowConfigs[5].row = 58;
    controlConfigs[0].rowConfigs[5].rowClose = 53;
    controlConfigs[0].rowConfigs[5].edgeThreshold = 40;
    controlConfigs[0].rowConfigs[5].minEdgeWidth = 0;
    controlConfigs[0].rowConfigs[5].maxEdgeWidth = 3;
    controlConfigs[0].rowConfigs[5].centerPixel= 158;
    controlConfigs[0].rowConfigs[5].minThickness = 0;
    controlConfigs[0].rowConfigs[5].maxCenterDifferenceForTurn = 10;
}
