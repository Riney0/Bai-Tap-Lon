#ifndef TAPPING_SYSTEM_H
#define TAPPING_SYSTEM_H

#include "Player.h"
#include "BossBattle.h"
#include <SDL2/SDL.h>

class Game; // Forward declaration
class UI; // Forward declaration

class TappingSystem {
private:
    Game* game;            // Con trỏ tới Game để gọi playTapSound và truy cập Challenge
    Player* player;        // Con trỏ tới Player để tăng activitive
    BossBattle* bossBattle; // Con trỏ tới BossBattle để tăng số lần tap
    bool spacePressed;     // Theo dõi trạng thái phím cách (Spacebar)
    UI* ui; // Con trỏ tới UI để gọi meme

public:
    TappingSystem(Player* player, BossBattle* bossBattle);
    
    void setGame(Game* g); // Thiết lập con trỏ Game
    void update(float deltaTime); // Cập nhật (hiện tại không dùng, nhưng giữ lại để mở rộng)
    void handleTapInput(SDL_Event& event); // Xử lý input tap
    void setUI(UI* ui); // Thiết lập con trỏ UI để gọi meme
};

#endif