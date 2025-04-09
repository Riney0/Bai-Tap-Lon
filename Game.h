#ifndef GAME_H
#define GAME_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "Background.h"
#include "Player.h"
#include "TappingSystem.h"
#include "UI.h"
#include "AudioManager.h"
#include "Challenge.h"
#include "BossBattle.h"
#include "Item.h"
#include <bits/stdc++.h>

class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();
    void playTapSound(); // Phát âm thanh gõ mõ
    void playBuySound(); // Phát âm thanh mua vật phẩm
    UI* getUI(); // Truy cập UI
    Challenge* getChallenge(); // Truy cập thử thách
    std::vector<Item>& getItems(); // Truy cập danh sách vật phẩm
    bool isGameComplete() const { return gameComplete; };

private:
    void update(float deltaTime);
    void render();
    void reset();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    SDL_Event event;
    Background background;
    Player player;
    TappingSystem tappingSystem;
    UI ui;
    AudioManager audioManager; // Quản lý âm thanh
    Challenge challenge; // Thử thách ngẫu nhiên
    float challengeTimer; // Bộ đếm thời gian để kích hoạt thử thách
    BossBattle bossBattle; // Trận đánh boss
    std::vector<Item> items; // Danh sách vật phẩm
    bool isBossMusicPlaying;
    SDL_Texture* gameCompleteTexture; // Ảnh "Game Complete"
    float gameCompleteTimer; // Thời gian hiển thị ảnh
    bool gameComplete; //trạng thái hoàn thành game
};

#endif