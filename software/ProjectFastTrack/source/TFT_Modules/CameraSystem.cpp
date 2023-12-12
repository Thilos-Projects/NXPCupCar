/*
 * CameraSystem.cpp
 *
 *  Created on: 12.12.2023
 *      Author: TFR
 */
//Pixy Sleep stuff, ist drinn, damit nicht vergessen wird das die Pixy idle waiting betreibt, eine bessere warriante sollte zum optimierungszeitraum gewählt werden.
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
	//#include <stdio.h>
	//#include <string.h>
	//#include <math.h>

	#include "Modules/mTimer.h"
	#include "Modules/mSpi.h"
}

#include "Pixy/Pixy2SPI_SS.h"
#include <TFT_Modules/CameraSystem.h>

//localer link für die pixy
Pixy2SPI_SS pixy;

//Camera minimal init
void CameraSystem::Setup(){
	pixy.init();
	pixy.changeProg("video");
}

//print any linear array of numbers
template<typename T>
void printArray(T* line, uint16_t length){
	printf("%d", *(line + 0));
	for(int i = 1; i < length; i++)
		printf(",\t%d", *(line + i));
	printf("\n");
}

//hohl zeile 104 aus dem bild
uint8_t* getImageRow(){
	//genug speicher ür eine linie, wird bei jedem lesen überschrieben
	static uint8_t rowDataBuffer[316];
	//hohlt zeile 104
	pixy.video.getGrayRect(0, 104, 158, 105, 1, 1, rowDataBuffer + 0, false);
	pixy.video.getGrayRect(158, 104, 316, 105, 1, 1, rowDataBuffer + 158, false);
	//gibt den pointer zur linie zurück
	return (uint8_t*)&rowDataBuffer;
}

//führt einen 1D sobel algorythmus auf einer reihe aus
int16_t* getSobelRow(uint8_t* rowDataBuffer){
	//genug speicher ür eine linie, wird bei jedem lesen überschrieben
	static int16_t rowSobleBuffer[314];

	//ja das ist ein 1D sobel
	for(int i = 0; i < 314; i++)
		rowSobleBuffer[i] = (((int16_t)(*(rowDataBuffer+i))) * 2 + ((int16_t)(*(rowDataBuffer+i+2))) * -2); //1D sobel ;)

	return (int16_t*)&rowSobleBuffer;
}

//eine struktur zum abspeichern einer kante
struct Edge{
	//der index des Pixels auf der x achse
	uint16_t startIndex;
	//die breite in pixeln
	uint16_t width;
	//ist die Kante ein übergang von weiß zu schwarz
	bool whiteToBlack;

	//löscht die kante
	void clear(){
		whiteToBlack = false;
		width = 0;
		startIndex = 0;
	}
	//ist der eintrag corrupt?
	bool corrupted(){
		return width == 0 && startIndex != 0;
	}
	//ist die Kante leer?
	bool isEmpty(){
		return startIndex == 0;
	}
};
//sucht alle Kanten in einer Reihe
Edge* getEdges(int16_t* rowSobleBuffer, int16_t schwellwert){
	static Edge edgesBuffer[157];
	uint8_t currentEdge = 0;

	edgesBuffer[currentEdge].clear();

	for(int i = 0; i < 314; i++){
		//ist der derzeitige wert außerhalb der schwellwerte
		if(rowSobleBuffer[i] < -schwellwert || schwellwert < rowSobleBuffer[i]){
			//wird derzeit eine Kante bearbeited
			if(edgesBuffer[currentEdge].isEmpty()){
				edgesBuffer[currentEdge].startIndex = i;
				edgesBuffer[currentEdge].width = 1;
				edgesBuffer[currentEdge].whiteToBlack = rowSobleBuffer[i] > 0;
			}
			else{
				//neue Edge ohne dass eine leer stelle gefunden wurde, fange eine neue kante an
				if(edgesBuffer[currentEdge].whiteToBlack != rowSobleBuffer[i] > 0){
					currentEdge++;		//neue edge
					edgesBuffer[currentEdge].startIndex = i;
					edgesBuffer[currentEdge].width = 1;
					edgesBuffer[currentEdge].whiteToBlack = rowSobleBuffer[i] > 0;
				}
				else{
					//erhöhe die breite der derzeitigen kante
					edgesBuffer[currentEdge].width++;
				}
			}
		}
		else{
			//fange eine neue kante an
			if(!edgesBuffer[currentEdge].isEmpty()){
				currentEdge++;		//neue edge
				edgesBuffer[currentEdge].clear();
			}
		}
	}

	//fülle das array mit einem leeren wert, wen der derzeitig letzte nicht leer ist
	if(!edgesBuffer[currentEdge].isEmpty()){
		currentEdge++;		//neue edge
		edgesBuffer[currentEdge].clear();
	}

	return (Edge*)&edgesBuffer;
}

//printed eine zeile aller gefundenen kanten
void printEdges(Edge* edges){
	uint8_t currentEdge = 0;
	while(!edges[currentEdge].isEmpty()){
		printf("%d, %d\t", edges[currentEdge].startIndex, edges[currentEdge].width);
		currentEdge++;
	}
	printf("\n");
}

//eine struktur zum abspeichern einer schwarzen lienie
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
//sucht alle schwarze linien in einer Reihe
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

//printed eine zeile aller gefundenen schwarzen lienien
void printTrackLines(TrackLine* lines){
	uint8_t currentLine = 0;
	while(!lines[currentLine].isEmpty()){
		printf("%d, %d\t", lines[currentLine].centerIndex, lines[currentLine].width);
		currentLine++;
	}
	printf("\n");
}

//eine struktur zum abspeichern einer möglichen track varriante
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
//sucht alle track varrianten und ordnet diese in ein array ein. Jede varriante ist anhand ihres mittelpunktes im array Indeziert.
TrackLineCombination* generateVallidTrackeLineCombinations(TrackLine* lines, uint16_t minTrackWidth, uint16_t maxTrackWidth){
	static TrackLineCombination trackeLineCombinations[316];

	for(int i = 0; i< 316; i++)
		trackeLineCombinations[i].clear();

	//teste alle kombinationen, aus schwarzen linien, um alle möglichen tracks zu generieren
	for(int i = 0; !lines[i].isEmpty(); i++){
		for(int j = i+1; !lines[j].isEmpty(); j++){
			uint16_t width = lines[j].centerIndex - lines[i].centerIndex;
			//schließt alle tracks aus, die zu breit oder kurz sind
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

//printed alle Tracks
void printTrackLineCombinations(TrackLineCombination* tracks){
	for(int i = 0; i < 316; i++)
		printf((tracks + i)->isEmpty() ? "_" : "-");
	printf("\n");
}

//führt ein Camera Tracking aus
void CameraSystem::cameraAlgorythmus(Sceduler::taskHandle* self){
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
	//printTrackLines(trackLineBuffer);
	trackLineCombinationsBuffer = generateVallidTrackeLineCombinations(trackLineBuffer, 130, 180);
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
