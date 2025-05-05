#include "TappingSystem.h"
#include "Game.h"
#include "UI.h"

TappingSystem::TappingSystem(Player* player, BossBattle* bossBattle, Challenge* challenge) 
    : game(nullptr), player(player), challenge(challenge), bossBattle(bossBattle), spacePressed(false) {
}

void TappingSystem::setGame(Game* g) {
    game = g;
}

void TappingSystem::setUI(UI* ui) {
    this->ui = ui;
    std::cout << "TappingSystem::setUI called, ui = " << ui << std::endl; // Debug
}

void TappingSystem::update(float deltaTime) {
    // Hiện tại chưa dùng nhưng sẽ mở rộng nếu cần
}

void TappingSystem::handleTapInput(SDL_Event& event) {
    // Xử lý nhấn chuột trái
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        player->tapMo();
        if (bossBattle->isActive()) {
            bossBattle->addTap();
        }
        if (ui) {
            ui->triggerMemeEffect();
            ui->triggerSupaMoEffect();
        }
    }

    // Xử lý nhấn và thả phím cách (Spacebar)
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !spacePressed) {
        spacePressed = true;
        player->tapMo();
        if (bossBattle->isActive()) {
            bossBattle->addTap();
        }
        if (ui) {
            ui->triggerMemeEffect();
            ui->triggerSupaMoEffect();
        }
    }

    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
        spacePressed = false;
    }
}