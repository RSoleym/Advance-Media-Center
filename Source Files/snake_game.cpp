#include "snake_game.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

extern "C" {
    #include <LPC17xx.h>
    #include "Images/background.c"
    #include "Images/head.c"
    #include "Images/body.c"
    #include "Images/apple.c"
    #include "Board Files/GLCD.h"
		#include "Board Files/LED.h"
		#include "Board Files/KBD.h"
}

#define __FI        0                      /* Font index 8x6               */
#define __USE_LCD   1	


uint32_t dir;

void SnakeGame::gameOverScreen() const {
    #ifdef __USE_LCD
        dir = get_button();
        GLCD_Clear(Black);                         /* Clear graphical LCD display  */
        GLCD_SetTextColor(Red);
        GLCD_DisplayString(5 , 4, 1, (unsigned char*) "Game Over!");
        GLCD_DisplayString(6 , 0, 1, (unsigned char*) "Press Select to Restart");
        while (dir != KBD_SELECT){
            dir = get_button();
        }
    #endif
}


void SnakeGame::drawBackground() const {
    #ifdef __USE_LCD
        for (int i = 0; i < fullWidth; i++){
            for (int j = 0; j < fullHeight; j++){
               GLCD_Bitmap(boardWidth + ( BACKGROUND_WIDTH * i), boardHeight + ( BACKGROUND_HEIGHT * j), BACKGROUND_WIDTH, BACKGROUND_HEIGHT, BACKGROUND_PIXEL_DATA);
            }
        }
    #endif
}

void SnakeGame::drawFood() const {
    #ifdef __USE_LCD
        GLCD_Bitmap(boardWidth + (APPLE_WIDTH * foodLocation.x), boardHeight + (APPLE_HEIGHT * foodLocation.y), APPLE_WIDTH, APPLE_HEIGHT, APPLE_PIXEL_DATA);
    #endif
}

void SnakeGame::clearSnake() const {
    #ifdef __USE_LCD
        GLCD_Bitmap(boardWidth + (BACKGROUND_WIDTH * previousTail.x), boardHeight + (BACKGROUND_HEIGHT * previousTail.y), BACKGROUND_WIDTH, BACKGROUND_HEIGHT, BACKGROUND_PIXEL_DATA);
    #endif
}


void SnakeGame::drawSnake() const {
    #ifdef __USE_LCD
        for (int i = 0; i < snakeSize; i++){
            if (i == 0){
                GLCD_Bitmap(boardWidth + (HEAD_WIDTH * snakeBody[i].x), boardHeight + (HEAD_HEIGHT * snakeBody[i].y), HEAD_WIDTH, HEAD_HEIGHT, HEAD_PIXEL_DATA);
            } else {
                GLCD_Bitmap(boardWidth + (BODY_WIDTH * snakeBody[i].x), boardHeight + (BODY_HEIGHT * snakeBody[i].y), BODY_WIDTH, BODY_HEIGHT, BODY_PIXEL_DATA);
            }
        }
    #endif
}

bool SnakeGame::wallCollision() const {
    if (snakeBody[0].x < 0 || snakeBody[0].x >= fullWidth || snakeBody[0].y < 0 || snakeBody[0].y >= fullHeight){ 
        return true;
    }else {
        return false;
    }
}

bool SnakeGame::selfCollision() const {
    for (int i = 1; i < snakeSize; i++){
        if (snakeBody[0].x == snakeBody[i].x && snakeBody[0].y == snakeBody[i].y){
            return true;
        }
    }
    return false;
}

bool SnakeGame::foodCollision() const {
    if (snakeBody[0].x == foodLocation.x && snakeBody[0].y == foodLocation.y){
        return true;
    } else {
        return false;
    }
}

void SnakeGame::moveSnake() {
    previousTail.x = snakeBody[snakeSize - 1].x;
    previousTail.y = snakeBody[snakeSize - 1].y;

    for (int i = snakeSize - 1; i > 0; i--){
        snakeBody[i].x = snakeBody[i - 1].x;
        snakeBody[i].y = snakeBody[i - 1].y;
    }

    switch (currentDirection){
        case up:
            snakeBody[0].y -= 1;
            break;
        case down:
            snakeBody[0].y += 1;
            break;
        case left:
            snakeBody[0].x -= 1;
            break;
        case right:
            snakeBody[0].x += 1;
            break;
    }
    
}

void SnakeGame::spawnFood() {
    bool conflict = true;
   
    while (conflict) {
        conflict = false; 

        foodLocation.x = rand() % fullWidth;
        foodLocation.y = rand() % fullHeight;

        for (int i = 0; i < snakeSize; i++) {
            if (foodLocation.x == snakeBody[i].x && foodLocation.y == snakeBody[i].y) {
                conflict = true;
                break;        
            }
        }
    }
    drawFood();
}


void SnakeGame::initGame() {
    snakeSize = 3;
    location head = {(fullWidth-1) / 2, (fullHeight-1) / 2};
    snakeBody[0] = head;
    location body1 = {head.x - 1, head.y};
    snakeBody[1] = body1;
    location body2 = {body1.x - 1, body1.y};
    snakeBody[2] = body2;
		
    currentDirection = right;

    foodLocation.x = head.x + 5;
    foodLocation.y = head.y;
    
    drawBackground();
    drawFood();
    drawSnake();

    score = 0;
}

void SnakeGame::start() {
	
		game_start = false;
		gameOver = false;
		score = 0;
    
		srand(1234);
    int select_counter = 0;

    dir = get_button();


    while (select_counter != 2){
		dir = get_button();
		if (dir == KBD_LEFT || dir == KBD_RIGHT || dir == KBD_UP || dir == KBD_DOWN) select_counter = 0;
		
        if (game_start == false){
            if (gameOver == true){
               gameOverScreen();
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
                GLCD_DisplayString(29 , 0, __FI, (unsigned char*) "Snake Game (Ramtin Soleymani)");
			#endif
            

            initGame();
						
            while (dir != KBD_LEFT && dir != KBD_RIGHT && select_counter != 2){
				dir = get_button();
				if (dir == KBD_SELECT) select_counter++;
			}
			game_start = true;

        } else {
            if (dir == KBD_UP && currentDirection != down){
                currentDirection = up;
            } else if (dir == KBD_DOWN && currentDirection != up){
                currentDirection = down;
            } else if (dir == KBD_LEFT && currentDirection != right){
                currentDirection = left;
            } else if (dir == KBD_RIGHT && currentDirection != left){
                currentDirection = right;
            } else if (dir == KBD_SELECT){
                select_counter++;
        
            }   
            
            clearSnake();
            moveSnake();
            drawSnake();

            if (wallCollision() || selfCollision()){
                gameOver = true;
                game_start = false;
            
            } else if (foodCollision()){
                score += 10;
								sprintf(scoreDisplay, "Your Score: %d", score);
                GLCD_DisplayString(27 , 0, __FI, (unsigned char*) scoreDisplay);
                location newSegment = {previousTail.x, previousTail.y};
								snakeSize ++;
                snakeBody[snakeSize - 1] = newSegment;
                spawnFood();
            }
            for (int delay = 0; delay < 9000000; delay++);
        }
    }
}
