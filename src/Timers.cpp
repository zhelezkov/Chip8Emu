//
//  Timers.cpp
//  Chip8Emulator
//
//  
//
//

#include "Timers.hpp"

void Timers::init()
{
	delay_timer = 0;
	sound_timer = 0;
}

void Timers::update()
{
	if (delay_timer > 0)
		delay_timer--;

	if (sound_timer > 0)
	{
		if (sound_timer == 1) std::cout << "BEEP!" << std::endl;
		sound_timer--;
	}
}

void Timers::setDelayTimer(double v)
{
	delay_timer = v;
}

double Timers::getDelayTimer()
{
	return delay_timer;
}

void Timers::setSoundTimer(double v)
{
	sound_timer = v;
}

double Timers::getSoundTimer()
{
	return sound_timer;
}