/*
 * MotorControll.cpp
 *
 *  Created on: 30.11.2023
 *      Author: TFR
 */

#include "Utils/def.h"
#include "MK64F12.h"
#include <ThilosAddons/MotorControl.h>
#include <TFT_Modules/Scheduler.h>
//#include <ThilosAddons/PI.h>
#include <cstdio>

#define FTM2_CLOCK CORE_CLOCK										  // ClockCount
#define FTM2_CLK_PRESCALE 7											  // Prescaler -> 7 = divide by 128
#define FTM2_OVERFLOW_FREQUENCY 100.0								  // PWM Fequenz
#define SERVO_MIN_DUTY_CYCLE (float)(.0007 * FTM2_OVERFLOW_FREQUENCY) // min = 1ms
#define SERVO_MAX_DUTY_CYCLE (float)(.0023 * FTM2_OVERFLOW_FREQUENCY) // max = 2ms

#define FTM1_CLK_PRESCALE 3 //Prescaller
#define FTM1_OVERFLOW_FREQUENCY (4)
#define kFreqTCNT ((float)6250000.0)

#define TFC_MOTOR_SWITCHING_FREQUENCY ((float)(10000.0))
#define FTM0_MOD_VALUE (int)((float)(PERIPHERAL_BUS_CLOCK) / TFC_MOTOR_SWITCHING_FREQUENCY)
#define FTM0_CLOCK PERIPHERAL_BUS_CLOCK
#define FTM0_CLK_PRESCALE 0
#define FTM0_OVERFLOW_FREQUENCY 5000

float istGeschwindigkeitL, istGeschwindigkeitR;
float sollGeschwindigkeitL, sollGeschwindigkeitR;
int64_t counterL, counterR;
// PI pL, pR;
float motorLPWMSpeed = 0.0f;
float motorRPWMSpeed = 0.0f;

