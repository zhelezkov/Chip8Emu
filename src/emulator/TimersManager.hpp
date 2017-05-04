//
//  TimersManager.hpp
//  Chip8Emulator
//

#ifndef TimersManager_hpp
#define TimersManager_hpp

#include "CHIP.h"

class TimersManager {
public:
    TimersManager();
    void reset();
    
	void update();
	
    byte getSoundTimer();
	void setSoundTimer(byte v);

	byte getDelayTimer();
	void setDelayTimer(byte v);
private:
    byte delayTimer;
    byte soundTimer;
};

#endif /* TimersManager_hpp */
