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
#include "Modules/mTimer.h"
#include "Modules/mCpu.h"		//mCpu_Setup
//#include "Modules/mSwitch.h"
#include "Modules/mLeds.h"
//#include "Modules/mAd.h"
#include "Modules/mDelay.h" //Pixy Änderungen: mDelay.h wird für das Auslesen der Pixy verwändert!!!
//#include "Modules/mRS232.h"
//#include "Modules/mVL6180x.h"

//#include "Applications/gInput.h"
//#include "Applications/gCompute.h"
//#include "Applications/gOutput.h"

}

/*----------------------------------*/
/* Pixy 2  Notwendiges um Pixy auszulesen*/
#include "Pixy/Pixy2SPI_SS.h"
#define	DELTA_T 34
#define K_MAIN_INTERVAL (DELTA_T / kPit1Period)
/*----------------------------------*/

int main(){
	printf("Hello Car\n");
	mCpu_Setup();
	mLeds_Setup();

	mTimer_Setup();
	mTimer_Open();

	mLeds_Write(kMaskLed1,kLedOn);

	mTimer_EnableHBridge();

	mTimer_SetMotorDuty(0.4f, 0.4f);




	//----------------------------------------------
	static Int16 sDelay;
	sDelay = mDelay_GetDelay(kPit1, K_MAIN_INTERVAL);
	//PRINTF("Hello World\n");
	printf("HellO World: %ld %d\n",clock(),CLOCKS_PER_SEC);

	printf("MyPixyTest\n");

    Pixy2SPI_SS pixy;
    pixy.init();
    pixy.getVersion();
    pixy.version->print();
    printf("HellO World: %ld\n",clock());
    pixy.setLED(0, 255, 0);
    //pixy.setLamp(1, 1);
    pixy.changeProg("video");

	/* Tom Ergänzungen: Pixy RGB ausprobieren */


	uint8_t rot, gruen, blau;
	uint8_t ergebnis;

	ergebnis = pixy.video.getRGB(250, 260, &rot, &gruen, &blau, false);

	for(;;) {
		ergebnis = pixy.video.getRGB(250, 260, &rot, &gruen, &blau, false);
		//printf("Ergebnis der getRGB Methode: %d\n",ergebnis);
		printf("Rot Wert: %d, Gruen Wert: %d, Blau Wert: %d\n",rot, gruen, blau);
	}



	/* Test: Pixy Line Traking API */
	/*
	pixy.changeProg("line");
	uint8_t ergebnis;

	for(;;) {
		ergebnis = pixy.line.getMainFeatures();
		printf("Ergebnis: %d\n",ergebnis);
		pixy.line.vectors->print();
	}
	*/
	printf("End of Pixy Test\n");

	//----------------------------------------------



	//bool up = true;
	//float test = 0;

	for(UInt32 i = 0; true; i++){

		if(mTimer_GetFaultMoteurLeft())
			printf("motor left fault");
		if(mTimer_GetFaultMoteurRight())
			printf("motor right fault");
		mLeds_Write(kMaskLed1,kLedOff);
		for(UInt32 j = 0; j < 2500000; j++);
		mLeds_Write(kMaskLed1,kLedOn);
		for(UInt32 j = 0; j < 2500000; j++);

		//if(up)
			//test+=0.1f;
		//else
			//test-=0.1f;

		//if(up && test > 0.9f)
		//	up = false;
		//if(!up && test < -0.9f)
		//	up = true;

		//mTimer_SetMotorDuty(test, 3/8.0f);

	}

	printf("Tshöööööööö\n");

	return 0;
}
