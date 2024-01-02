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

#define TRACKLINE_OUTPUT_LENGTH 10

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
		EdgeOutput edges[157];

		void calculateEdges();
		//Ende Kantenerkennung


		//Linienerkennung
		//TrackLineOutput
		struct TrackLineOutput{
			uint16_t centerIndex;
			uint16_t width;

			void clear();
			bool isEmpty();
		};
		struct TrackLineInput{
			uint8_t minTrackLineWidth;		//current 5
			uint8_t maxTrackLineWidth;		//current 15
			uint8_t minEdgeWidth;			//current 0
			uint8_t maxEdgeWidth;			//current 6
		};

		TrackLineInput straightTrackLineInput;
		TrackLineInput rightCurveTrackLineInput;
		TrackLineInput leftCurveTrackLineInput;
		TrackLineInput crossingTrackLineInput;
		TrackLineInput finishTrackLineInput;

		TrackLineOutput straightTrackLinesOutput[TRACKLINE_OUTPUT_LENGTH];
		TrackLineOutput rightCurveTrackLinesOutput[TRACKLINE_OUTPUT_LENGTH];
		TrackLineOutput leftCurveTrackLinesOutput[TRACKLINE_OUTPUT_LENGTH];
		TrackLineOutput crossingTrackLinesOutput[TRACKLINE_OUTPUT_LENGTH];
		TrackLineOutput finishTrackLinesOutput[TRACKLINE_OUTPUT_LENGTH];

		void calculateTrackLines();
		//Ende Lininenerkennung



		struct Track{
			uint16_t left;
			uint16_t right;
			uint16_t width;

			void clear();
			bool isEmpty();
			uint16_t getCenter();
		};

		Track tracks[316];



		uint16_t minTrackWidth;			//current 130
		uint16_t maxTrackWidth;			//current 240

		void Setup(	Pixy2SPI_SS* pixy, uint16_t row, uint16_t minTrackWidth, uint16_t maxTrackWidth);

		void calculateValidTracks(TrackLineOutput* lines);


		//----------------------Print-------------------
		void printImageRow();
		void printSobleRow();
		void printEdges();
		void printTrackLines(TrackLineOutput* lines);
		void printFoundCenters();
	};
}

#endif /* TFT_MODULES_CAMERAANALYSIS_H_ */
