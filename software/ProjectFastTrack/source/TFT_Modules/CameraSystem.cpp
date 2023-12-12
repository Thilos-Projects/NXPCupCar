/*
 * CameraSystem.cpp
 *
 *  Created on: 12.12.2023
 *      Author: TFR
 */

void pixyUsleep(int useconds) {
	int i = 0;
	int j = 0;
	for (i = 0; i < useconds; i++)
	{
		j++;
	}
}

extern "C" {
	#include <stdlib.h>
}

#include "Pixy/Pixy2SPI_SS.h"
#include <TFT_Modules/CameraSystem.h>

Pixy2SPI_SS pixy;

void CameraSystem::Setup(){
	pixy.init();
	pixy.changeProg("video");
}

void CameraSystem::Line::setupLine(uint16_t rowInd, uint16_t threshold, uint16_t minEdgeWidth, uint16_t maxEdgeWidth, uint16_t minTrackLineWidth, uint16_t maxTrackLineWidth, uint16_t minTrackWidth, uint16_t maxTrackWidth){
	this->rowIndex = rowIndex;
	this->threshold = threshold;
	this->minEdgeWidth = minEdgeWidth;
	this->maxEdgeWidth = maxEdgeWidth;
	this->minTrackLineWidth = minTrackLineWidth;
	this->maxTrackLineWidth = maxTrackLineWidth;
	this->minTrackWidth = minTrackWidth;
	this->maxTrackWidth = maxTrackWidth;
}

void CameraSystem::Line::getLineFromPixy(){
	pixy.video.getGrayRect(0, rowIndex, 158, rowIndex + 1, 1, 1, cameraDataBufer + 0, false);
	pixy.video.getGrayRect(158, rowIndex, 316, rowIndex + 1, 1, 1, cameraDataBufer + 158, false);
}

void CameraSystem::Line::do1DSoble(){
	for(int i = 0; i < 314; i++)
		sobleOutputBuffer[i] = (((int16_t)(*(cameraDataBufer+i))) * 2 + ((int16_t)(*(cameraDataBufer+i+2))) * -2);
}

void CameraSystem::Line::doEdgeDetection(){
	uint8_t currentEdge = 0;
	edgesOutputBuffer[currentEdge].clear();
	for(int i = 0; i < 314; i++){
		if(sobleOutputBuffer[i] < -threshold || threshold < sobleOutputBuffer[i]){
			if(edgesOutputBuffer[currentEdge].isEmpty()){
				edgesOutputBuffer[currentEdge].startIndex = i;
				edgesOutputBuffer[currentEdge].width = 1;
				edgesOutputBuffer[currentEdge].whiteToBlack = sobleOutputBuffer[i] > 0;
			}
			else{
				if(edgesOutputBuffer[currentEdge].whiteToBlack != (sobleOutputBuffer[i] > 0)){
					currentEdge++;		//neue edge
					edgesOutputBuffer[currentEdge].startIndex = i;
					edgesOutputBuffer[currentEdge].width = 1;
					edgesOutputBuffer[currentEdge].whiteToBlack = sobleOutputBuffer[i] > 0;
				}
				else{
					edgesOutputBuffer[currentEdge].width++;
				}
			}
		}
		else{
			if(!edgesOutputBuffer[currentEdge].isEmpty()){
				currentEdge++;		//neue edge
				edgesOutputBuffer[currentEdge].clear();
			}
		}
	}

	//fülle das array mit einem leeren wert, wen der derzeitig letzte nicht leer ist
	if(!edgesOutputBuffer[currentEdge].isEmpty()){
		currentEdge++;		//neue edge
		edgesOutputBuffer[currentEdge].clear();
	}
}

void CameraSystem::Line::doTrackLineDetection(){
	uint8_t currendLinePointer = 0;
	uint8_t currendEdgePointer = 0;
	bool lastEdegeVallid = false;

	trackLinesOutputBuffer[currendLinePointer].clear();

	if(edgesOutputBuffer[currendEdgePointer].isEmpty())
		return;

	if(minEdgeWidth < edgesOutputBuffer[currendEdgePointer].width && edgesOutputBuffer[currendEdgePointer].width < maxEdgeWidth)
		lastEdegeVallid = true;

	currendEdgePointer++;

	while(!edgesOutputBuffer[currendEdgePointer].isEmpty()){
		if(edgesOutputBuffer[currendEdgePointer].width < minEdgeWidth || maxEdgeWidth < edgesOutputBuffer[currendEdgePointer].width){
			lastEdegeVallid = false;
			currendEdgePointer++;
			continue;
		}

		if(!lastEdegeVallid){
			lastEdegeVallid = true;
			currendEdgePointer++;
			continue;
		}

		uint16_t lineWidth = edgesOutputBuffer[currendEdgePointer].startIndex + edgesOutputBuffer[currendEdgePointer].width - edgesOutputBuffer[currendEdgePointer-1].startIndex;
		if(lineWidth < minTrackLineWidth || maxTrackLineWidth < lineWidth){
			lastEdegeVallid = true;
			currendEdgePointer++;
			continue;
		}

		trackLinesOutputBuffer[currendLinePointer].centerIndex = edgesOutputBuffer[currendEdgePointer-1].startIndex + lineWidth/2;
		trackLinesOutputBuffer[currendLinePointer].width = lineWidth;
		currendLinePointer++;
		trackLinesOutputBuffer[currendLinePointer].clear();

		currendEdgePointer++;
	}
}

void CameraSystem::Line::doTrackDetection(){
	for(int i = 0; i< 316; i++)
		tracks[i].clear();

	for(int i = 0; !trackLinesOutputBuffer[i].isEmpty(); i++){
		for(int j = i+1; !trackLinesOutputBuffer[j].isEmpty(); j++){
			uint16_t width = trackLinesOutputBuffer[j].centerIndex - trackLinesOutputBuffer[i].centerIndex;
			if(width < minTrackWidth || maxTrackWidth < width)
				continue;

			uint16_t center = trackLinesOutputBuffer[i].centerIndex + width/2;
			tracks[center].left = trackLinesOutputBuffer[i].centerIndex;
			tracks[center].right = trackLinesOutputBuffer[j].centerIndex;
			tracks[center].width = width;
		}
	}
}

bool CameraSystem::analyzeLine(Line* line, float* output){
	line->getLineFromPixy();
	line->do1DSoble();
	line->doEdgeDetection();
	line->doTrackLineDetection();
	line->doTrackDetection();

	uint16_t center = 0;

	for(int i = 1; i < 317; i++){
		uint16_t index = 158 + (i/2) * (i%2==0?-1:1);
		if(!line->tracks[index].isEmpty()){
			center = index;
			break;
		}
	}

	if(center == 0)
		return false;

	*output = center/158.0f - 1.0f;
	return true;
}
