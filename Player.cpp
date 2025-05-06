#include "Player.h"
#include "Constants.h"
#include "Game.h"
#include <cstdlib>
#include <iostream> // Debug

Player::Player(Game* game) 
    : game(game), activitive(0), radient(0), level(0), tapCount(0), moMultiplier(1.0f), 
      activitiveMultiplier(1.5f), radientMultiplier(1.0f), previousLevel(0) {}

void Player::tapMo() {
    float activitiveGained = BASE_ACTIVITIVE_PER_TAP * moMultiplier * activitiveMultiplier;
    float radientGained = (rand() % (5 - 2 + 1) + 2) * radientMultiplier;
    
    // Áp dụng hiệu ứng vật phẩm

    for (const auto& item : game->getItems()) {
        if (item.isActive()) {
            if (item.getName() == "X2 Radient") {
                radientGained *= item.getMultiplier(); // Tăng radient theo vật phẩm
            } else if (item.getName() == "X2 Activitive") {
                activitiveGained *= item.getMultiplier(); // Tăng activitive theo vật phẩm
            } else if (item.getName() == "X5 Activitive") {
                activitiveGained *= item.getMultiplier(); // Tăng activitive theo vật phẩm
            }
        }
    }

    activitive += activitiveGained;
    radient += radientGained;
    tapCount++;
    
    Challenge* challenge = game->getChallenge();
    if (challenge && challenge->isActive()) {
        challenge->addTap();
    }
    
    checkLevelUp();
    game->playTapSound();
}

void Player::checkLevelUp() {
    int newLevel = 0;
    if (activitive >= 5000) newLevel = 5;
    else if (activitive >= 4000) newLevel = 4;
    else if (activitive >= 3000) newLevel = 3;
    else if (activitive >= 2000) newLevel = 2;
    else if (activitive >= 1000) newLevel = 1;

    if (newLevel > previousLevel) {
        level = newLevel;
        activitiveMultiplier += 1.0f * (level - previousLevel);
        radientMultiplier += 1.0f * (level - previousLevel);
        previousLevel = newLevel;
        game->getUI()->showLevelUpMessage(level);
    }
}


void Player::addRadient(float amount) {
    radient += amount;
}

void Player::addActivitive(float amount) {
    activitive += amount * activitiveMultiplier;
    checkLevelUp();
}

float Player::getActivitive() const { return activitive; }
float Player::getRadient() const { return radient; }
int Player::getLevel() const { return level; }
int Player::getTapCount() const { return tapCount; }

void Player::incrementTapCount() {
    tapCount++; // Tăng số lần tap
}

void Player::update(float deltaTime) {
    // Cập nhật trạng thái của người chơi (nếu cần)
    activitive += deltaTime * activitiveMultiplier; // Tăng activitive theo thời gian

    checkLevelUp(); // Kiểm tra xem có lên level không
}