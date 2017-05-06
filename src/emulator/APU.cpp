//
//  APU.cpp
//  Chip8Emulator
//

#include "APU.hpp"

#include "SDL.h"

float volume = 0;

void sdlAudioCallback(void* userdata, Uint8* stream, int len) {
    
//    for (int i = 0; i < len; i += 4) {
//        stream[i] = 1;
//    }
}

APU::APU() {
//    SDL_AudioSpec spec;
//    spec.freq = 3000;
//    spec.format = AUDIO_F32;
//    spec.channels = 1;
//    spec.samples = 32;
//    spec.callback = sdlAudioCallback;
//    int err = SDL_OpenAudio(&spec, NULL);
//    CHECK_F(err != -1, "Error initing audio");
//    SDL_PauseAudio(false);
}

APU::~APU() {
//    SDL_CloseAudio();
}

void APU::reset() {
    
}

void APU::playSound() {
    LOG_F(INFO, "BEEP");
}
