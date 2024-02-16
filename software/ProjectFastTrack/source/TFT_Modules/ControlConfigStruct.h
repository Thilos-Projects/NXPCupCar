#ifndef _TFTMODULES_CONTROLCONFIGSTRUCT_H_
#define _TFTMODULES_CONTROLCONFIGSTRUCT_H_
extern "C"
{
#include <string.h>
#include <stdint.h>
}

struct RowConfig {
    uint16_t row = 0;
    uint16_t edgeThreshold = 0;
    uint8_t minEdgeWidth = 0;
    uint8_t maxEdgeWidth = 0;
    uint16_t centerPixel = 158;
    uint16_t minThickness = 0;
};

struct ControlConfig {
    uint16_t timePerFrame = 17;
    float servoSteeringOffset = 0.0f;
    float speedMin = 0.0f;
    float speedMax = 0.0f;
    float speedAdjustTIme = 1000.0f;
    uint16_t maxCenterDiffForSpeedUp = 0;
    uint8_t pixyLedColorR = 255, pixyLeadColorG = 255, pixyLedColorB = 255;
    const char* cameraProgram = "video";
    uint8_t rowConfigLength = 0;
    RowConfig* rowConfigs;
    float steeringFactor = 0.0f;
};

#endif
