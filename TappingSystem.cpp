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
    std::cout << "TappingSystem::setUI called, ui = " << ui << std::endl; // debug
}

void TappingSystem::update(float deltaTime) {
    // Hiện tại không cần xử lý gì, nhưng giữ lại để mở rộng sau này
}

void TappingSystem::handleTapInput(SDL_Event& event) {
    // Xử lý nhấn chuột trái
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        player->incrementTapCount(); // Tăng tapCount trong Player
        float radientGained = (rand() % (10 - 3 + 1) + 3);
        float activitiveGained = (1.0f);

        if (game){
            for (const auto& item : game->getItems()) {
                if (item.isActive()) {
                    if (item.getName() == "X2 Radient"){
                        radientGained *= item.getMultiplier(); // Tăng radient theo vật phẩm
                    } else if (item.getName() == "X2 Activitive"){
                        activitiveGained *= item.getMultiplier(); // Tăng activitive theo vật phẩm
                    } else if (item.getName() == "X5 Activitive"){
                        activitiveGained *= item.getMultiplier(); // Tăng activitive theo vật phẩm
                    }
                }
            }
        }

        player->addActivitive(activitiveGained); // Tăng activitive mỗi lần tap
        player->addRadient(radientGained); // Tăng radient mỗi lần tap
        if (game) {
            game->playTapSound();
            }
        if (bossBattle->isActive()) {
            bossBattle->addTap();
        }
        if (challenge->isActive()) {
            challenge->addTap();
        }
        if(ui) {
            // std::cout << "Mouse tap detected, calling triggerMemeEffect, ui=" << ui << std::endl; // debug
            ui->triggerMemeEffect(); // Hiện meme mỗi khi tap
            ui->triggerSupaMoEffect(); // Hiện cái mõ siêu vip mỗi khi tap trong bossfight
        }
    }

    // Xử lý nhấn và thả phím cách (Spacebar)
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !spacePressed) {
        spacePressed = true;
        player->incrementTapCount(); // Tăng tapCount trong Player
        float activitiveGained = (1.0f);
        float radientGained = (rand() % (10 - 3 + 1) + 3);

        if (game){
            for (const auto& item : game->getItems()) {
                if (item.isActive()) {
                    if (item.getName() == "X2 Radient"){
                        radientGained *= item.getMultiplier(); // Tăng radient theo vật phẩm
                    } else if (item.getName() == "X2 Activitive"){
                        activitiveGained *= item.getMultiplier(); // Tăng activitive theo vật phẩm
                    } else if (item.getName() == "X5 Activitive"){
                        activitiveGained *= item.getMultiplier(); // Tăng activitive theo vật phẩm
                    }
                }
            }
        }

        player->addActivitive(activitiveGained);
        player->addRadient(radientGained); // Tăng radient mỗi lần tap
        if (game) {
            game->playTapSound();
        }
        if (bossBattle->isActive()) {
            bossBattle->addTap();
        }
        if (challenge->isActive()) { // Thêm dòng này
            challenge->addTap();
        }
        if(ui) {
            // std::cout << "Space tap detected, calling triggerMemeEffect, ui=" << ui << std::endl; // debug
            ui->triggerMemeEffect(); // Hiện meme mỗi khi tap
            ui->triggerSupaMoEffect(); // Hiện cái mõ siêu vip mỗi khi tap trong bossfight
        }
    }

    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
        spacePressed = false; // Đặt lại trạng thái khi thả phím
    }
}