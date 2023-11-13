//muss sein weil nur c fähig
extern "C"
{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "MK64F12.h"
//#include "fsl_debug_console.h"

//hier die SD-card
//BuildError: #include "sdcard/sdmmc_config.h"

//#include "Modules/mSpi.h"
//#include "Modules/mDac.h"
//#include "Modules/mAccelMagneto.h"
//#include "Modules/mGyro.h"
//#include "Modules/mTimer.h"
//#include "Modules/mCpu.h"		//mCpu_Setup
//#include "Modules/mSwitch.h"
//#include "Modules/mLeds.h"
//#include "Modules/mAd.h"
//#include "Modules/mDelay.h"
//#include "Modules/mRS232.h"
//#include "Modules/mVL6180x.h"

//#include "Applications/gInput.h"
//#include "Applications/gCompute.h"
//#include "Applications/gOutput.h"
}

//auskommentieren für final build
#define _SERIAL_DEBUGGER_

//ersetzt prinf für bessere auskommentier möglichkeiten.
#ifdef _SERIAL_DEBUGGER_
#define _printf(n) printf(n)
#else
#define _printf(n)
#endif

int main(){
	_printf("Hello Car\n");






	return 0;
}
