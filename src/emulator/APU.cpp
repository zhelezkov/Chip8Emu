//
//  APU.cpp
//  Chip8Emulator
//

#include "APU.hpp"
#include <SDL.h>

float tonePose = 0;

void audioCallback(void* userdata, Uint8* stream, int streamLength) {
    for (int i = 0; i < streamLength; i++) {
        stream[i] = sinf(tonePose) + 127;
        tonePose += 3.14159 * 1000 / 44100;
    }
}

APU::APU() {
    SDL_AudioSpec audioSpec;
    SDL_zero(audioSpec);
    
    audioSpec.freq = 44100;
    audioSpec.format = AUDIO_U8;
    audioSpec.channels = 1;
    audioSpec.samples = 4096;
    audioSpec.callback = audioCallback;

    int err = SDL_OpenAudio(&audioSpec, NULL);
    CHECK_F(err != -1, "Error initing audio");
}

APU::~APU() {
    pauseSound();
    SDL_CloseAudio();
}

void APU::reset() {
    pauseSound();
}

void APU::playSound() {
    SDL_PauseAudio(false);
}

void APU::pauseSound() {
    SDL_PauseAudio(true);
}
