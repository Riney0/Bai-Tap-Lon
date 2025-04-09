#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SDL2/SDL_mixer.h>
#include <string>


class AudioManager {
private:
    Mix_Music* backgroundMusic; // Nhạc nền
    Mix_Music* bossMusic;       // Nhạc boss
    Mix_Music* endGameMusic;    // Nhạc kết thúc game
    Mix_Chunk* tapSound;        // Hiệu ứng âm thanh khi tap
    Mix_Chunk* buySound;        // Hiệu ứng âm thanh khi mua

public:
    AudioManager();
    ~AudioManager();

    bool init(); // Khởi tạo SDL_mixer

    // Tải các loại nhạc
    void loadBackgroundMusic(const std::string& path);
    void loadBossMusic(const std::string& path);
    void loadEndGameMusic(const std::string& path);

    // Phát các loại nhạc
    void playBackgroundMusic(int loops);
    void playBossMusic(int loops);
    void playEndGameMusic(int loops);

    // Dừng nhạc
    void stopMusic();

    // Tải và phát hiệu ứng âm thanh
    void loadSoundEffect(const std::string& name, const std::string& path);
    void playSoundEffect(const std::string& name);
};

#endif