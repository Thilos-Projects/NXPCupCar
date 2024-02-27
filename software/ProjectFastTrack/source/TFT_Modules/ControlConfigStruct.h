#ifndef _TFTMODULES_CONTROLCONFIGSTRUCT_H_
#define _TFTMODULES_CONTROLCONFIGSTRUCT_H_
extern "C"
{
#include <string.h>
#include <stdint.h>
}

struct RowConfig {
    uint16_t row = 0;
    uint16_t rowClose = 0;
    uint16_t edgeThreshold = 0;
    uint8_t minEdgeWidth = 0;
    uint8_t maxEdgeWidth = 0;
    uint16_t centerPixel = 158;
    uint16_t minThickness = 0;
    uint8_t maxCenterDifferenceForTurn = 0;
};

struct ControlConfig {
    uint16_t timePerFrame = 17;
    float servoSteeringOffset = 0.0f;
    uint8_t pixyLedColorR = 255, pixyLedColorG = 255, pixyLedColorB = 255;
    uint16_t pixyLamps = 0x0101; // 8-Bit upper | 8-Bit lower
    const char* cameraProgram = "video";
    uint8_t rowConfigLength = 0;
    RowConfig* rowConfigs;
    float steeringFactor = 0.0f;
    float steeringDerivativeFactor = 0.0f;
    uint8_t brakeFrameCount = 0;
    uint8_t brakeRowDistance = 0;
    float brakeSpeed = 0.0f;
    float straightSpeed = 0.0f;
    float turnSpeed = 0.0f;
};

#endif
