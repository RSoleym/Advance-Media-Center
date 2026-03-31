#include "game_center.hpp"
#include "block_breaker.hpp"
#include "snake_game.hpp"
#include "stdio.h"

extern "C" {
	#include "LPC17xx.h"
	#include "Board Files/GLCD.h"
	#include "Board Files/LED.h"
	#include "Board Files/KBD.h"
}

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   1	

int Game::gameFunc(){
	game_select = 0;
	last_selection = -1;
	back_counter = 0;
	
	dir = get_button();
	
	while (dir != KBD_SELECT && back_counter != 2){
		dir = get_button();
		switch(dir){
			case(KBD_DOWN):
				back_counter = 0;
				game_select --;
				if (game_select < 0) game_select = 1;
				while (get_button() == KBD_DOWN);
				break;
			case(KBD_UP):
				back_counter = 0;
				game_select ++;
				if (game_select > 1) game_select = 0;
				while (get_button() == KBD_UP);
				break;
			case(KBD_LEFT):
				back_counter ++;
				while (get_button() == KBD_LEFT);
				break;
			case(KBD_RIGHT):
				back_counter = 0;
				break;
		}
		
		if (game_select != last_selection){
			last_selection = game_select;
      if (game_select == 0) {
				printf("1. Block Breaker (Highlighted)\n");
      } else {
        printf("1. Block Breaker\n");
      }
      if (game_select == 1) {
				printf("2. Snake Game (Highlighted)\n");
      } else {
        printf("2. Snake Game\n");
      }
      #ifdef __USE_LCD
				// Photo Gallery (Row 5)
        if (game_select == 0) {
					GLCD_SetTextColor(White);
          GLCD_SetBackColor(Blue);
        } else {
          GLCD_SetTextColor(Black);
          GLCD_SetBackColor(White);
        }
        GLCD_DisplayString(5, 0, __FI, (unsigned char*) "1. Block Breaker");

        // MP3 Player (Row 7)
        if (game_select == 1) {
          GLCD_SetTextColor(White);
          GLCD_SetBackColor(Blue);
        } else {
          GLCD_SetTextColor(Black);
          GLCD_SetBackColor(White);
        }
        GLCD_DisplayString(7, 0, __FI, (unsigned char*) "2. Snake Game");

      #endif
    }
	}
	if (back_counter == 2){
		return back_counter;
	} else{
		while (get_button() == KBD_SELECT);
		return game_select;
	}
}	


void Game::gameDisplay(){
	
  GLCD_Clear(White);                         /* Clear graphical LCD display  */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
	GLCD_DisplayString(0, 0, __FI, (unsigned char*) "    COE718 Project  ");
  GLCD_DisplayString(1, 0, __FI, (unsigned char*) "  Ramtin Soleymani   ");
  GLCD_SetTextColor(White);
  GLCD_DisplayString(2, 0, __FI, (unsigned char*) "       Game Center     ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
	
	
	class_selection = gameFunc();
	Breaker::Display breaker;
	SnakeGame snake;
	
	switch (class_selection){
		case(0):
			breaker.breakerDisplay();
			printf("Running Block Breaker \n");
			break;
		case(1):
			snake.start();
			printf("Running Snake Game \n");
			break;
		case(2):
			printf("Back \n");
			break;
	}
}
