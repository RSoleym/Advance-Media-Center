#include <stdio.h>
#include "menu.hpp"

extern "C" {
	#include "LPC17xx.h"
	#include "Board Files/GLCD.h"
	#include "Board Files/LED.h"
	#include "Board_ADC.h"
	#include "Board Files/KBD.h"
}

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   1	

int main (void){
	ADC_Initialize();
  KBD_Init();
	#ifdef __USE_LCD
		GLCD_Init();
		LED_Init();
	#endif
	Menu menu;
	
	while (1){
			#ifdef __USE_LCD
        GLCD_Clear(White);                         /* Clear graphical LCD display  */
        GLCD_SetBackColor(Blue);
        GLCD_SetTextColor(Yellow);
        GLCD_DisplayString(0, 0, __FI, (unsigned char*) "    COE718 Project  ");
				GLCD_DisplayString(1, 0, __FI, (unsigned char*) "  Ramtin Soleymani   ");
				GLCD_SetTextColor(White);
        GLCD_DisplayString(2, 0, __FI, (unsigned char*) "       Main Menu     ");
        GLCD_SetBackColor(White);
        GLCD_SetTextColor(Blue);
    #endif
		
		menu.menuDisplay();
	}
}
