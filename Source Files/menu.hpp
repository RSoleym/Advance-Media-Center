extern "C" {
	#include "LPC17xx.h"
}

class Menu{
	private: 
		int displayFunc();
		uint32_t dir;
		int menu_selection;
    int last_selection;
		int class_selection;
	
	public:
		void menuDisplay();
};
