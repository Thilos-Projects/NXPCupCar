
/*
 * TrackAnalyse.cpp
 *
 *  Created on: 13.12.2023
 *      Author: TFR
 */
void pixyUsleep(int useconds);

#include <TFT_Modules/CameraAnalysis.h>

#include <stdio.h>

extern "C" {
	#include "Modules/mLeds.h"
}

template<typename T>
void printArray(T* line, uint16_t length){
	printf("%d", *(line + 0));
	for(int i = 1; i < length; i++)
		printf(",\t%d", *(line + i));
	printf("\n");
}


void CameraAnalysis::SingleRowAnalysis::Setup(	Pixy2SPI_SS* pixy, uint16_t row, uint8_t minTrackLineWidth,
										uint8_t maxTrackLineWidth, uint8_t minEdgeWidth, uint8_t maxEdgeWidth,
										uint16_t minTrackWidth, uint16_t maxTrackWidth) {
	this->pixy = pixy;
	this->row = row;
	this->minTrackLineWidth = minTrackLineWidth;
	this->maxTrackLineWidth = maxTrackLineWidth;
	this->minEdgeWidth = minEdgeWidth;
	this->maxEdgeWidth = maxEdgeWidth;
	this->minTrackWidth = minTrackWidth;
	this->maxTrackWidth = maxTrackWidth;
}

void CameraAnalysis::SingleRowAnalysis::EdgeOutput::clear(){
	whiteToBlack = false;
	width = 0;
	startIndex = 0;
}
bool CameraAnalysis::SingleRowAnalysis::EdgeOutput::corrupted(){
	return width == 0 && startIndex != 0;
}
bool CameraAnalysis::SingleRowAnalysis::EdgeOutput::isEmpty(){
	return startIndex == 0;
}

void CameraAnalysis::SingleRowAnalysis::TrackLine::clear(){
	centerIndex = 0;
}
bool CameraAnalysis::SingleRowAnalysis::TrackLine::isEmpty(){
	return centerIndex == 0;
}

void CameraAnalysis::SingleRowAnalysis::Track::clear(){
	left = 0;
	right = 0;
	width = 0;
}
bool CameraAnalysis::SingleRowAnalysis::Track::isEmpty(){
	return left == 0 && right == 0 && width == 0;
}
uint16_t CameraAnalysis::SingleRowAnalysis::Track::getCenter(){
	return left + width/2;
}

