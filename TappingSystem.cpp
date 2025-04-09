#include "TappingSystem.h"
#include "Game.h"

TappingSystem::TappingSystem(Player* player, BossBattle* bossBattle) 
    : game(nullptr), player(player), bossBattle(bossBattle) {}

void TappingSystem::setGame(Game* g) {
    game = g;
}

void TappingSystem::update(float deltaTime) {
    // Hiện tại không cần logic cập nhật, nhưng giữ lại để mở rộng sau này
}

void TappingSystem::handleTapInput(SDL_Event& event) {
    if ((event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) || event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
        
        // Phát âm thanh tap
        game->playTapSound();

        // Tăng activitive cho người chơi
        player->addActivitive(1.0f); // Mỗi lần tap tăng 1 activitive

        // Tăng số lần tap cho Challenge (nếu đang active)
        if (game->getChallenge()->isActive()) {
            game->getChallenge()->addTap();
        }

        // Tăng số lần tap cho BossBattle (nếu đang active)
        if (bossBattle->isActive()) {
            bossBattle->addTap();
        }
    }
}