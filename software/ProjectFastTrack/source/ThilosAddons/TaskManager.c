/*
 * TaskManager.cpp
 *
 *  Created on: 13.11.2023
 *      Author: TFR
 */

#include "MK64F12.h"
#include "Utils/def.h"
#include "ThilosAddons/TaskManager.h"

UInt32 counter;
taskHandle handles[maxTaskCount];

void Setup(){
	printf("Im Setup \n");
	counter = 0;

	for(UInt16 i = 0; i < maxTaskCount; i++) {
		handles[i].isFree = true;
	}


	// Enable du clock du p�riph�rique PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

	// PIT Module Control Register (PIT_MCR)
	// --> Timers are stopped in debug mode
	PIT->MCR |= PIT_MCR_FRZ_MASK;				 								/**< PIT Module Control Register, offset: 0x0 */
	// Counting value to do the delay

	// Counting Value=DelayTimeMs/(1/50000)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1017
	// Timer Load Value Register (PIT_LDVALn)
	PIT->CHANNEL[3].LDVAL = (UInt32)(minTaskTimeInMs / (1 / (kClockPeriphkHz)));	/**< Timer Load Value Register, array offset: 0x100, array step: 0x10 */

	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
	// Timer Control Register (PIT_TCTRLn)
	// Enable PIT interrupt
	PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TIE_MASK;								/**< Timer Control Register, array offset: 0x108, array step: 0x10 */

	// Clear prending interrupt
	NVIC_ClearPendingIRQ(PIT3_IRQn);

	// Interrupt Set-enable Registers
	// Interrupt enable
	NVIC_EnableIRQ(PIT3_IRQn);

	// Interrupt Priority Registers
	// Set interrupt priority
	NVIC_SetPriority(PIT3_IRQn, kPIT_IntPriority);

	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1016
	// PIT Module Control Register (PIT_MCR)
	// Clock for PIT Timers is enabled
	PIT->MCR &= (~PIT_MCR_MDIS_MASK);				 							/**< PIT Module Control Register, offset: 0x0 */

	// Timer Control Register (PIT_TCTRLn)
	// Timer is active
	PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;								/**< Timer Control Register, array offset: 0x108, array step: 0x10 */
}

void Interupt() {
	counter = counter + 1;
}

UInt32 getMicros(){
	return counter / minTaskTimeInMs / 1000;
}
UInt32 getMillis(){
	return counter / minTaskTimeInMs;
	//return counter;
}

void Update(){
	//printf("Counter: %d \n",counter);
	for(UInt16 i = 0; i < maxTaskCount; i++) {
		//printf("Next Acitvation: %d, Counter %d\n", handles[i].nextActivationAt, counter);
		if(!handles[i].isFree && handles[i].active && handles[i].nextActivationAt < counter){
			handles[i].functionToCall();
			//handles[i].nextActivationAt = counter + handles[i].delay;
			handles[i].nextActivationAt = handles[i].nextActivationAt + handles[i].delay;
		}
	}
}
taskHandle* getTaskHandle(){
	for(UInt16 i = 0; i < maxTaskCount; i++)
		if(handles[i].isFree){
			handles[i].isFree = false;
			handles[i].nextActivationAt = counter;
			return &handles[i];
		}
	return 0;
}

//------------------------------------------------------------
// PIT 3 interrupt routine
//------------------------------------------------------------
void PIT3_IRQHandler(void)
{
	// PIT TIF flag clear
	PIT->CHANNEL[3].TFLG |= PIT_TFLG_TIF_MASK;							/**< Timer Flag Register, array offset: 0x10C, array step: 0x10 */
	UInt32 aTmp = PIT->CHANNEL[3].CVAL;									/**< Current Timer Value Register, array offset: 0x104, array step: 0x10 */

	Interupt();
}
