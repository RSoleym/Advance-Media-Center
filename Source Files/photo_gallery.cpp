#include "photo_gallery.hpp"
#include <stdio.h>
extern "C"{
	#include "Images/one_piece.c"
	#include "Images/nightwing.c"
	#include "Images/bleach.c"
	#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
	#include "string.h"
	#include "Board Files/GLCD.h"
	#include "Board Files/KBD.h"
}

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   1	


void Gallery::nightPhoto() const{
	#ifdef __USE_LCD
			GLCD_Clear(White);                         /* Clear graphical LCD display  */
			GLCD_SetBackColor(Blue);
			GLCD_SetTextColor(Yellow);
			GLCD_DisplayString(9 , 0, __FI, (unsigned char*) "  Select To Go Back  ");
			GLCD_Bitmap (0, 0, NIGHTWING_WIDTH ,  NIGHTWING_HEIGHT, NIGHTWING_PIXEL_DATA);
	#endif
	
	printf("This is Nightwing! \n");
}

void Gallery::onePhoto() const{
	#ifdef __USE_LCD
			GLCD_Clear(White);                         /* Clear graphical LCD display  */
			GLCD_SetBackColor(Blue);
			GLCD_SetTextColor(Yellow);
			GLCD_DisplayString(9 , 0, __FI, (unsigned char*) "  Select To Go Back  ");
			GLCD_Bitmap (0, 0, ONE_PIECE_WIDTH ,  ONE_PIECE_HEIGHT, ONE_PIECE_PIXEL_DATA);
	#endif
	
	printf("This is One Piece! \n");
}

void Gallery::bleachPhoto() const{
	#ifdef __USE_LCD
			GLCD_Clear(White);                         /* Clear graphical LCD display  */
			GLCD_SetBackColor(Blue);
			GLCD_SetTextColor(Yellow);
			GLCD_DisplayString(9 , 0, __FI, (unsigned char*) "  Select To Go Back  ");
			GLCD_Bitmap (0, 0, BLEACH_WIDTH ,  BLEACH_HEIGHT, BLEACH_PIXEL_DATA);
	#endif
	
	printf("This is Bleach! \n");
}


void Gallery::photoDisplay(){	
	photo_selection = 0;
	last_selection = -1;
	dir = get_button();
	
	#ifdef __USE_LCD
	  GLCD_Clear(White);                         /* Clear graphical LCD display  */
    GLCD_SetBackColor(Blue);
    GLCD_SetTextColor(Yellow);
		GLCD_DisplayString(9 , 0, __FI, (unsigned char*) "  Select To Go Back  ");
	#endif
	
	
	while (dir !=KBD_SELECT){
		dir = get_button();
		switch (dir){
			case (KBD_LEFT):
				photo_selection --;
				if (photo_selection < 0) photo_selection = 2;
				while (get_button() == KBD_LEFT);
				break;
			case (KBD_RIGHT):
				photo_selection ++;
				if (photo_selection > 2) photo_selection = 0;
				while (get_button() == KBD_RIGHT);
				break;
		}
		
		if (last_selection != photo_selection){
			last_selection = photo_selection;
			switch (photo_selection){
				case(0):
					nightPhoto();
					break;
				case(1):
					onePhoto();
					break;
				case(2):
					bleachPhoto();
					break;
			}
		}
	}
	while (get_button() == KBD_SELECT);
}
