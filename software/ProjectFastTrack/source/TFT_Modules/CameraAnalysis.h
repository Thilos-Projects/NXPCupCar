/*
 * TrackAnalyse.h
 *
 *  Created on: 13.12.2023
 *      Author: TFR
 */

#ifndef TFT_MODULES_CAMERAANALYSIS_H_
#define TFT_MODULES_CAMERAANALYSIS_H_

#include <stdint.h>
#include <stdlib.h>
#include "Pixy/Pixy2SPI_SS.h"

namespace CameraAnalysis {

	struct SingleRowAnalysis{

		//Kamera Abfrage + Sobel
		uint8_t rowDataBuffer[316];
		int16_t rowSobel[314];

		Pixy2SPI_SS* pixy;
		uint16_t row;					//current 160

		void getImageRow();
		void calculateSobelRow();
		//Stop Kamera Abfrage + Sobel

		//Kantenerkennung
		struct EdgeOutput{
			uint16_t startIndex;
			uint16_t width;
			bool whiteToBlack;

			void clear();
			bool corrupted();
			bool isEmpty();
		};

		struct EdgeInput{
			uint16_t edgeThreshold;			//current 20
		};

		EdgeInput edgeInputs[1]; //Bei Kantenschwellwert Problemen hier mehr hinzufügen
		void calculateEdges();



		struct TrackLine{
			uint16_t centerIndex;
			uint16_t width;

			void clear();
			bool isEmpty();
		};
		struct Track{
			uint16_t left;
			uint16_t right;
			uint16_t width;

			void clear();
			bool isEmpty();
			uint16_t getCenter();
		};

		EdgeOutput edges[157];
		TrackLine lines[156];
		Track tracks[316];


		uint8_t minTrackLineWidth;		//current 5
		uint8_t maxTrackLineWidth;		//current 15
		uint8_t minEdgeWidth;			//current 0
		uint8_t maxEdgeWidth;			//current 6
		uint16_t minTrackWidth;			//current 130
		uint16_t maxTrackWidth;			//current 240

		void Setup(	Pixy2SPI_SS* pixy, uint16_t row, uint8_t minTrackLineWidth, uint8_t maxTrackLineWidth,
					uint8_t minEdgeWidth, uint8_t maxEdgeWidth, uint16_t minTrackWidth, uint16_t maxTrackWidth);

		void calculateTrackLines();
		void calculateValidTracks();


		//----------------------Print-------------------
		void printImageRow();
		void printSobleRow();
		void printEdges();
		void printTrackLines();
		void printFoundCenters();
	};
}

#endif /* TFT_MODULES_CAMERAANALYSIS_H_ */
