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
#include "TFT_Modules/Sceduler.h"

void pixyUsleep(int useconds) {
	int i = 0;
	int j = 0;
	for (i = 0; i < useconds; i++)
	{
		j++;
	}
}

#include "Pixy/Pixy2SPI_SS.h"

Pixy2SPI_SS pixy;

void Setup(){
	pixy.init();
	pixy.getVersion();
	pixy.version->print();
	printf("HellO World: %ld\n",clock());
	pixy.setLED(0, 255, 0);
	//pixy.setLamp(1, 1);
	pixy.changeProg("video");
}

template<typename T>
void printArray(T* line, uint16_t length){
	printf("%d", *(line + 0));
	for(int i = 1; i < length; i++)
		printf(",\t%d", *(line + i));
	printf("\n");
}

uint8_t* getImageRow(){
	static uint8_t rowDataBuffer[316];
	pixy.video.getGrayRect(0, 160, 158, 161, 1, 1, rowDataBuffer + 0, false);
	pixy.video.getGrayRect(158, 160, 316, 161, 1, 1, rowDataBuffer + 158, false);
	return (uint8_t*)&rowDataBuffer;
}

int16_t* getSobelRow(uint8_t* rowDataBuffer){
	static int16_t rowSobleBuffer[314];

	for(int i = 0; i < 314; i++)
		rowSobleBuffer[i] = (((int16_t)(*(rowDataBuffer+i))) * 2 + ((int16_t)(*(rowDataBuffer+i+2))) * -2); //1D sobel ;)

	return (int16_t*)&rowSobleBuffer;
}

struct Edge{
	uint16_t startIndex;
	uint16_t width;
	bool whiteToBlack;

	void clear(){
		whiteToBlack = false;
		width = 0;
		startIndex = 0;
	}
	bool corrupted(){
		return width == 0 && startIndex != 0;
	}
	bool isEmpty(){
		return startIndex == 0;
	}
};
Edge* getEdges(int16_t* rowSobleBuffer, int16_t schwellwert){
	static Edge edgesBuffer[157];
	uint8_t currentEdge = 0;

	edgesBuffer[currentEdge].clear();

	for(int i = 0; i < 314; i++){
		if(rowSobleBuffer[i] < -schwellwert || schwellwert < rowSobleBuffer[i]){
			if(edgesBuffer[currentEdge].isEmpty()){
				edgesBuffer[currentEdge].startIndex = i;
				edgesBuffer[currentEdge].width = 1;
				edgesBuffer[currentEdge].whiteToBlack = rowSobleBuffer[i] > 0;
			}
			else{
				//neue Edge ohne dass eine leer stelle gefunden wurde
				if(edgesBuffer[currentEdge].whiteToBlack != rowSobleBuffer[i] > 0){
					currentEdge++;		//neue edge
					edgesBuffer[currentEdge].startIndex = i;
					edgesBuffer[currentEdge].width = 1;
					edgesBuffer[currentEdge].whiteToBlack = rowSobleBuffer[i] > 0;
				}
				else{
					edgesBuffer[currentEdge].width++;
				}
			}
		}
		else{
			if(!edgesBuffer[currentEdge].isEmpty()){
				currentEdge++;		//neue edge
				edgesBuffer[currentEdge].clear();
			}
		}
	}

	if(!edgesBuffer[currentEdge].isEmpty()){
		currentEdge++;		//neue edge
		edgesBuffer[currentEdge].clear();
	}

	return (Edge*)&edgesBuffer;
}

void printEdges(Edge* edges){
	uint8_t currentEdge = 0;
	while(!edges[currentEdge].isEmpty()){
		printf("%d, %d\t", edges[currentEdge].startIndex, edges[currentEdge].width);
		currentEdge++;
	}
	printf("\n");
}

struct TrackLine{
	uint16_t centerIndex;
	uint16_t width;

	void clear(){
		centerIndex = 0;
	}
	bool isEmpty(){
		return centerIndex == 0;
	}
};
TrackLine* findTrackLines(Edge* edges, uint8_t minLineWidth, uint8_t maxLineWidth, uint8_t minEdgeWidth, uint8_t maxEdgeWidth){
	static TrackLine lines[156];

	uint8_t currendLinePointer = 0;
	uint8_t currendEdgePointer = 0;
	bool lastEdegeVallid = false;

	lines[currendLinePointer].clear();

	//wenn keine Edge vorhanden, dann keine Linie
	if(edges[currendEdgePointer].isEmpty())
		return (TrackLine*)&lines;

	//feststellen ob erste edge eine vallide Edge ist
	if(minEdgeWidth < edges[currendEdgePointer].width && edges[currendEdgePointer].width < maxEdgeWidth)
		lastEdegeVallid = true;

	//nächste Edge auswählen
	currendEdgePointer++;

	//für alle not Empty Edges
	while(!edges[currendEdgePointer].isEmpty()){
		//feststellen ob die derzeitige edge eine vallide Edge ist, wenn nicht das flag für die vorherige Vallide edge resetten und übverspringen
		if(edges[currendEdgePointer].width < minEdgeWidth || maxEdgeWidth < edges[currendEdgePointer].width){
			lastEdegeVallid = false;
			currendEdgePointer++;
			continue;
		}
		//überspringen fals die vorherige Edge nicht vallide ist und das flag für die vorherige Vallide edge setzen
		if(!lastEdegeVallid){
			lastEdegeVallid = true;
			currendEdgePointer++;
			continue;
		}

		//berechnen der Linienbreite
		uint16_t lineWidth = edges[currendEdgePointer].startIndex + edges[currendEdgePointer].width - edges[currendEdgePointer-1].startIndex;
		//prüen ob linie außerhalb erwartungen liegt, wenn ja das flag für die vorherige Vallide edge setzen und überspringen
		if(lineWidth < minLineWidth || maxLineWidth < lineWidth){
			lastEdegeVallid = true;
			currendEdgePointer++;
			continue;
		}

		//Linie setzen
		lines[currendLinePointer].centerIndex = edges[currendEdgePointer-1].startIndex + lineWidth/2;
		lines[currendLinePointer].width = lineWidth;
		//nächste Linie auswählen und clearen
		currendLinePointer++;
		lines[currendLinePointer].clear();

		//nächste edge wählen
		currendEdgePointer++;
	}

	return (TrackLine*)&lines;
}

