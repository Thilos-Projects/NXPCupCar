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
    uint16_t minThickness = 0;
    uint8_t maxCenterDifferenceForTurn = 0;
    uint16_t maxTrackWidth = 316;
    float weight = 1.0f;
};

struct ColumnConfig {
    uint16_t column = 158;
    uint16_t edgeThreshold = 0;
    uint8_t minEdgeWidth = 0;
    uint8_t maxEdgeWidth = 0;
    uint16_t minThickness = 0;
};

struct BatteryLevelLookupEntry {
    float batteryLevel = 0.0f;
    float accelerationFactor = 1.0f;
    bool disableWhenLower = false;
};

struct BrakeLookupEntry {
    float minSpeedDifference = 0.0f;
    uint8_t frameCount = 0;
    float acceleration = -0.0f;
};

struct AccelerationLookupEntry {
    float minSpeedDifference = 0.0f;
    float acceleration = 0.0f;
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
    ColumnConfig columnConfig;

    // Finish Line Detection
    bool finishLineDetection = false;
    bool switchConfigAfterFinishLineDetection = false;
    uint8_t configAfterFinishLineDetected;
    uint16_t switchConfigAfterFinishLineTimeout = 0;
    uint16_t startFinishLineDetectionAfter = 5000;
    uint8_t finishLineLeftOffset = 35, finishLineRightOffset = 35;

    // Obstacle Detection
    bool obstacleDetection = false;
    uint8_t minObstacleRow = 0;

    // Steering-Stuff
    uint16_t trackCenter = 154; //TODO: Should be 158 when camera is position correctly!
    float servoSteeringOffset = -0.015f;
    float steeringPotentialFactor = 0.0f;
    float steeringPotentialFactorPerSpeed = 0.0f;
    uint8_t steeringPotentialFactorSpeedIncrements = 1;
    float steeringDerivativeFactor = 0.0f;

    // Speed v2
    uint8_t brakeRowDistance = 0;
    float straightSpeed = 0.0f;
    float turnSpeed = 0.0f;
    float slowdownAcceleration = 0.0f;
    uint8_t brakeLookupEntryCount = 0;
    BrakeLookupEntry* brakeLookupEntries;
    uint8_t acceleatationLookupEntryCount = 0;
    AccelerationLookupEntry* acceleatationLookupEntries;
    float speedDerivate = 0.0f;
    float linearAcceleration = 0.35f;
    float linearBrake = -0.3f;
    uint8_t stopAfterFrames = 11;
};

#endif
