/*
 * TaskManager.cpp
 *
 *  Created on: 13.11.2023
 *      Author: TFR
 */

#include <TFT_Modules/Scheduler.h>
extern "C" {
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <math.h>
	#include "MK64F12.h"
	#include "Utils/def.h"
}
#include <stdint.h>

#define countsProMs 1.00f
#define maxTaskCount 12

uint32_t counter;
Scheduler::taskHandle handles[maxTaskCount];

//macht den setup und started den Timer 3(PIT3)
void Scheduler::Setup(){
	counter = 0;

	for(uint16_t i = 0; i < maxTaskCount; i++) {
		handles[i].isFree = true;
	}

	//erstellt einen Timer auf Pit3 der alle ... triggert
	//Copied from iPit.c
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	PIT->CHANNEL[3].LDVAL = kClockPeriphkHz / countsProMs;
	PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TIE_MASK;
	NVIC_ClearPendingIRQ(PIT3_IRQn);
	NVIC_EnableIRQ(PIT3_IRQn);
	NVIC_SetPriority(PIT3_IRQn, kPIT_IntPriority);
	PIT->MCR &= (~PIT_MCR_MDIS_MASK);
	PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;
}

uint32_t Scheduler::getClock(){
	return counter;
}
float Scheduler::getMillis(){
	return counter / countsProMs;
}

//wird jeden frame aufgerufen, und fürt zu gegebener zeit die einzelnen callbacks aus.
void Scheduler::Update(){
	for(uint16_t i = 0; i < maxTaskCount; i++) {
		if(!handles[i].isFree && handles[i].active && handles[i].nextActivationAt < counter){
			handles[i].functionToCall(&handles[i]);
			handles[i].nextActivationAt = handles[i].nextActivationAt + handles[i].delay;
		}
	}
}

void Scheduler::SetActive(Scheduler::taskHandle* task, bool active, bool imidiate) {
	if (task->isFree || task->active == active)
		return;
	task->nextActivationAt = counter + (imidiate ? 0 : task->delay);
	task->active = active;
}

//gibt den nächsten freien Handle zurück und richted diesen ein, ist keiner frei wird 0 zurückgegeben
Scheduler::taskHandle* Scheduler::getTaskHandle(CallbackFunc functionToCall, uint32_t delay, bool active, bool imidiate){
	for(uint16_t i = 0; i < maxTaskCount; i++)
		if(handles[i].isFree){
			handles[i].isFree = false;
			handles[i].active = active;
			handles[i].delay = delay * countsProMs;
			handles[i].functionToCall = functionToCall;
			handles[i].nextActivationAt = counter + (imidiate ? 0 : handles[i].delay);
			return &handles[i];
		}
	return 0;
}

extern "C" {
	//------------------------------------------------------------
	// PIT 3 interrupt routine
	//------------------------------------------------------------
	void PIT3_IRQHandler(void)
	{
		// PIT TIF flag clear
		PIT->CHANNEL[3].TFLG |= PIT_TFLG_TIF_MASK;							/**< Timer Flag Register, array offset: 0x10C, array step: 0x10 */
		uint32_t aTmp = PIT->CHANNEL[3].CVAL;									/**< Current Timer Value Register, array offset: 0x104, array step: 0x10 */

		counter++;
	}
}
