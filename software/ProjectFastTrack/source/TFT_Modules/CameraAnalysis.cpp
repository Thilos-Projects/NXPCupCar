
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
	#include "Modules/mTimer.h"
}

template<typename T>
void printArray(T* line, uint16_t length){
	printf("%d", *(line + 0));
	for(int i = 1; i < length; i++)
		printf(",\t%d", *(line + i));
	printf("\n");
}

template<typename T>
void printPartialArray(T* line, uint16_t length, uint16_t start, uint16_t end){
	printf("%d", *(line + 0));
	for(int i = start; i < end; i++)
		printf(",\t%d", *(line + i));
	printf("\n");
}


void CameraAnalysis::SingleRowAnalysis::Setup(Pixy2SPI_SS* pixy, uint16_t row, uint16_t edgeThreshold,
		uint8_t minEdgeWidth, uint8_t maxEdgeWidth, uint16_t centerPixel, uint16_t minThickness) {
	this->pixy = pixy;
	this->row = row;
	this->edgeThreshold = edgeThreshold;
	this->minEdgeWidth = minEdgeWidth;
	this->maxEdgeWidth = maxEdgeWidth;
	this->centerPixel = centerPixel;
	this->minThickness = minThickness;

	this->trackCenter = 0;
	this->trackWidth = 0;
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


uint16_t getEdgeWithThickness(bool isLeft, uint16_t centerPixel, int16_t* rowSobel, uint16_t edgeThreshold, uint8_t minEdgeWidth, uint8_t maxEdgeWidth, uint8_t minThickness) {
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

				if(edgeWidth < minThickness){
					//Sobel threshold reached, edge found, edge in wrong direction, edge with right, edge thickness was wrong
					isAnEdgeFound = false;
					continue;
				}

				//Sobel threshold reached, edge found, edge in wrong direction, edge with right, edge thickness was right
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

			if(edgeWidth > minThickness){
				//Sobel threshold not reached, edge found, edge with right, edge thickness was right
				break;
			}

			//Sobel threshold not reached, edge found, edge with right, edge thickness was wrong
			edgeWidth++;
			continue;
		}
	}

	if(!isAnEdgeFound)
		edgeStartPosition = isLeft ? 2 : 310;

	return edgeStartPosition;
}


void CameraAnalysis::SingleRowAnalysis::findBlankArea() {
	uint16_t leftEdge = getEdgeWithThickness(true, centerPixel, rowSobel, edgeThreshold, minEdgeWidth, maxEdgeWidth, minThickness);
	uint16_t rightEdge = getEdgeWithThickness(false, centerPixel, rowSobel, edgeThreshold, minEdgeWidth, maxEdgeWidth, minThickness);

	if(rightEdge <= leftEdge){
		// TODO: Error - Don't care :D
		// mLeds_Write(LedMaskEnum::kMaskLed4, LedStateEnum::kLedOn);
	}

	trackWidth = rightEdge - leftEdge;
	trackCenter = leftEdge + trackWidth/2;


	// printf("Mid: %d Width:%d leftEdge:%d rightEdge:%d \n", trackCenter, trackWidth, leftEdge, rightEdge);

}


//----------------------Print-------------------
void CameraAnalysis::SingleRowAnalysis::printImageRow(){
	printArray(rowDataBuffer, 316);
}
void CameraAnalysis::SingleRowAnalysis::printSobleRow(){
	printArray(rowSobel, 314);
}

// Column STUFF


void CameraAnalysis::SingleColumnAnalysis::Setup(Pixy2SPI_SS* pixy, uint16_t column, uint16_t edgeThreshold,
		uint8_t minEdgeWidth, uint8_t maxEdgeWidth, uint16_t minThickness) {
	this->pixy = pixy;
	this->column = column;
	this->edgeThreshold = edgeThreshold;
	this->minEdgeWidth = minEdgeWidth;
	this->maxEdgeWidth = maxEdgeWidth;
	this->minThickness = minThickness;
}
void CameraAnalysis::SingleColumnAnalysis::getImageColumn(){
	pixy->video.getGrayRect(column, 1, column + 1, 204+1, 1, 1, columnDataBuffer + 0, false);
}
void CameraAnalysis::SingleColumnAnalysis::calculateSobel(){
	for(int i = 0; i < 204; i++)
		columnSobel[i] = (((int16_t)(*(columnDataBuffer+i))) * 2 + ((int16_t)(*(columnDataBuffer+i+2))) * -2);
}


