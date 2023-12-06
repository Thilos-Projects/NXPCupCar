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
		float Pout = lookupTable[(uint8_t)(sollPoint*10)%10];					//lookup
		float next = lookupTable[(uint8_t)(sollPoint*10)%10 + 1];
		float d = sollPoint - (((uint8_t)(sollPoint*10)%10)/10.0f);

		float output = Pout + (next-Pout)*d;

		if(-0.01f > sollPoint || sollPoint > 0.01f){
			float error = (sollPoint < 0 ? -sollPoint : sollPoint) - (istPoint < 0 ? -istPoint : istPoint);
			integralAdder += error * deltaTime * 0.001f;
			float Iout = integralFactor * integralAdder;
			output *= Iout;
		}

		if(outMin > output)
			return outMin;
		if(outMax < output)
			return outMax;

		return output;
	}