void MotorControl::Setup(){
	Scheduler::getTaskHandle([](Scheduler::taskHandle* self){MotorControl::Update();}, 10);
	Scheduler::getTaskHandle([](Scheduler::taskHandle* self){MotorControl::doSpeedCalc();}, 17);

	istGeschwindigkeitL = 0;
	istGeschwindigkeitR = 0;
	counterL = 0;
	counterR = 0;

	// pL.setup(0.9f, 1.02f, -1.0f, 1.0f);
	// pR.setup(0.9f, 1.02f, -1.0f, 1.0f);

	//PWM Servo
	FTM2->MODE |= FTM_MODE_WPDIS_MASK;
	FTM2->MODE &= (~FTM_MODE_FTMEN_MASK);
	FTM2->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;
	FTM2->CNT = 0x0;
	FTM2->MOD = (PERIPHERAL_BUS_CLOCK / (1 << FTM2_CLK_PRESCALE)) / FTM2_OVERFLOW_FREQUENCY;
	FTM2->CNTIN = 0;
	FTM2->CONTROLS[0].CnSC = 0;
	FTM2->CONTROLS[0].CnSC |= (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK);
	FTM2->CONTROLS[1].CnSC = 0;
	FTM2->CONTROLS[1].CnSC |= (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK);
	FTM2->SC = 0;
	FTM2->CONTROLS[0].CnV = FTM2->MOD / 10;
	FTM2->CONTROLS[1].CnV = FTM2->MOD / 10;
	FTM2->SC |= (FTM_SC_CLKS(1) | FTM_SC_PS(FTM2_CLK_PRESCALE));
	FTM2->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

	//HallInput
	SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
	PORTA->PCR[12] = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
	PORTA->PCR[13] = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
	FTM1->MODE |= FTM_MODE_WPDIS_MASK;
	FTM1->MODE &= (~FTM_MODE_FTMEN_MASK);
	FTM1->CNT = 0x0;
	FTM1->MOD = 65535;
	FTM1->CNTIN = 0;
	FTM1->CONTROLS[0].CnSC = 0;
	FTM1->CONTROLS[0].CnSC |= (FTM_CnSC_ELSA_MASK | FTM_CnSC_CHIE_MASK);
	FTM1->CONTROLS[1].CnSC = 0;
	FTM1->CONTROLS[1].CnSC |= (FTM_CnSC_ELSA_MASK | FTM_CnSC_CHIE_MASK);
	FTM1->SC = 0 | FTM_SC_TOIE_MASK | (FTM_SC_CLKS(1) | FTM_SC_PS(FTM1_CLK_PRESCALE));
	NVIC_ClearPendingIRQ(FTM1_IRQn);
	NVIC_EnableIRQ(FTM1_IRQn);
	NVIC_SetPriority(FTM1_IRQn, kFTM1_IntPriority);

	//PWM Motor
	SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
	PORTC->PCR[1] = PORT_PCR_MUX(4);
	PORTC->PCR[2] = PORT_PCR_MUX(4);
	PORTC->PCR[3] = PORT_PCR_MUX(4);
	PORTC->PCR[4] = PORT_PCR_MUX(4);
	FTM0->MODE |= FTM_MODE_WPDIS_MASK;
	FTM0->MODE &= (~FTM_MODE_FTMEN_MASK);
	FTM0->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;
	FTM0->CNT = 0x0;
	FTM0->MOD = (FTM0_CLOCK / (1 << FTM0_CLK_PRESCALE)) / TFC_MOTOR_SWITCHING_FREQUENCY;
	FTM0->CNTIN = 0;
	FTM0->CONTROLS[0].CnSC = 0;
	FTM0->CONTROLS[0].CnSC |= (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK);
	FTM0->CONTROLS[1].CnSC = 0;
	FTM0->CONTROLS[1].CnSC |= (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK);
	FTM0->CONTROLS[2].CnSC = 0;
	FTM0->CONTROLS[2].CnSC |= (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK);
	FTM0->CONTROLS[3].CnSC = 0;
	FTM0->CONTROLS[3].CnSC |= (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK);
	FTM0->SC = 0;
	FTM0->CONTROLS[0].CnV = 0;
	FTM0->CONTROLS[1].CnV = 0;
	FTM0->CONTROLS[2].CnV = 0;
	FTM0->CONTROLS[3].CnV = 0;
	FTM0->SC |= (FTM_SC_CLKS(1) | FTM_SC_PS(FTM0_CLK_PRESCALE));
	FTM0->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;
}
void MotorControl::setServo(float angle, float offset){
	// TFT: 0.8 instead of 1

	// Saturation
	if (angle > 0.82)
	{
		angle = 0.852;
	}
	else if (angle < -0.82)
	{
		angle = -0.852;
	}

	angle += offset;

	float aDutyCycle = (((angle + 1.0) / 2) * (SERVO_MAX_DUTY_CYCLE - SERVO_MIN_DUTY_CYCLE)) + SERVO_MIN_DUTY_CYCLE;

	FTM2->CONTROLS[0].CnV = FTM2->MOD * aDutyCycle;
}
void MotorControl::getSpeed(float *left, float *right){
	*left = istGeschwindigkeitL;
	*right = istGeschwindigkeitR;
}
void MotorControl::setSpeed(float left, float right){
	sollGeschwindigkeitL = left;
	sollGeschwindigkeitR = right;
}

