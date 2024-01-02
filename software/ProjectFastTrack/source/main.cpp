#define CAR_CENTER_IN_PIXEL 165 //ToDo: 7 Abziehen SOll Wert 158 bei zentraler Kamera

void pixyUsleep(int useconds) {
	int i = 0;
	int j = 0;
	for (i = 0; i < useconds; i++)
	{
		j++;
	}
}


//muss sein weil nur c fähig
extern "C"
{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "MK64F12.h"
//#include "fsl_debug_console.h"

//hier die SD-card
//BuildError: #include "sdcard/sdmmc_config.h"

#include "Modules/mSpi.h"
//#include "Modules/mDac.h"
//#include "Modules/mAccelMagneto.h"
//#include "Modules/mGyro.h"
#include "Modules/mTimer.h"
#include "Modules/mCpu.h"
//#include "Modules/mSwitch.h"
#include "Modules/mLeds.h"
//#include "Modules/mAd.h"
//#include "Modules/mDelay.h"
//#include "Modules/mRS232.h"
//#include "Modules/mVL6180x.h"

//#include "Applications/gInput.h"
//#include "Applications/gCompute.h"
//#include "Applications/gOutput.h"
}
#include <TFT_Modules/Scheduler.h>
#include <TFT_Modules/CameraAnalysis.h>

#include "Pixy/Pixy2SPI_SS.h"

//Lokale Definitionen
Pixy2SPI_SS pixy;
CameraAnalysis::SingleRowAnalysis singleRowAnalysis_160;

//Task Definitionen
Scheduler::taskHandle* t_blinkLED;
Scheduler::taskHandle* t_motorStop;
Scheduler::taskHandle* t_generalCamera;

Scheduler::taskHandle* t_cameraAlgorithm;



//Bennenungen für Programmstruktur
void pixySetup();
void cameraRowsSetup();
float getSteeringAngleOneLine(CameraAnalysis::SingleRowAnalysis::TrackLine* trackLine);
int16_t getBestTrackIndexFromMultipleTracks(CameraAnalysis::SingleRowAnalysis* singleRow);
bool currentRowAnalysis_160(float* steeringAngle);
float getSteeringAngleSingleTrack(CameraAnalysis::SingleRowAnalysis::Track* singleTrack);


void Setup() {
	mCpu_Setup();

	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	mSpi_Setup();
	mSpi_Open();

	pixySetup();

	Scheduler::Setup();

	cameraRowsSetup();

	//Motor Setup (Motor Enable)
	//mTimer_EnableHBridge();
}

void pixySetup(){
	pixy.init();
	pixy.getVersion();
	pixy.version->print();
	printf("HellO World: %ld\n",clock());
	pixy.setLED(0, 255, 0);
	//pixy.setLamp(1, 1);
	pixy.changeProg("video");
}

//Eine / Mehrere Zeilen können definiert + gewählt werden
void cameraRowsSetup() {
	singleRowAnalysis_160.Setup(&pixy, 160, 5, 15, 0, 6, 130, 240);
	singleRowAnalysis_160.edgeInputs[0].edgeThreshold = 20;
	//ToDo: Hier können weitere Reihen analysiert werden
}


//Allgemeine Methode des Fahren
void fahren() {

	//Kamera Daten abfragen


	//Lenkwinkel Berechnen und einstellen


	//Geschwindigkeit Berechnen und einstellen

}


//Auslesen der Kamera, Sobel und Kanten der übergebenen Reihen!
void generalCameraTask(CameraAnalysis::SingleRowAnalysis** rowsToDo, uint8_t length) {
	for(uint8_t i = 0; i<length; i++) {
		rowsToDo[i]->getImageRow();
		rowsToDo[i]->calculateSobelRow();
		rowsToDo[i]->calculateEdges();
	}
}


void lenkung() {

	mLeds_Write(kMaskLed2,kLedOff);

	//Kameradaten die fehlen!
	singleRowAnalysis_160.calculateTrackLines();
	singleRowAnalysis_160.calculateValidTracks();

	float steeringAngle = 0;
	if (currentRowAnalysis_160(&steeringAngle))
		mTimer_SetServoDuty(0, steeringAngle);
}


