//
//  TimersManager.hpp
//  Chip8Emulator
//

#ifndef TimersManager_hpp
#define TimersManager_hpp

#include "CHIP.h"

class TimersManager
{
	byte delay_timer;
	byte sound_timer;
public:
    TimersManager();
	void update();
	
    byte getSoundTimer();
	void setSoundTimer(byte v);

	byte getDelayTimer();
	void setDelayTimer(byte v);
};

#endif /* TimersManager_hpp */
