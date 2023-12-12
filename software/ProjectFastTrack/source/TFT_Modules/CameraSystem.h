/*
 * CameraSystem.h
 *
 *  Created on: 12.12.2023
 *      Author: TFR
 */

#ifndef TFT_MODULES_CAMERASYSTEM_H_
#define TFT_MODULES_CAMERASYSTEM_H_

#include <TFT_Modules/Sceduler.h>

namespace CameraSystem{
	struct Line{
		struct Edge{
			uint16_t startIndex;
			uint16_t width;
			bool whiteToBlack;

			void clear(){
				whiteToBlack = false;
				width = 0;
				startIndex = 0;
			}
			bool corrupted(){
				return width == 0 && startIndex != 0;
			}
			bool isEmpty(){
				return startIndex == 0;
			}
		};
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
		struct Track{
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
		uint16_t rowIndex;
		uint16_t threshold;
		uint16_t minEdgeWidth;
		uint16_t maxEdgeWidth;
		uint16_t minTrackLineWidth;
		uint16_t maxTrackLineWidth;
		uint16_t minTrackWidth;
		uint16_t maxTrackWidth;

		uint8_t cameraDataBufer[316];
		uint8_t sobleOutputBuffer[314];
		Edge edgesOutputBuffer[157];
		TrackLine trackLinesOutputBuffer[156];
		Track tracks[316];

		void setupLine(uint16_t rowInd, uint16_t threshold, uint16_t minEdgeWidth, uint16_t maxEdgeWidth, uint16_t minTrackLineWidth, uint16_t maxTrackLineWidth, uint16_t minTrackWidth, uint16_t maxTrackWidth);

		void getLineFromPixy();
		void do1DSoble();
		void doEdgeDetection();
		void doTrackLineDetection();
		void doTrackDetection();
	};

	void Setup();
	bool analyzeLine(Line* line, float* output);
}

#endif /* TFT_MODULES_CAMERASYSTEM_H_ */
