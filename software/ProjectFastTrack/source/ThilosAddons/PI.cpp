/*
 * Pid.cpp
 *
 *  Created on: 05.12.2023
 *      Author: TFR
 */

#include <stdint.h>
#include <stdio.h>
#include <ThilosAddons/PI.h>

	void PI::setup(float integralFactor, float startError, float outMin, float outMax){
		this->integralFactor = integralFactor;
		this->integralAdder = startError;
		this->outMin = outMin;
		this->outMax = outMax;
	}

	float PI::doTick(float istPoint, float sollPoint, float deltaTime){
		uint8_t parts = 10;

		float uSP = sollPoint < 0 ? -sollPoint : sollPoint;
		float sign = sollPoint / uSP;
		uint8_t first = ((uint8_t)(uSP*parts))%parts;
		uint8_t next = first + 1;

		float firstF = lookupTable[first];
		float nextF = lookupTable[next];

		float d = uSP - (first / parts);

		float output = (firstF + (nextF-firstF) * d) * sign;

		if(-0.01f > sollPoint || sollPoint > 0.01f){
			float error = (sollPoint < 0 ? -sollPoint : sollPoint) - (istPoint < 0 ? -istPoint : istPoint);
			integralAdder += error * deltaTime * 0.5f;
			float Iout = integralFactor * integralAdder;
			output *= Iout;
		}

		if(outMin > output)
			return outMin;
		if(outMax < output)
			return outMax;

		return output;
	}
