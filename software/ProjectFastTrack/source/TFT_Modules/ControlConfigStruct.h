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

struct BatteryLevelLookupEntry {
    float batteryLevel = 0.0f;
    float accelerationFactor = 1.0f;
};

struct ControlConfig {
    uint16_t timePerFrame = 17;

    // Pixy-Stuff
    uint8_t pixyLedColorR = 255, pixyLedColorG = 255, pixyLedColorB = 255;
    uint16_t pixyLamps = 0x0101; // 8-Bit upper | 8-Bit lower
    const char* cameraProgram = "video";

    // Camera-Stuff
    uint8_t rowConfigLength = 0;
    RowConfig* rowConfigs;

    // Steering-Stuff
    float servoSteeringOffset = 0.0f;
    float steeringPotentialFactor = 0.0f;
    float steeringDerivativeFactor = 0.0f;

    // Speed-Stuff
    uint8_t brakeFrameCount = 0;
    uint8_t brakeRowDistance = 0;
    float brakeSpeed = 0.0f;
    float straightSpeed = 0.0f;
    float turnSpeed = 0.0f;

    // Speed-Battery-Control
    uint16_t batteryLevelCheckInterval = 1000;
    uint8_t batteryLevelLookupLength = 0;
    BatteryLevelLookupEntry* batteryLevelLookup; // From top down (first entry highest voltage)
};

#endif
