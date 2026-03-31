extern "C" {
	#include "LPC17xx.h"
}

class Gallery{
	private: 
		void nightPhoto() const;
		void onePhoto() const;
		void bleachPhoto() const;
		uint32_t dir;
		int photo_selection;
	 	int last_selection;
	
	public:
		void photoDisplay();
};
