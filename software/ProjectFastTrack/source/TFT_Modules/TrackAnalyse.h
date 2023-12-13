/*
 * TrackAnalyse.h
 *
 *  Created on: 13.12.2023
 *      Author: TFR
 */

#ifndef TFT_MODULES_TRACKANALYSE_H_
#define TFT_MODULES_TRACKANALYSE_H_

#include <stdint.h>
#include <stdlib.h>
#include "Pixy/Pixy2SPI_SS.h"

namespace TrackAnalys {
	struct TrackAnalyse{
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
		int16_t rowSobleBuffer[314];
		Edge edgesBuffer[157];
		TrackLine lines[156];
		Track tracks[316];

		Pixy2SPI_SS* pixy;
		uint16_t row;				//current 160
		uint16_t schwellwert;		//current 20
		uint8_t minLineWidth;		//current 5
		uint8_t maxLineWidth;		//current 15
		uint8_t minEdgeWidth;		//current 0
		uint8_t maxEdgeWidth;		//current 6
		uint16_t minTrackWidth;		//current 130
		uint16_t maxTrackWidth;		//current 240

		void Setup(	Pixy2SPI_SS* pixy, uint16_t row, uint16_t schwellwert, uint8_t minLineWidth, uint8_t maxLineWidth,
					uint8_t minEdgeWidth, uint8_t maxEdgeWidth, uint16_t minTrackWidth, uint16_t maxTrackWidth);

		void getImageRow();
		void getSobelRow();
		void getEdges();
		void findTrackLines();
		void generateVallidTrackes();

		void callAll();

		//----------------------Print-------------------
		void printImageRow();
		void printSobleRow();
		void printEdges();
		void printTrackLines();
		void printTrackLineCombinations();
	};
}

#endif /* TFT_MODULES_TRACKANALYSE_H_ */
