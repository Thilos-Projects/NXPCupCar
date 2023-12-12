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
	void Setup();
	void cameraAlgorythmus(Sceduler::taskHandle* self);
}

#endif /* TFT_MODULES_CAMERASYSTEM_H_ */
