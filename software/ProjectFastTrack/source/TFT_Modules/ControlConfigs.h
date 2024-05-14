#ifndef _TFTMODULES_CONTROLCONFIGS_H_
#define _TFTMODULES_CONTROLCONFIGS_H_
extern "C"
{
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
}

#include "ControlConfigStruct.h"

void loadControlConfigs(uint8_t* controlConfigsLength, ControlConfig* controlConfigs);

#endif