bool CameraAnalysis::SingleColumnAnalysis::detectObstacle(uint8_t start) {
	// 44 // 54 // 110 //

	// TODO: Move this stuff to setup
	uint8_t minDiffObstacleTop = 5;
	uint8_t maxDiffObstacleTop = 15;
	uint8_t minDiffObstacleFront = 60;
	uint8_t maxDiffObstacleFront = 100;

	uint8_t startPosition = 40;
	uint8_t endPosition = 200;
	uint8_t foundInThreshold = 0;
	uint8_t sobelInThreshold[206];
	uint8_t foundLength = 0;
	int16_t lastSobel = 0;
	for (uint8_t i = startPosition; i < endPosition; i++) {
		if(columnSobel[i] < -edgeThreshold || edgeThreshold < columnSobel[i]) {
			if (lastSobel != 0 && (lastSobel > 0) != (columnSobel[i] > 0)) {
				if (foundLength > minEdgeWidth && foundLength < maxEdgeWidth) {
					sobelInThreshold[foundInThreshold] = i;
					foundInThreshold++;
				}
				foundLength = 0;
			}
			foundLength++;
		} else {
			if (foundLength > minEdgeWidth && foundLength < maxEdgeWidth) {
				sobelInThreshold[foundInThreshold] = i;
				foundInThreshold++;
			}
			foundLength = 0;
		}
		lastSobel = columnSobel[i];
	}
	
	uint8_t firstEdge = 0, secondEdge = 0, thirdEdge = 0;
	bool foundObstacle = false;
	// Search first
	for (uint8_t i1 = 0; i1 < foundInThreshold; i1++) {
		// Search second
		for (uint8_t i2 = i1 + 1; i2 < foundInThreshold; i2++) {
			// Is over max?
			if (sobelInThreshold[i2] > sobelInThreshold[i1] + maxDiffObstacleTop)
				break;
			// Is second valid?
			if (sobelInThreshold[i2] >= sobelInThreshold[i1] + minDiffObstacleTop) {
					// Search third
					for (uint8_t i3 = i2 + 1; i3 < foundInThreshold; i3++) {
						// Is over max?
						if (sobelInThreshold[i3] > sobelInThreshold[i2] + maxDiffObstacleFront)
							break;
						// Is third valid?
						if (sobelInThreshold[i3] >= sobelInThreshold[i2] + minDiffObstacleFront) {
							firstEdge = sobelInThreshold[i1];
							secondEdge = sobelInThreshold[i2];
							thirdEdge = sobelInThreshold[i3];
							foundObstacle = true;
							break;
						}
					}
					if (foundObstacle)
						break;
				}
		}
		if (foundObstacle)
			break;
	}

	if (foundObstacle) {
		obstacleBottomEdge = thirdEdge;
	}

	return foundObstacle;

}

//----------------------Print-------------------
void CameraAnalysis::SingleColumnAnalysis::printImageColumn(){
	printArray(columnDataBuffer, 204);
}
void CameraAnalysis::SingleColumnAnalysis::printSobleColumn(){
	printArray(columnSobel, 202);
}
void CameraAnalysis::SingleColumnAnalysis::printLines(){
	printf("%d", 1);
	for(int i = 2; i < 204 +1; i++)
		printf(",\t%d", i);
	printf("\n");
}

//Partial Colum Stuff

void CameraAnalysis::PartialColumnAnalysis::Setup(Pixy2SPI_SS* pixy, uint16_t column, uint16_t startHeight, uint16_t endHeight, uint16_t edgeThreshold, uint8_t minEdgeWidth, uint8_t maxEdgeWidth, uint16_t minThickness) {
	this->pixy = pixy;
	this->column = column;
	this->startHeight = startHeight;
	this->endHeight = endHeight;
	this->edgeThreshold = edgeThreshold;
	this->minEdgeWidth = minEdgeWidth;
	this->maxEdgeWidth = maxEdgeWidth;
	this->minThickness = minThickness;
}
void CameraAnalysis::PartialColumnAnalysis::getImageColumn() {
	pixy->video.getGrayRect(column, startHeight, column+1, endHeight, 1, 1, columnDataBuffer + 0, false);
}
void CameraAnalysis::PartialColumnAnalysis::calculateSobel() {
	for(int i = 0; i < endHeight - startHeight - 2; i++)
		columnSobel[i] = (((int16_t)(*(columnDataBuffer+i))) * 2 + ((int16_t)(*(columnDataBuffer+i+2))) * -2);
}

bool CameraAnalysis::PartialColumnAnalysis::detectFinishline() {
	// TODO: Use configurable values
	const uint8_t threshold = 30;
	const uint8_t minWidth = 2;
	const uint8_t maxWidth = 6;
	const uint8_t minThicknes = 2;
	const uint8_t maxThicknes = 15;

	firstPos = 0;
	secondPos = 0;
	uint8_t state = 0;
	uint16_t counter = 0;

	for(int i = 0; i < endHeight - startHeight - 2 && secondPos == 0; i++){
		switch(state){
		case 0: {
			if(columnSobel[i] > threshold){
				state = 1;
				counter++;
			}
		} break;
		case 1: {
			if(columnSobel[i] > threshold){
				counter++;
			}else{
				if(counter < minWidth || counter > maxWidth) {
					state = 0;
					counter = 0;
				} else {
					state = 2;
					firstPos = i;
					counter = 0;
				}
			}
		}break;
		case 2: {
			if(columnSobel[i] < -threshold){
				state = 3;
				counter++;
			}
		} break;
		case 3: {
			if(columnSobel[i] < -threshold){
				counter++;
			}else{
				if(counter < minWidth || counter > maxWidth) {
					state = 2;
					counter = 0;
				} else {
					if(i-firstPos < minThicknes || i-firstPos > maxThicknes){
						state = 2;
						counter = 0;
						firstPos = i;
					}
					else{
						secondPos = i;
						state = 4;
					}
				}
			}
		}break;
		default : break;
		}
	}

	return secondPos != 0;
}

//----------------------Print-------------------
void CameraAnalysis::PartialColumnAnalysis::printImageColumn(){
	printPartialArray(columnDataBuffer, 316, 1, endHeight - startHeight);
}
void CameraAnalysis::PartialColumnAnalysis::printSobleColumn(){
	printPartialArray(columnSobel, 314, 1, endHeight - startHeight);
}
