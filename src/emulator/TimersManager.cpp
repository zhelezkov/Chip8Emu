//
//  Timers.cpp
//  Chip8Emulator
//

#include "TimersManager.hpp"

TimersManager::TimersManager() {
}

void TimersManager::reset() {
    setDelayTimer(0);
    setSoundTimer(0);
}

void TimersManager::update()
{
	if (delayTimer > 0)
		delayTimer--;

	if (soundTimer > 0)
	{
        if (soundTimer == 1) {
            //PLAY BEEP SOUND   
        }
		soundTimer--;
	}
}

void TimersManager::setDelayTimer(unsigned char v)
{
	delayTimer = v;
}

unsigned char TimersManager::getDelayTimer()
{
	return delayTimer;
}

void TimersManager::setSoundTimer(unsigned char v)
{
	soundTimer = v;
}

unsigned char TimersManager::getSoundTimer()
{
	return soundTimer;
}
