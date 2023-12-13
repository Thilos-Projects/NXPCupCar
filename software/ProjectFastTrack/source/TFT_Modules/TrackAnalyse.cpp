/*
 * TrackAnalyse.cpp
 *
 *  Created on: 13.12.2023
 *      Author: TFR
 */
void pixyUsleep(int useconds);

#include <TFT_Modules/TrackAnalyse.h>

#include <stdio.h>

template<typename T>
void printArray(T* line, uint16_t length){
	printf("%d", *(line + 0));
	for(int i = 1; i < length; i++)
		printf(",\t%d", *(line + i));
	printf("\n");
}

void TrackAnalys::TrackAnalyse::Edge::clear(){
	whiteToBlack = false;
	width = 0;
	startIndex = 0;
}
bool TrackAnalys::TrackAnalyse::Edge::corrupted(){
	return width == 0 && startIndex != 0;
}
bool TrackAnalys::TrackAnalyse::Edge::isEmpty(){
	return startIndex == 0;
}

void TrackAnalys::TrackAnalyse::TrackLine::clear(){
	centerIndex = 0;
}
bool TrackAnalys::TrackAnalyse::TrackLine::isEmpty(){
	return centerIndex == 0;
}

void TrackAnalys::TrackAnalyse::Track::clear(){
	left = 0;
	right = 0;
	width = 0;
}
bool TrackAnalys::TrackAnalyse::Track::isEmpty(){
	return left == 0 && right == 0 && width == 0;
}
uint16_t TrackAnalys::TrackAnalyse::Track::getCenter(){
	return left + width/2;
}

void TrackAnalys::TrackAnalyse::getImageRow(){
	pixy->video.getGrayRect(0, row, 158, row+1, 1, 1, rowDataBuffer + 0, false);
	pixy->video.getGrayRect(158, row, 316, row+1, 1, 1, rowDataBuffer + 158, false);
}
void TrackAnalys::TrackAnalyse::getSobelRow(){
	for(int i = 0; i < 314; i++)
		rowSobleBuffer[i] = (((int16_t)(*(rowDataBuffer+i))) * 2 + ((int16_t)(*(rowDataBuffer+i+2))) * -2);
}
void TrackAnalys::TrackAnalyse::getEdges(){
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
void TrackAnalys::TrackAnalyse::findTrackLines(){
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
void TrackAnalys::TrackAnalyse::generateVallidTrackes(){
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
void TrackAnalys::TrackAnalyse::callAll(){
	getImageRow();
	getSobelRow();
	getEdges();
	findTrackLines();
	generateVallidTrackes();
}

//----------------------Print-------------------
void TrackAnalys::TrackAnalyse::printImageRow(){
	printArray(rowDataBuffer, 316);
}
void TrackAnalys::TrackAnalyse::printSobleRow(){
	printArray(rowSobleBuffer, 314);
}
void TrackAnalys::TrackAnalyse::printEdges(){
	uint8_t currentEdge = 0;
	while(!edgesBuffer[currentEdge].isEmpty()){
		printf("%d, %d\t", edgesBuffer[currentEdge].startIndex, edgesBuffer[currentEdge].width);
		currentEdge++;
	}
	printf("\n");
}
void TrackAnalys::TrackAnalyse::printTrackLines(){
	uint8_t currentLine = 0;
	while(!lines[currentLine].isEmpty()){
		printf("%d, %d\t", lines[currentLine].centerIndex, lines[currentLine].width);
		currentLine++;
	}
	printf("\n");
}
void TrackAnalys::TrackAnalyse::printTrackLineCombinations(){
	for(int i = 0; i < 316; i++)
		printf((tracks + i)->isEmpty() ? "_" : "-");
	printf("\n");
}