bool currentRowAnalysis_160(float* steeringAngle) {

	if(singleRowAnalysis_160.lines[0].isEmpty()) {
		//ToDo: getSteeringAngelZeroLines()
		return false; //kein Wert
	}
	else if (singleRowAnalysis_160.lines[1].isEmpty()) {
		*steeringAngle = getSteeringAngleOneLine(singleRowAnalysis_160.lines + 0);
	}
	else {
		int16_t bestTrackIndex = getBestTrackIndexFromMultipleTracks(&singleRowAnalysis_160);
		if (bestTrackIndex == -1) {
			return false;
		}
		*steeringAngle = getSteeringAngleSingleTrack(singleRowAnalysis_160.tracks + bestTrackIndex);
		mLeds_Write(kMaskLed2,kLedOn);
	}
	return true;
}


float getSteeringAngleZeroLines() {
	//ToDo: do Something
	return 0;
}

float getSteeringAngleOneLine(CameraAnalysis::SingleRowAnalysis::TrackLine* trackLine) {

	static const uint16_t expected_track_width_160 = 250;

	uint16_t destinationCenter = 0;
	float steeringAngleOneLine = destinationCenter;

	if (trackLine->centerIndex < CAR_CENTER_IN_PIXEL) {
		// TODO
		destinationCenter = trackLine->centerIndex + (expected_track_width_160 / 2);
	} else {
		// TODO
		destinationCenter = trackLine->centerIndex - (expected_track_width_160 / 2);
	}
	if(destinationCenter != 0){
		steeringAngleOneLine = destinationCenter;
		steeringAngleOneLine /= 158.0f;
		steeringAngleOneLine -= 1.0f;
		steeringAngleOneLine *= 8.0f;
	}
	return steeringAngleOneLine;
}


//ToDo: es gibt mehrere Lininen (mehr als 1) aber kein valider Track vorhanden muss angeguckt werden
float getSteeringAngleMultipleLinesWithoutTracks() {
	return 0; //ToDo
}


//Für den Fall das genau ein Track gefunden wurde
float getSteeringAngleSingleTrack(CameraAnalysis::SingleRowAnalysis::Track* singleTrack) {
	float steeringAngleSingleTrack = singleTrack->getCenter();
	steeringAngleSingleTrack /= 158.0f;
	steeringAngleSingleTrack -= 1.0f;
	steeringAngleSingleTrack *= 2.0f;
	return steeringAngleSingleTrack;
}

int16_t getBestTrackIndexFromMultipleTracks(CameraAnalysis::SingleRowAnalysis* singleRow) {
	for(int i = 1; i < 316; i++){
		uint16_t index = 158 + (i/2) * (i%2==0?-1:1);
		if(!singleRow->tracks[index].isEmpty()){
			return index;
		}
	}
	return -1;
}



void defineTasks() {

	t_blinkLED = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		static bool state = false;
		mLeds_Write(kMaskLed1,state ? kLedOff : kLedOn);
		state = !state;
	}, 500, false, false);

	t_motorStop = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		mTimer_SetMotorDuty(0, 0);
		self->active = false;
	}, 30000, true, false);


	t_generalCamera = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		static CameraAnalysis::SingleRowAnalysis* usedCameraRows[] =  {
			&singleRowAnalysis_160
		};
		generalCameraTask(usedCameraRows, 1);
	}, 17);


	t_cameraAlgorithm = Scheduler::getTaskHandle([](Scheduler::taskHandle* self){
		lenkung();
	}, 100);

}

int main(){
	printf("Hello Car\n");

	Setup();
	defineTasks();

	//ToDo: Geschwindigkeitssteuerung muss noch richtig gesteuert werden!
	//mTimer_SetMotorDuty(0.4f, 0.4f);


	for(UInt32 i = 0; true; i++){
		Scheduler::Update();
	}

	return 0;
}
