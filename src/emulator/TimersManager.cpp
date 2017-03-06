//
//  Timers.cpp
//  Chip8Emulator
//

#include "TimersManager.hpp"

TimersManager::TimersManager() {
    delay_timer = 0;
    sound_timer = 0;
}

void TimersManager::update()
{
	if (delay_timer > 0)
		delay_timer--;

	if (sound_timer > 0)
	{
        if (sound_timer == 1) {
            //PLAY BEEP SOUND   
        }
		sound_timer--;
	}
}

void TimersManager::setDelayTimer(unsigned char v)
{
	delay_timer = v;
}

unsigned char TimersManager::getDelayTimer()
{
	return delay_timer;
}

void TimersManager::setSoundTimer(unsigned char v)
{
	sound_timer = v;
}

unsigned char TimersManager::getSoundTimer()
{
	return sound_timer;
}