void CameraAnalysis::SingleRowAnalysis::getImageRow(){
	pixy->video.getGrayRect(0, row, 158, row+1, 1, 1, rowDataBuffer + 0, false);
	pixy->video.getGrayRect(158, row, 316, row+1, 1, 1, rowDataBuffer + 158, false);
}
void CameraAnalysis::SingleRowAnalysis::calculateSobelRow(){
	for(int i = 0; i < 314; i++)
		rowSobel[i] = (((int16_t)(*(rowDataBuffer+i))) * 2 + ((int16_t)(*(rowDataBuffer+i+2))) * -2);
}
void CameraAnalysis::SingleRowAnalysis::calculateEdges(){
	uint8_t currentEdge = 0;

	edges[currentEdge].clear();

	for(int i = 2; i < 312; i++){ //2 - 312 Die Kanten Links und Rechts rausnehmen
		if(rowSobel[i] < -edgeInputs[0].edgeThreshold || edgeInputs[0].edgeThreshold < rowSobel[i]){ //ToDo: ggf. mehrere Thresholds nutzen
			if(edges[currentEdge].isEmpty()){
				edges[currentEdge].startIndex = i;
				edges[currentEdge].width = 1;
				edges[currentEdge].whiteToBlack = rowSobel[i] > 0;
			}
			else{
				//neue Edge ohne dass eine leer stelle gefunden wurde
				if(edges[currentEdge].whiteToBlack != rowSobel[i] > 0){
					currentEdge++;		//neue edge
					edges[currentEdge].startIndex = i;
					edges[currentEdge].width = 1;
					edges[currentEdge].whiteToBlack = rowSobel[i] > 0;
				}
				else{
					edges[currentEdge].width++;
				}
			}
		}
		else{
			if(!edges[currentEdge].isEmpty()){
				currentEdge++;		//neue edge
				edges[currentEdge].clear();
			}
		}
	}

	if(!edges[currentEdge].isEmpty()){
		currentEdge++;		//neue edge
		edges[currentEdge].clear();
	}
}
void CameraAnalysis::SingleRowAnalysis::calculateTrackLines(){
	uint8_t currendLinePointer = 0;
	uint8_t currendEdgePointer = 0;
	bool lastEdegeVallid = false;

	lines[currendLinePointer].clear();

	if(edges[currendEdgePointer].isEmpty())
		return;

	if(minEdgeWidth < edges[currendEdgePointer].width && edges[currendEdgePointer].width < maxEdgeWidth)
		lastEdegeVallid = true;

	currendEdgePointer++;

	while(!edges[currendEdgePointer].isEmpty()){
		if(edges[currendEdgePointer].width < minEdgeWidth || maxEdgeWidth < edges[currendEdgePointer].width){
			lastEdegeVallid = false;
			currendEdgePointer++;
			continue;
		}

		if(!lastEdegeVallid){
			lastEdegeVallid = true;
			currendEdgePointer++;
			continue;
		}

		uint16_t lineWidth = edges[currendEdgePointer].startIndex + edges[currendEdgePointer].width - edges[currendEdgePointer-1].startIndex;
		if(lineWidth < minTrackLineWidth || maxTrackLineWidth < lineWidth){
			lastEdegeVallid = true;
			currendEdgePointer++;
			continue;
		}

		lines[currendLinePointer].centerIndex = edges[currendEdgePointer-1].startIndex + lineWidth/2;
		lines[currendLinePointer].width = lineWidth;

		currendLinePointer++;
		lines[currendLinePointer].clear();

		currendEdgePointer++;
	}
}
void CameraAnalysis::SingleRowAnalysis::calculateValidTracks(){
	for(int i = 0; i< 316; i++)
		tracks[i].clear();

	uint8_t trackCount = 0;
	uint16_t centerFoud = 0;

	for(int i = 0; !lines[i].isEmpty(); i++){
		for(int j = i+1; !lines[j].isEmpty(); j++){
			uint16_t width = lines[j].centerIndex - lines[i].centerIndex;
			if(width < minTrackWidth || maxTrackWidth < width)
				continue;

			trackCount++;
			uint16_t center = lines[i].centerIndex + width/2;
			tracks[center].left = lines[i].centerIndex;
			tracks[center].right = lines[j].centerIndex;
			tracks[center].width = width;
			centerFoud = center;
		}
	}
	//Ausgabe der Anzahl gefundener Tracks
	printf("Track Count: %d Center Found; %d \n", trackCount, centerFoud);
	printTrackLines();
	printEdges();
	mLeds_Write(kMaskLed1,(trackCount/1) % 2 == 1 ? kLedOff : kLedOn);
	mLeds_Write(kMaskLed2,(trackCount/2) % 2 == 1 ? kLedOff : kLedOn);
	mLeds_Write(kMaskLed3,(trackCount/4) % 2 == 1 ? kLedOff : kLedOn);
	mLeds_Write(kMaskLed4,(trackCount/8) % 2 == 1 ? kLedOff : kLedOn);
}

//----------------------Print-------------------
void CameraAnalysis::SingleRowAnalysis::printImageRow(){
	printArray(rowDataBuffer, 316);
}
void CameraAnalysis::SingleRowAnalysis::printSobleRow(){
	printArray(rowSobel, 314);
}
void CameraAnalysis::SingleRowAnalysis::printEdges(){
	uint8_t currentEdge = 0;
	while(!edges[currentEdge].isEmpty()){
		printf("E: %d, %d\t", edges[currentEdge].startIndex, edges[currentEdge].width);
		currentEdge++;
	}
	printf("\n");
}
void CameraAnalysis::SingleRowAnalysis::printTrackLines(){
	uint8_t currentLine = 0;
	while(!lines[currentLine].isEmpty()){
		printf("L: %d, %d\t", lines[currentLine].centerIndex, lines[currentLine].width);
		currentLine++;
	}
	printf("\n");
}
void CameraAnalysis::SingleRowAnalysis::printFoundCenters(){
	for(int i = 0; i < 316; i++)
		printf((tracks + i)->isEmpty() ? "_" : "-");
	printf("\n");
}
