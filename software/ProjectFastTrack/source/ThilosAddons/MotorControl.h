/*
 * MotorControll.h
 *
 *  Created on: 30.11.2023
 *      Author: TFR
 */

#ifndef _MOTORCONTROLL_H_
#define _MOTORCONTROLL_H_

namespace MotorControl{
	void Setup();
	void setServo(float angle);
	void getSpeed(float *left, float *right);
	void setSpeed(float left, float right);

	void doSpeedCalc();
	void Update();
}

#endif /* _MOTORCONTROLL_H_ */