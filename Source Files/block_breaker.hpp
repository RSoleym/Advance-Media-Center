extern "C" {
	#include "LPC17xx.h"
}


class Breaker{

		protected:		
		static uint32_t dir;
		static int score;
		static bool game_start;
		static bool block_hit;
		static bool pad_change;
		static void delay(int);
		static int select_counter;
		static void fillRect(int, int, int, int);
		static void gameOverScreen();
		static bool gameOver;
		
		


	class Ball{ //Let Ball be "o" and 6 x 6 font
		friend class Block;
		friend class Pad;
		friend class Breaker;
		
		protected:
			static bool movX;
			static bool movY;
			static int prevX;
			static int prevY;
			static int ballposX;
			static int ballposY;
			static void ballMech();
	};

	class Block{ //Let Blocks be "=" and 32 x 16 font
			friend class Ball;
			friend class Pad;
			friend class Breaker;
		
			protected:
				static bool blockPos[4][10];	
				static void blockInit();
				static void blockDraw();
	};

	class Pad{ //Let Pad be "-" and 40 x 10 font
			friend class Ball;
			friend class Breaker;	
				
			protected:
				static int padX;
				static int padY;
				static int prevX;
				static int prevY;
				static void padMech();
};
		public:
			class Display{
	
				friend class Block;
				friend class Pad;
				friend class Ball;

				public:
					void breakerDisplay();
	};

};