void printTrackLines(TrackLine* lines){
	uint8_t currentLine = 0;
	while(!lines[currentLine].isEmpty()){
		printf("%d, %d\t", lines[currentLine].centerIndex, lines[currentLine].width);
		currentLine++;
	}
	printf("\n");
}

struct TrackLineCombination{
	uint16_t left;
	uint16_t right;
	uint16_t width;

	void clear(){
		left = 0;
		right = 0;
		width = 0;
	}
	bool isEmpty(){
		return left == 0 && right == 0 && width == 0;
	}
	uint16_t getCenter(){
		return left + width/2;
	}
};
TrackLineCombination* generateVallidTrackeLineCombinations(TrackLine* lines, uint16_t minTrackWidth, uint16_t maxTrackWidth){
	static TrackLineCombination trackeLineCombinations[316];

	for(int i = 0; i< 316; i++)
		trackeLineCombinations[i].clear();

	for(int i = 0; !lines[i].isEmpty(); i++){
		for(int j = i+1; !lines[j].isEmpty(); j++){
			uint16_t width = lines[j].centerIndex - lines[i].centerIndex;
			if(width < minTrackWidth || maxTrackWidth < width)
				continue;

			uint16_t center = lines[i].centerIndex + width/2;
			trackeLineCombinations[center].left = lines[i].centerIndex;
			trackeLineCombinations[center].right = lines[j].centerIndex;
			trackeLineCombinations[center].width = width;
		}
	}

	return (TrackLineCombination*)trackeLineCombinations;
}

void printTrackLineCombinations(TrackLineCombination* tracks){
	for(int i = 0; i < 316; i++)
		printf((tracks + i)->isEmpty() ? "_" : "-");
	printf("\n");
}

void cameraAlgorythmus(Sceduler::taskHandle* self){
	uint8_t* lineDataBuffer;
	int16_t* lineSobleBuffer;
	Edge* edgesBuffer;
	TrackLine* trackLineBuffer;
	TrackLineCombination* trackLineCombinationsBuffer;

	lineDataBuffer = getImageRow();
	//printArray(lineDataBuffer, 316);
	lineSobleBuffer = getSobelRow(lineDataBuffer);
	//printArray(lineSobleBuffer, 314);
	edgesBuffer = getEdges(lineSobleBuffer, 20);
	//printEdges(edgesBuffer);
	trackLineBuffer = findTrackLines(edgesBuffer, 5, 15, 0, 6);
	printTrackLines(trackLineBuffer);

	if(trackLineBuffer[0].isEmpty()) {
		// TODO: NO Track border -> do nothing?
	} else if(trackLineBuffer[1].isEmpty()) {
		// Exactly 1 TrackBorder
		// TODO: Move to config / Constant
		uint16_t car_center = 316 / 2;
		uint16_t destination_center = 0;
		uint16_t expected_track_width = 250;
		if (trackLineBuffer[0].centerIndex < car_center) {
			// TODO
			destination_center = trackLineBuffer[0].centerIndex + (expected_track_width / 2);
		} else {
			// TODO
			destination_center = trackLineBuffer[0].centerIndex - (expected_track_width / 2);
		}
		if(destination_center != 0){
			float stellwinkel = destination_center;
			stellwinkel /= 158.0f;
			stellwinkel -= 1.0f;
			mTimer_SetServoDuty(0, stellwinkel * 6);
		}
	} else {
		// 2+ Borders
		trackLineCombinationsBuffer = generateVallidTrackeLineCombinations(trackLineBuffer, 130, 240);
		//printTrackLineCombinations(trackLineCombinationsBuffer);

		uint16_t center = 0;

		for(int i = 1; i < 317; i++){
			uint16_t index = 158 + (i/2) * (i%2==0?-1:1);
			if(!trackLineCombinationsBuffer[index].isEmpty()){
				center = index;
				break;
			}
		}
		if(center != 0){
			float stellwinkel = center;
			stellwinkel /= 158.0f;
			stellwinkel -= 1.0f;
			mTimer_SetServoDuty(0, stellwinkel * 2);
		}
	}
	
}



int main(){
	printf("Hello Car\n");

	mCpu_Setup();

	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	mSpi_Setup();
	mSpi_Open();

	Setup();


	mLeds_Write(kMaskLed1,kLedOn);

	mTimer_EnableHBridge();

	mTimer_SetMotorDuty(0.4f, 0.4f);

	Sceduler::Setup();

	Sceduler::taskHandle* ledHandle;
	ledHandle = Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		static bool state = false;
		mLeds_Write(kMaskLed1,state ? kLedOff : kLedOn);
		state = !state;
	}, 500);
	ledHandle = Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		mTimer_SetMotorDuty(0, 0);
		self->active = false;
	}, 30000, true, false);
	Sceduler::taskHandle* pixyHandel = Sceduler::getTaskHandle(&cameraAlgorythmus,100);

	for(UInt32 i = 0; true; i++){
		Sceduler::Update();
	}

	return 0;
}
