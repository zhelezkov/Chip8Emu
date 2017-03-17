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

void TimersManager::update() {
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

void TimersManager::setDelayTimer(byte v) {
	delayTimer = v;
}

byte TimersManager::getDelayTimer() {
	return delayTimer;
}

void TimersManager::setSoundTimer(byte v) {
	soundTimer = v;
}

byte TimersManager::getSoundTimer() {
	return soundTimer;
}
