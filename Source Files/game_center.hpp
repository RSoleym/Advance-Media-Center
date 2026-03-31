extern "C" {
	#include "LPC17xx.h"
}

class Game{
	private:
		int gameFunc();
		uint32_t dir;
		int game_select;
		int back_counter;
    int last_selection;
		int class_selection;
	
	public:
		void gameDisplay();
};
