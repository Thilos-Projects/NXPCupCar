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


		uint16_t edgeThreshold; 		//current 20
		uint8_t minEdgeWidth;			//current 0
		uint8_t maxEdgeWidth;			//current 6
		uint16_t centerPixel;			//current 158
		uint16_t minThickness;			//current 158

		uint16_t trackCenter;
		uint16_t trackWidth;

		void findBlankArea();

		void Setup(Pixy2SPI_SS* pixy, uint16_t row, uint16_t edgeThreshold, 	uint8_t minEdgeWidth, uint8_t maxEdgeWidth, uint16_t pixelMitte, uint16_t minThickness);

		//----------------------Print-------------------
		void printImageRow();
		void printSobleRow();
	};
}

#endif /* TFT_MODULES_CAMERAANALYSIS_H_ */
