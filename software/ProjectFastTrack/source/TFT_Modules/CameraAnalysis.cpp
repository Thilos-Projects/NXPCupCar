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

void CameraAnalysis::SingleRowAnalysis::Setup(Pixy2SPI_SS* pixy, uint16_t row, uint16_t edgeThreshold,
		uint8_t minEdgeWidth, uint8_t maxEdgeWidth, uint16_t centerPixel, uint16_t minEdgeThicness) {
	this->pixy = pixy;
	this->row = row;
	this->edgeThreshold = edgeThreshold;
	this->minEdgeWidth = minEdgeWidth;
	this->maxEdgeWidth = maxEdgeWidth;
	this->centerPixel = centerPixel;
	this->minEdgeThicness = minEdgeThicness;
}

void CameraAnalysis::SingleRowAnalysis::getImageRow(){
	pixy->video.getGrayRect(0, row, 158, row+1, 1, 1, rowDataBuffer + 0, false);
	pixy->video.getGrayRect(158, row, 316, row+1, 1, 1, rowDataBuffer + 158, false);
}

void CameraAnalysis::SingleRowAnalysis::calculateSobelRow(){
	for(int i = 0; i < 314; i++)
		rowSobel[i] = (((int16_t)(*(rowDataBuffer+i))) * 2 + ((int16_t)(*(rowDataBuffer+i+2))) * -2);
}

uint16_t getEdge(bool isLeft, uint16_t centerPixel, int16_t* rowSobel, uint16_t edgeThreshold, uint8_t minEdgeWidth, uint8_t maxEdgeWidth) {
	uint16_t edge = isLeft ? 2 : 310;
	uint16_t width = 0;
	bool isEmpty = true;

	for(uint16_t i = centerPixel; isLeft? (i > 2) : (i < 310); isLeft ? (i--) : (i++)) {
		if(rowSobel[i] < -edgeThreshold || edgeThreshold < rowSobel[i]){
			if(isEmpty){
				if(isLeft ? (rowSobel[i] > 0) : (rowSobel[i] < 0)) //Übergänge von Schwarz zu Weiß ignorieren
					continue;
				isEmpty = false;
				edge = i;
				width = 1;
			}
			else {
				//neue Edge ohne dass eine leer stelle gefunden wurde
				if(isLeft ? (rowSobel[i] > 0) : (rowSobel[i] < 0)){
					//kanten ende
					if(width < minEdgeWidth || maxEdgeWidth < width) {
						isEmpty = true;
						width = 0;
						continue;
					}
					else {
						break;
					}
				}
				else {
					width++;
				}
			}
		}
		else {
			if(!isEmpty){
				if(width < minEdgeWidth || maxEdgeWidth < width) {
					isEmpty = true;
					width = 0;
					continue;
				}
				else {
					break;
				}
			}
		}
	}

	return edge;
}

uint16_t getEdgeWithThikness(bool isLeft, uint16_t centerPixel, int16_t* rowSobel, uint16_t edgeThreshold, uint8_t minEdgeWidth, uint8_t maxEdgeWidth, uint8_t minThicness) {
	uint16_t edgeStartPosition;
	uint16_t edgeWidth;
	bool isAnEdgeFound = false;

	bool isEdgeWithinSobelThreshold;
	bool doesEdgeChangeInRightDirection;

	for(uint16_t i = centerPixel; isLeft? (i > 2) : (i < 310); isLeft ? (i--) : (i++)) {
		isEdgeWithinSobelThreshold = rowSobel[i] < -edgeThreshold || edgeThreshold < rowSobel[i];
		doesEdgeChangeInRightDirection = isLeft ? (rowSobel[i] < 0) : (rowSobel[i] > 0);
		int16_t temp = rowSobel[i];

		if(isEdgeWithinSobelThreshold) {
			if(!isAnEdgeFound){
				if(!doesEdgeChangeInRightDirection)
					//Sobel threshold reached, no edge found, edge in wrong direction
					continue;

				//Sobel threshold reached, no edge found, edge in right direction
				isAnEdgeFound = true;
				edgeStartPosition = i;
				edgeWidth = 1;
				continue;
			}

			if(!doesEdgeChangeInRightDirection){
				if(edgeWidth < minEdgeWidth || maxEdgeWidth < edgeWidth) {
					//Sobel threshold reached, edge found, edge in wrong direction, edge with false
					isAnEdgeFound = false;
					continue;
				}

				if(edgeWidth < minThicness){
					//Sobel threshold reached, edge found, edge in wrong direction, edge with right, edge Thicness was wrong
					isAnEdgeFound = false;
					continue;
				}

				//Sobel threshold reached, edge found, edge in wrong direction, edge with right, edge Thicness was right
				break;
			}

			//Sobel threshold reached, edge found, edge in right direction
			edgeWidth++;
			continue;
		}

		if(isAnEdgeFound){
			if(edgeWidth < minEdgeWidth || maxEdgeWidth < edgeWidth) {
				//Sobel threshold not reached, edge found, edge with false
				isAnEdgeFound = false;
				continue;
			}

			if(edgeWidth > minThicness){
				//Sobel threshold not reached, edge found, edge with right, edge Thicness was right
				break;
			}

			//Sobel threshold not reached, edge found, edge with right, edge Thicness was wrong
			edgeWidth++;
			continue;
		}
	}

	if(!isAnEdgeFound)
		edgeStartPosition = isLeft ? 2 : 310;

	return edgeStartPosition;
}

void CameraAnalysis::SingleRowAnalysis::findBlancArea() {
	uint16_t leftEdge = getEdgeWithThikness(true, centerPixel, rowSobel, edgeThreshold, minEdgeWidth, maxEdgeWidth, minEdgeThicness);
	uint16_t rightEdge = getEdgeWithThikness(false, centerPixel, rowSobel, edgeThreshold, minEdgeWidth, maxEdgeWidth, minEdgeThicness);

	trackWidth = rightEdge - leftEdge;
	trackCenter = leftEdge + trackWidth/2;
}

void CameraAnalysis::SingleRowAnalysis::calculateTrackDifferences() {
	centerDiff = (int16_t)trackCenter - (int16_t)oldTrackCenter;
	widthDiff = (int16_t)trackWidth - (int16_t)oldTrackWidth;

	oldTrackCenter = trackCenter;
	oldTrackWidth = trackWidth;
}

//----------------------Print-------------------
void CameraAnalysis::SingleRowAnalysis::printImageRow(){
	printArray(rowDataBuffer, 316);
}

void CameraAnalysis::SingleRowAnalysis::printSobleRow(){
	printArray(rowSobel, 314);
}
