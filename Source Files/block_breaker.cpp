#include <stdio.h>
#include "block_breaker.hpp"
#include <string> 

extern "C" {
	#include "Images/pad.c"
	#include "Images/brick.c"
	#include "Images/ball.c"
	#include "LPC17xx.h"
	#include "Board Files/GLCD.h"
	#include "Board Files/LED.h"
	#include "Board Files/KBD.h"
}


#define __USE_LCD   1	
#define __FI        0                      /* Font index 8x6               */


// Top-level Breaker
uint32_t Breaker::dir          = 0;
int      Breaker::score        = 0;
bool     Breaker::game_start   = false;
bool     Breaker::block_hit    = false;
bool     Breaker::pad_change   = false;
int      Breaker::select_counter = 0;
bool 		 Breaker::gameOver		 = false;

// ---- Ball ("o", 6x6) ----
bool     Breaker::Ball::movX    = true;   // start moving to the right (your choice)
bool     Breaker::Ball::movY    = true;   // start moving down (your choice)
int      Breaker::Ball::prevX   = 0;
int      Breaker::Ball::prevY   = 0;
int      Breaker::Ball::ballposX = 0;
int      Breaker::Ball::ballposY = 0;

// ---- Block ("=", 32x16) ----
bool     Breaker::Block::blockPos[4][10]   = { { false } };


// ---- Pad ("-", 40x10) ----
int      Breaker::Pad::padX    = 0;
int      Breaker::Pad::padY    = 0;
int      Breaker::Pad::prevX   = 0;
int      Breaker::Pad::prevY   = 0;


void Breaker::gameOverScreen() {
    #ifdef __USE_LCD
        dir = get_button();
        GLCD_Clear(Black);                         /* Clear graphical LCD display  */
        GLCD_SetTextColor(Red);
        GLCD_DisplayString(5 ,4,  1, (unsigned char*) "Game Over!");
        GLCD_DisplayString(6 , 0, 1, (unsigned char*) "Press Select to Restart");
        while (dir != KBD_SELECT){
            dir = get_button();
        }
    #endif
}

void Breaker::fillRect(int x, int y, int w, int h) {
    GLCD_SetTextColor(White);
    for (int yy = 0; yy < h; yy++) {
        for (int xx = 0; xx < w; xx++) {
            GLCD_PutPixel(x + xx, y + yy); 
        }
    }
	}

void Breaker::delay(int x){
	for (int i = 0; i < x; i++);
}

void Breaker::Ball::ballMech(){
	
	if (game_start){
		
		if (ballposX>=314 || ballposX<=0 ) movX = !movX;
		
		if (ballposY<=0) movY = !movY;
		
		if (ballposY>=204){
			game_start = false;
			gameOver = true;
		}	
		
		if ((ballposX >= (Pad::padX-6) && ballposX <= (Pad::padX + 40)) && (ballposY == 190)) movY = !movY;
		
		
		if (ballposY <= 80) {   // only check blocks near top
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 10; j++) {

            if (!Block::blockPos[i][j]) continue;

            int brickX = j * 32;
            int brickY = i * 16;

            bool overlapX = ballposX < brickX + 32 &&
                            ballposX + 6 > brickX;

            bool overlapY = ballposY < brickY + 16 &&
                            ballposY + 6 > brickY;

            if (overlapX && overlapY) {

                Block::blockPos[i][j] = false;
                block_hit = true;

                // Bounce logic
                if (prevY + 6 <= brickY || prevY >= brickY + 16)
                    movY = !movY;   // hit top/bottom
                else
                    movX = !movX;   // hit left/right

                return;  // stop checking after 1 block
            }
        }
    }
	}


		prevX = ballposX;
		prevY = ballposY;
		
		if(movX) ballposX ++;
		else ballposX --;
		
		if(movY) ballposY ++;
		else ballposY --;
		
		delay(500000);
		
	}else {
		ballposX = 155;
		ballposY = 188;
		movX = true;
		movY = false;
		
	}
	
}

