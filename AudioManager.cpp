#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager() 
    : backgroundMusic(nullptr), bossMusic(nullptr), endGameMusic(nullptr), 
      tapSound(nullptr), buySound(nullptr) {}

AudioManager::~AudioManager() {
    // Giải phóng tài nguyên
    if (backgroundMusic) Mix_FreeMusic(backgroundMusic);
    if (bossMusic) Mix_FreeMusic(bossMusic);
    if (endGameMusic) Mix_FreeMusic(endGameMusic);
    if (tapSound) Mix_FreeChunk(tapSound);
    if (buySound) Mix_FreeChunk(buySound);
    Mix_CloseAudio();
}

bool AudioManager::init() {
    // Khởi tạo SDL_mixer với tần số 44100 Hz, định dạng mặc định, 2 kênh (stereo), và buffer 2048
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void AudioManager::loadBackgroundMusic(const std::string& path) {
    backgroundMusic = Mix_LoadMUS(path.c_str());
    if (!backgroundMusic) {
        std::cerr << "Failed to load background music: " << Mix_GetError() << std::endl;
    }
}

void AudioManager::loadBossMusic(const std::string& path) {
    bossMusic = Mix_LoadMUS(path.c_str());
    if (!bossMusic) {
        std::cerr << "Failed to load boss music: " << Mix_GetError() << std::endl;
    }
}

void AudioManager::loadEndGameMusic(const std::string& path) {
    endGameMusic = Mix_LoadMUS(path.c_str());
    if (!endGameMusic) {
        std::cerr << "Failed to load end game music: " << Mix_GetError() << std::endl;
    }
}

void AudioManager::playBackgroundMusic(int loops) {
    if (backgroundMusic) {
        Mix_PlayMusic(backgroundMusic, loops); // loops = -1 để lặp vô hạn, 0 để phát 1 lần
    }
}

void AudioManager::playBossMusic(int loops) {
    if (bossMusic) {
        Mix_PlayMusic(bossMusic, loops);
    }
}

void AudioManager::playEndGameMusic(int loops) {
    if (endGameMusic) {
        Mix_PlayMusic(endGameMusic, loops);
    }
}

void AudioManager::stopMusic() {
    Mix_HaltMusic(); // Dừng nhạc đang phát
}

void AudioManager::loadSoundEffect(const std::string& name, const std::string& path) {
    if (name == "tap") {
        tapSound = Mix_LoadWAV(path.c_str());
        if (!tapSound) {
            std::cerr << "Failed to load tap sound: " << Mix_GetError() << std::endl;
        }
    } else if (name == "buy") {
        buySound = Mix_LoadWAV(path.c_str());
        if (!buySound) {
            std::cerr << "Failed to load buy sound: " << Mix_GetError() << std::endl;
        }
    }
}

void AudioManager::playSoundEffect(const std::string& name) {
    if (name == "tap" && tapSound) {
        Mix_PlayChannel(-1, tapSound, 0); // -1 để chọn kênh tự động, 0 để không lặp
    } else if (name == "buy" && buySound) {
        Mix_PlayChannel(-1, buySound, 0);
    }
}