void MotorControl::doSpeedCalc(){
	static float lastTime;
	
	float time = Scheduler::getMillis();
	float deltaTime = time - lastTime;
	lastTime = time;

	static uint32_t frameCountersL[3] = {0};
	static uint32_t frameCountersR[3] = {0};
	static uint32_t framePointer = 0;

	static uint32_t lastCounterL, lastCounterR;

	// Write current values
	frameCountersL[framePointer % 3] = ((float)(counterL-lastCounterL));
	frameCountersR[framePointer % 3] = ((float)(counterR-lastCounterR));

	// Calculate
	istGeschwindigkeitL = 0.0f;
	istGeschwindigkeitR = 0.0f;
	for (uint8_t i = 0; i < 3; i++) {
		istGeschwindigkeitL += frameCountersL[i];
		istGeschwindigkeitR += frameCountersR[i];
	}
	istGeschwindigkeitL /= (deltaTime * 3.0f) * 6.0f; // 3 Frames über die wir rechnen * 6 Magneten
	istGeschwindigkeitL *= 1000.0f; // 1000ms/s
	istGeschwindigkeitR /= (deltaTime * 3.0f) * 6.0f; // 3 Frames über die wir rechnen * 6 Magneten
	istGeschwindigkeitR *= 1000.0f; // 1000ms/s

	lastCounterL = counterL;
	lastCounterR = counterR;
	framePointer++;
}
//alle 10 ms oder so
void MotorControl::Update(){
	static float lastTime;
	float time = Scheduler::getMillis();
	float deltaTime = time - lastTime;
	lastTime = time;

	//PID test
	motorLPWMSpeed = sollGeschwindigkeitL; // pL.doTick(istGeschwindigkeitL, sollGeschwindigkeitL, deltaTime / 1000.0f);
	motorRPWMSpeed = sollGeschwindigkeitR; // pR.doTick(istGeschwindigkeitR, sollGeschwindigkeitR, deltaTime / 1000.0f);

	float cMax = 0.6f;

	//MotorSpeed
	if (motorLPWMSpeed > cMax) motorLPWMSpeed = cMax;
	else if (motorLPWMSpeed < -cMax) motorLPWMSpeed = -cMax;

	if (motorRPWMSpeed > cMax) motorRPWMSpeed = cMax;
	else if (motorRPWMSpeed < -cMax) motorRPWMSpeed = -cMax;

	if (motorLPWMSpeed > 0)
	{
		FTM0->CONTROLS[0].CnV = (uint16_t)((float)FTM0->MOD * motorLPWMSpeed);
		FTM0->CONTROLS[3].CnV = 0;
	}
	else
	{
		FTM0->CONTROLS[3].CnV = (uint16_t)((float)FTM0->MOD * -motorLPWMSpeed);
		FTM0->CONTROLS[0].CnV = 0;
	}

	if (motorRPWMSpeed > 0)
	{
		FTM0->CONTROLS[2].CnV = (uint16_t)((float)FTM0->MOD * motorRPWMSpeed);
		FTM0->CONTROLS[1].CnV = 0;
	}
	else
	{
		FTM0->CONTROLS[1].CnV = (uint16_t)((float)FTM0->MOD * -motorRPWMSpeed);
		FTM0->CONTROLS[2].CnV = 0;
	}
}

extern "C"{
	void FTM1_IRQHandler(void)
	{
		static uint32_t sOverFlowCounterL = 0;
		static uint32_t sOverFlowCounterR = 0;

		if ((FTM1->SC & FTM_SC_TOF_MASK) == FTM_SC_TOF_MASK) {
			FTM1->SC &= (~FTM_SC_TOF_MASK);

			sOverFlowCounterL++;
			sOverFlowCounterR++;
			//KP was hier ab geht
		}

		if ((FTM1->CONTROLS[0].CnSC & FTM_CnSC_CHF_MASK) == FTM_CnSC_CHF_MASK) {
			FTM1->CONTROLS[0].CnSC &= (~FTM_CnSC_CHF_MASK);
			//aVal = FTM1->CONTROLS[0].CnV;						//time ore somthing like that
			//Right Motor Stuff
			static bool first = true;
			if(!first) counterR++;
			else first = false;
		}

		if ((FTM1->CONTROLS[1].CnSC & FTM_CnSC_CHF_MASK) == FTM_CnSC_CHF_MASK) {
			FTM1->CONTROLS[1].CnSC &= (~FTM_CnSC_CHF_MASK);
			//aVal = FTM1->CONTROLS[1].CnV;						//time ore somthing like that
			//Left Motor Stuff
			static bool first = true;
			if(!first) counterL++;
			else first = false;

		}
	}
}