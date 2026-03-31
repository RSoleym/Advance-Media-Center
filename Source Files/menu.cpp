#include <stdio.h>  

#include "menu.hpp"
#include "photo_gallery.hpp"
#include "game_center.hpp"



extern "C" {
	extern int audio_main(void);
	#include "LPC17xx.h"
	#include "Board Files/GLCD.h"
	#include "Board Files/LED.h"
	#include "Board_ADC.h"
	#include "Board Files/KBD.h"
}

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   1	

int Menu::displayFunc() {
  menu_selection = 0;
  last_selection = -1;
	dir = get_button();
	
  while (dir != KBD_SELECT){
		dir = get_button();
		if (dir == KBD_UP) {
			menu_selection--;
      if (menu_selection < 0) menu_selection = 2;
			while (get_button() == KBD_UP);
		} else if (dir == KBD_DOWN) {
      menu_selection++;
      if (menu_selection > 2) menu_selection = 0;
			while (get_button() == KBD_DOWN);
    }
    
    if (menu_selection != last_selection) {
			last_selection = menu_selection;
      if (menu_selection == 0) {
				printf("1. Photo Gallery (Highlighted)\n");
      } else {
        printf("1. Photo Gallery\n");
      }
      if (menu_selection == 1) {
				printf("2. MP3 Player (Highlighted)\n");
      } else {
        printf("2. MP3 Player\n");
      }
      if (menu_selection == 2) {
				printf("3. Game Center (Highlighted)\n");
      } else {
        printf("3. Game Center\n");
      }

      #ifdef __USE_LCD
				// Photo Gallery (Row 5)
        if (menu_selection == 0) {
					GLCD_SetTextColor(White);
          GLCD_SetBackColor(Blue);
        } else {
          GLCD_SetTextColor(Black);
          GLCD_SetBackColor(White);
        }
        GLCD_DisplayString(5, 0, __FI, (unsigned char*) "1. Photo Gallery");

        // MP3 Plasyer (Row 7)
        if (menu_selection == 1) {
          GLCD_SetTextColor(White);
          GLCD_SetBackColor(Blue);
        } else {
          GLCD_SetTextColor(Black);
          GLCD_SetBackColor(White);
        }
        GLCD_DisplayString(7, 0, __FI, (unsigned char*) "2. MP3 Player");

        // Game (Row 9)
        if (menu_selection == 2) {
          GLCD_SetTextColor(White);
          GLCD_SetBackColor(Blue);
        } else {
          GLCD_SetTextColor(Black);
          GLCD_SetBackColor(White);
        }
          GLCD_DisplayString(9, 0, __FI, (unsigned char*) "3. Game Center");
      #endif
    }
	}
	while (get_button() == KBD_SELECT); 
	return menu_selection;
}
	
void Menu::menuDisplay(){
	class_selection = displayFunc();
	Gallery photo_gallery;
	Game game_center;
	
	switch (class_selection){
		case(0):
			printf("option 1 \n");
			photo_gallery.photoDisplay();
			break;
		case(1):
			printf("option 2 \n");
			audio_main();
			break;
		case(2):
			printf("option 3 \n");
			game_center.gameDisplay();
			break;
	}
}