void Breaker::Block::blockDraw(){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 10; j++){
			if (Block::blockPos[i][j] == false) fillRect(32*j, 16*i, BRICK_WIDTH, BRICK_HEIGHT);
			else GLCD_Bitmap(32*j, 16*i, BRICK_WIDTH ,  BRICK_HEIGHT, BRICK_PIXEL_DATA);
		}
	}
}

void Breaker::Block::blockInit(){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 10; j++){
			Block::blockPos[i][j] = true;
		}
	}
}



void Breaker::Pad::padMech(){
	dir = get_button();
	
	if (game_start){
		prevX = padX;
	  prevY = padY;
		switch (dir){
			case (KBD_LEFT):
				prevX = padX;
				if (padX != 0) padX -= 20;
				break;
			case (KBD_RIGHT):
				prevY = padY;
				if (padX != 280) padX += 20;
				break;
		}
	} else{
		padX = 140;
		padY = 196;
	}
	
}


void Breaker::Display::breakerDisplay(){
	select_counter = 0;
	game_start = false;
	gameOver = false;
	char scoreDisplay[20];
	
	dir = get_button();
	
	while (select_counter != 2){
		dir = get_button();
		if (dir == KBD_LEFT || dir == KBD_RIGHT || dir == KBD_UP || dir == KBD_DOWN) select_counter = 0;
		
		if (game_start == false){
			if (gameOver == true){
							Breaker::gameOverScreen();
               gameOver = false;
						}
			
			score = 0;
			#ifdef __USE_LCD
				GLCD_Clear(White);                         /* Clear graphical LCD display  */
				GLCD_SetBackColor(Black);
				GLCD_SetTextColor(White);
        sprintf(scoreDisplay, "Your Score: %d", score);
        GLCD_DisplayString(27 , 0, __FI, (unsigned char*) scoreDisplay);
				GLCD_DisplayString(28 , 0, __FI, (unsigned char*) "To Go Back (Select Twice)");
        GLCD_DisplayString(29 , 0, __FI, (unsigned char*) "Block Breaker (Ramtin Soleymani)");
			#endif
			
			Breaker::Block::blockInit();
			Breaker::Block::blockDraw();
			Breaker::Ball::ballMech();
			Breaker::Pad::padMech();
			
			GLCD_Bitmap(Breaker::Ball::ballposX, Breaker::Ball::ballposY, BALL_WIDTH ,  BALL_HEIGHT, BALL_PIXEL_DATA);
			GLCD_Bitmap(Breaker::Pad::padX, Breaker::Pad::padY, PAD_WIDTH , PAD_HEIGHT, PAD_PIXEL_DATA);
			
			while (dir != KBD_LEFT && dir != KBD_RIGHT && select_counter != 2){
				dir = get_button();
				if (dir == KBD_SELECT) select_counter++;
			}
			game_start = true;
		} else{
			Breaker::Ball::ballMech();
			fillRect(Breaker::Ball::prevX, Breaker::Ball::prevY, BALL_WIDTH , BALL_HEIGHT);
			GLCD_Bitmap(Breaker::Ball::ballposX, Breaker::Ball::ballposY, BALL_WIDTH ,  BALL_HEIGHT, BALL_PIXEL_DATA);
			
			if (block_hit){
				Breaker::Block::blockDraw();
				score += 10;
				sprintf(scoreDisplay, "Your Score: %d", score);
        GLCD_DisplayString(27 , 0, __FI, (unsigned char*) scoreDisplay);
				block_hit = false;
			}
			
			if (dir == KBD_LEFT || dir == KBD_RIGHT){
				Breaker::Pad::padMech();
				fillRect(Breaker::Pad::prevX, Breaker::Pad::prevY, PAD_WIDTH , PAD_HEIGHT);
				GLCD_Bitmap(Breaker::Pad::padX, Breaker::Pad::padY, PAD_WIDTH , PAD_HEIGHT, PAD_PIXEL_DATA);
			}
			
			if (dir == KBD_SELECT) select_counter ++;
			
		}			
	}
}
