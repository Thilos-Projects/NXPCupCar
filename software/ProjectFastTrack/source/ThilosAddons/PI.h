/*
 * Pid.h
 *
 *  Created on: 05.12.2023
 *      Author: TFR
 */

#ifndef THILOSADDONS_PI_H_
#define THILOSADDONS_PI_H_

class PI{
private:
	float lookupTable[11] = {
			0.0f,					//0.0-0.1
			0.35f,					//0.1-0.2
			0.38f,					//0.2-0.3
			0.42f,					//0.3-0.4
			0.45f,					//0.4-0.5
			0.475f,					//0.5-0.6
			0.51f,					//0.6-0.7
			0.54f,					//0.7-0.8
			0.58f,					//0.8-0.9
			0.605f,					//0.9-1.0
			0.61f,					//end Point
	};

	float integralFactor, integralAdder, outMin, outMax;
	float previousError = 0;

public:
	void setup(float integralFactor, float derivitiveFactor, float outMin, float outMax);
	float doTick(float istPoint, float sollPoint, float deltaTime);
};

#endif /* THILOSADDONS_PI_H_ */
