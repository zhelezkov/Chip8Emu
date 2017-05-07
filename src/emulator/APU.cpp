//
//  APU.cpp
//  Chip8Emulator
//

#include "APU.hpp"
#include <SDL.h>

void audioCallback(void* userdata, Uint8* stream, int streamLength) {
    for (int i = 0; i < streamLength; i += 4) {
        stream[i] = 1;
    }
}

APU::APU() {
    SDL_AudioSpec audioSpec;
    audioSpec.freq = 3000;
    audioSpec.format = AUDIO_S8;
    audioSpec.channels = 1;
    audioSpec.samples = 32;
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
    LOG_F(INFO, "BEEP");
}

void APU::pauseSound() {
    SDL_PauseAudio(true);
}
