/*
 * TaskManager.h
 *
 *  Created on: 13.11.2023
 *      Author: TFR
 */

//Requires Pit3 !!!

#ifndef SCEDULER_H_
#define SCEDULER_H_

#include <stdint.h>

namespace Sceduler{
	//pre def
	struct taskHandle;

	//typedefine für den Callback
	typedef void (*CallbackFunc)(taskHandle* self);

	//handle struktur
	struct taskHandle{
		//findet aufruf statt
		bool active;

		//delay zwischen den aufrufen
		uint32_t delay;

		//funktion die perriodisch aufgerufen werden soll
		CallbackFunc functionToCall;

		//wenn true wird die struktur freigegeben, der handle ist ungöltig
		bool isFree;

		//wenn du hier rein greifst solltest du wissen was du tust
		uint32_t nextActivationAt;
	};

	//macht den setup und started den Timer 3(PIT3)
	void Setup();

	//aktueller clockzyklus
	uint32_t getClock();

	//aktueller clockzyklus in millisekunden
	float getMillis();

	//periodischer aufruf zum ausführen der tasks
	void Update();

	//erstelle einen task
	taskHandle* getTaskHandle(CallbackFunc functionToCall, uint32_t delay, bool active = true, bool imidiate = true);

	void setTimerActive(taskHandle* handel);
}

#endif /* SCEDULER_H_ */
