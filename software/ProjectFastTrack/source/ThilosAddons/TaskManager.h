/*
 * TaskManager.h
 *
 *  Created on: 13.11.2023
 *      Author: TFR
 */

//Requires Pit3 !!!

#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include "Utils/def.h"

#define minTaskTimeInMs 0.001f
#define maxTaskCount 5

typedef void (*CallbackFunc)(void);
typedef struct {
	bool isFree;
	bool active;
	UInt32 delay;					//in minTaskTime
	UInt32 nextActivationAt;		//in minTaskTime
	CallbackFunc functionToCall;
} taskHandle;

void Setup();

void Interupt();
UInt32 getMicros();
UInt32 getMillis();

void Update();
taskHandle* getTaskHandle();

#endif /* TASKMANAGER_H_ */
