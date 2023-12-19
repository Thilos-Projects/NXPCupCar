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
		struct Edge{
			uint16_t startIndex;
			uint16_t width;
			bool whiteToBlack;

			void clear();
			bool corrupted();
			bool isEmpty();
		};
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

		uint8_t rowDataBuffer[316];
		int16_t rowSoble[314];
		Edge edges[157];
		TrackLine lines[156];
		Track tracks[316];

		Pixy2SPI_SS* pixy;
		uint16_t row;					//current 160
		uint16_t edgeThreshold;			//current 20
		uint8_t minTrackLineWidth;		//current 5
		uint8_t maxTrackLineWidth;		//current 15
		uint8_t minEdgeWidth;			//current 0
		uint8_t maxEdgeWidth;			//current 6
		uint16_t minTrackWidth;			//current 130
		uint16_t maxTrackWidth;			//current 240

		void Setup(	Pixy2SPI_SS* pixy, uint16_t row, uint16_t edgeThreshold, uint8_t minTrackLineWidth, uint8_t maxTrackLineWidth,
					uint8_t minEdgeWidth, uint8_t maxEdgeWidth, uint16_t minTrackWidth, uint16_t maxTrackWidth);

		void getImageRow();
		void calculateSobelRow();
		void calculateEdges();
		void calculateTrackLines();
		void calculateValidTracks();

		void callAll();

		//----------------------Print-------------------
		void printImageRow();
		void printSobleRow();
		void printEdges();
		void printTrackLines();
		void printTrackLineCombinations();
	};
}

#endif /* TFT_MODULES_CAMERAANALYSIS_H_ */
