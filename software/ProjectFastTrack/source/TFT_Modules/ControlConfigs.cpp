#include "ControlConfigs.h"
#include "ControlConfigStruct.h"

void loadControlConfigs(uint8_t* controlConfigsLength, ControlConfig* controlConfigs) {
    *controlConfigsLength = 1;

    // *controlConfigs = (ControlConfig*)malloc(sizeof(ControlConfig) * *controlConfigsLength);

    controlConfigs[0].timePerFrame = 17;
    controlConfigs[0].servoSteeringOffset = 0.0f;
    controlConfigs[0].speedMin = 0.4f;
    controlConfigs[0].speedMax = 0.55f;
    controlConfigs[0].speedAdjustTIme = 500.0f;
    controlConfigs[0].maxCenterDiffForSpeedUp = 5;
    controlConfigs[0].pixyLedColorR = 255;
    controlConfigs[0].pixyLedColorG = 127;
    controlConfigs[0].pixyLedColorB = 0;
    controlConfigs[0].pixyLamps = 0x0101;
    controlConfigs[0].cameraProgram = "video";
    controlConfigs[0].steeringFactor = 3.0f;

    controlConfigs[0].rowConfigLength = 2;
    controlConfigs[0].rowConfigs = (RowConfig*)malloc(sizeof(RowConfig) * controlConfigs[0].rowConfigLength);
    // First Row Config
    controlConfigs[0].rowConfigs[0].row = 150;
    controlConfigs[0].rowConfigs[0].edgeThreshold = 40;
    controlConfigs[0].rowConfigs[0].minEdgeWidth = 0;
    controlConfigs[0].rowConfigs[0].maxEdgeWidth = 6;
    controlConfigs[0].rowConfigs[0].centerPixel = 158;
    controlConfigs[0].rowConfigs[0].minThickness = 4;
    // Second Row Config
    controlConfigs[0].rowConfigs[1].row = 100;
    controlConfigs[0].rowConfigs[1].edgeThreshold = 40;
    controlConfigs[0].rowConfigs[1].minEdgeWidth = 0;
    controlConfigs[0].rowConfigs[1].maxEdgeWidth = 6;
    controlConfigs[0].rowConfigs[1].centerPixel= 158;
    controlConfigs[0].rowConfigs[1].minThickness = 4;
}
