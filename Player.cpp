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
    float radientGained = (rand() % (10 - 3 + 1) + 3) * radientMultiplier;

    // Debug
    std::cerr << "Radient Multiplier: " << radientMultiplier 
              << ", Radient Gained: " << radientGained 
              << ", Radient Before: " << radient;

    // Áp dụng hiệu ứng vật phẩm
    for (const auto& item : game->getItems()) {
        if (item.isActive()) {
            if (item.getName() == "Double Activitive") {
                activitiveGained *= item.getMultiplier();
            } else if (item.getName() == "Radient Boost") {
                radientGained *= item.getMultiplier();
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
    if (activitive >= 5000) newLevel = 5; // test
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

void Player::convertActivitiveToRadient(float amount) {
    if (activitive >= amount) {
        activitive -= amount;
        radient += amount * ACTIVITIVE_TO_RADIENT_RATE;
    }
}

void Player::upgradeMo(float cost, float activitiveMultiplier) {
    if (radient >= cost) {
        radient -= cost;
        moMultiplier += activitiveMultiplier;
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