#include "TappingSystem.h"
#include "Game.h"

TappingSystem::TappingSystem(Player* player, BossBattle* bossBattle) 
    : game(nullptr), player(player), bossBattle(bossBattle), spacePressed(false) {
}

void TappingSystem::setGame(Game* g) {
    game = g;
}

void TappingSystem::update(float deltaTime) {
    // Hiện tại không cần xử lý gì, nhưng giữ lại để mở rộng sau này
}

void TappingSystem::handleTapInput(SDL_Event& event) {
    // Xử lý nhấn chuột trái
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            player->incrementTapCount(); // Tăng tapCount trong Player
            player->addActivitive(1.0f);
            if (game) {
                game->playTapSound();
            }
            if (bossBattle->isActive()) {
                bossBattle->addTap();
            }
    }

    // Xử lý nhấn và thả phím cách (Spacebar)
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !spacePressed) {
        spacePressed = true;
        player->incrementTapCount(); // Tăng tapCount trong Player
        player->addActivitive(1.0f);
        if (game) {
            game->playTapSound();
        }
        if (bossBattle->isActive()) {
            bossBattle->addTap();
        }
    }
    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
        spacePressed = false; // Đặt lại trạng thái khi thả phím
    }
}