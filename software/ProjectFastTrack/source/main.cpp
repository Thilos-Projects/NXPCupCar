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

struct TrackAnalyse{
	uint16_t row;				//current 160
	uint16_t schwellwert;		//current 20
	uint8_t minLineWidth;		//current 5
	uint8_t maxLineWidth;		//current 15
	uint8_t minEdgeWidth;		//current 0
	uint8_t maxEdgeWidth;		//current 6
	uint16_t minTrackWidth;		//current 130
	uint16_t maxTrackWidth;		//current 240

	uint8_t rowDataBuffer[316];
	void getImageRow(){
		pixy.video.getGrayRect(0, row, 158, row+1, 1, 1, rowDataBuffer + 0, false);
		pixy.video.getGrayRect(158, row, 316, row+1, 1, 1, rowDataBuffer + 158, false);
	}

	int16_t rowSobleBuffer[314];
	void getSobelRow(){
		for(int i = 0; i < 314; i++)
			rowSobleBuffer[i] = (((int16_t)(*(rowDataBuffer+i))) * 2 + ((int16_t)(*(rowDataBuffer+i+2))) * -2);
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
	Edge edgesBuffer[157];
	void getEdges(){
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
	TrackLine lines[156];
	void findTrackLines(){
		uint8_t currendLinePointer = 0;
		uint8_t currendEdgePointer = 0;
		bool lastEdegeVallid = false;

		lines[currendLinePointer].clear();

		if(edgesBuffer[currendEdgePointer].isEmpty())
			return;

		if(minEdgeWidth < edgesBuffer[currendEdgePointer].width && edgesBuffer[currendEdgePointer].width < maxEdgeWidth)
			lastEdegeVallid = true;

		currendEdgePointer++;

		while(!edgesBuffer[currendEdgePointer].isEmpty()){
			if(edgesBuffer[currendEdgePointer].width < minEdgeWidth || maxEdgeWidth < edgesBuffer[currendEdgePointer].width){
				lastEdegeVallid = false;
				currendEdgePointer++;
				continue;
			}

			if(!lastEdegeVallid){
				lastEdegeVallid = true;
				currendEdgePointer++;
				continue;
			}

			uint16_t lineWidth = edgesBuffer[currendEdgePointer].startIndex + edgesBuffer[currendEdgePointer].width - edgesBuffer[currendEdgePointer-1].startIndex;
			if(lineWidth < minLineWidth || maxLineWidth < lineWidth){
				lastEdegeVallid = true;
				currendEdgePointer++;
				continue;
			}

			lines[currendLinePointer].centerIndex = edgesBuffer[currendEdgePointer-1].startIndex + lineWidth/2;
			lines[currendLinePointer].width = lineWidth;

			currendLinePointer++;
			lines[currendLinePointer].clear();

			currendEdgePointer++;
		}
	}

	struct Track{
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
	Track tracks[316];
	void generateVallidTrackes(){
		for(int i = 0; i< 316; i++)
			tracks[i].clear();

		for(int i = 0; !lines[i].isEmpty(); i++){
			for(int j = i+1; !lines[j].isEmpty(); j++){
				uint16_t width = lines[j].centerIndex - lines[i].centerIndex;
				if(width < minTrackWidth || maxTrackWidth < width)
					continue;

				uint16_t center = lines[i].centerIndex + width/2;
				tracks[center].left = lines[i].centerIndex;
				tracks[center].right = lines[j].centerIndex;
				tracks[center].width = width;
			}
		}
	}

	void callAll(){
		getImageRow();
		getSobelRow();
		getEdges();
		findTrackLines();
		generateVallidTrackes();
	}

	//----------------------Print-------------------
	template<typename T>
	void printArray(T* line, uint16_t length){
		printf("%d", *(line + 0));
		for(int i = 1; i < length; i++)
			printf(",\t%d", *(line + i));
		printf("\n");
	}
	void printImageRow(){
		printArray(rowDataBuffer, 316);
	}
	void printSobleRow(){
		printArray(rowSobleBuffer, 314);
	}
	void printEdges(){
		uint8_t currentEdge = 0;
		while(!edgesBuffer[currentEdge].isEmpty()){
			printf("%d, %d\t", edgesBuffer[currentEdge].startIndex, edgesBuffer[currentEdge].width);
			currentEdge++;
		}
		printf("\n");
	}
	void printTrackLines(){
		uint8_t currentLine = 0;
		while(!lines[currentLine].isEmpty()){
			printf("%d, %d\t", lines[currentLine].centerIndex, lines[currentLine].width);
			currentLine++;
		}
		printf("\n");
	}
	void printTrackLineCombinations(){
		for(int i = 0; i < 316; i++)
			printf((tracks + i)->isEmpty() ? "_" : "-");
		printf("\n");
	}
};

void cameraAlgorythmus_2(TrackAnalyse* analysMethod){

	analysMethod->callAll();

	if(analysMethod->lines[0].isEmpty()) {
		// TODO: NO Track border -> do nothing?
	} else if(analysMethod->lines[1].isEmpty()) {
		// Exactly 1 TrackBorder
		// TODO: Move to config / Constant
		uint16_t car_center = 316 / 2;
		uint16_t destination_center = 0;
		uint16_t expected_track_width = 250;
		if (analysMethod->lines[0].centerIndex < car_center) {
			// TODO
			destination_center = analysMethod->lines[0].centerIndex + (expected_track_width / 2);
		} else {
			// TODO
			destination_center = analysMethod->lines[0].centerIndex - (expected_track_width / 2);
		}
		if(destination_center != 0){
			float stellwinkel = destination_center;
			stellwinkel /= 158.0f;
			stellwinkel -= 1.0f;
			printf("Neuer Stellwinkel: %f", stellwinkel * 6);
		}
	} else {
		uint16_t center = 0;

		for(int i = 1; i < 317; i++){
			uint16_t index = 158 + (i/2) * (i%2==0?-1:1);
			if(!analysMethod->tracks[index].isEmpty()){
				center = index;
				break;
			}
		}
		if(center != 0){
			float stellwinkel = center;
			stellwinkel /= 158.0f;
			stellwinkel -= 1.0f;
			printf("Neuer Stellwinkel: %f", stellwinkel * 2);
		}
	}
}


TrackAnalyse trackAnalys_0;

int main(){
	printf("Hello Car\n");

	mCpu_Setup();

	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	mSpi_Setup();
	mSpi_Open();

	Setup();

	trackAnalys_0.row = 160;
	trackAnalys_0.schwellwert = 20;
	trackAnalys_0.minLineWidth = 5;
	trackAnalys_0.maxLineWidth = 15;
	trackAnalys_0.minEdgeWidth = 0;
	trackAnalys_0.maxEdgeWidth = 6;
	trackAnalys_0.minTrackWidth = 130;
	trackAnalys_0.maxTrackWidth = 240;

	mLeds_Write(kMaskLed1,kLedOn);

	mTimer_EnableHBridge();

	mTimer_SetMotorDuty(0.4f, 0.4f);

	Sceduler::Setup();

	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		static bool state = false;
		mLeds_Write(kMaskLed1,state ? kLedOff : kLedOn);
		state = !state;
	}, 500);
	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){
		mTimer_SetMotorDuty(0, 0);
		self->active = false;
	}, 30000, true, false);
	Sceduler::getTaskHandle([](Sceduler::taskHandle* self){cameraAlgorythmus_2(&trackAnalys_0); }, 100);

	for(UInt32 i = 0; true; i++){
		Sceduler::Update();
	}

	return 0;
}
