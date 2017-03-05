//
//  TimersManager.hpp
//  Chip8Emulator
//

#ifndef TimersManager_hpp
#define TimersManager_hpp

#include <iostream>

class TimersManager
{
	unsigned char delay_timer;
	unsigned char sound_timer;
public:
    TimersManager();
	void update();
	
	unsigned char getSoundTimer();
	void setSoundTimer(unsigned char v);

	unsigned char getDelayTimer();
	void setDelayTimer(unsigned char v);
};

#endif /* TimersManager_hpp */
