//
//  APU.cpp
//  Chip8Emulator
//

#include "APU.hpp"
#include <ctime>

struct Float_Number {
    Float_Number(float val) {
        rawCode = val;
    }

    union {
        struct {
            unsigned char n1;
            unsigned char n2;
            unsigned char n3;
            unsigned char n4;
        };
        float rawCode;
    };
};

Float_Number BeepVolume = 1.0;

void Callback(void* userdata, Uint8* stream, int streamLength)
{
    for (int i = 0; i < streamLength; i += 4)
    {
        stream[i] = BeepVolume.n1;
        stream[i + 1] = BeepVolume.n2;
        stream[i + 2] = BeepVolume.n3;
        stream[i + 3] = BeepVolume.n4;
    }
}

APU::APU() {
    SDL_Init(SDL_INIT_AUDIO);
    
    SDL_memset(&wavSpec, 0, sizeof(wavSpec));
    wavSpec.freq = 48000;
    wavSpec.format = AUDIO_F32;
    wavSpec.channels = 2; // stereo
    wavSpec.samples = 32; // 128 bytes = 32 * sizeof(float)
    wavSpec.callback = Callback;
    wavSpec.userdata = NULL;

    int err = SDL_OpenAudio(&wavSpec, NULL);
    CHECK_F(err != -1, "Error initing audio");
}

APU::~APU() {
      SDL_CloseAudio();
}

void APU::reset() {
    SDL_PauseAudio(true);
}

void APU::playSound() {
    SDL_PauseAudio(false);
    LOG_F(INFO, "BEEP");
}
