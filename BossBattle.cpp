#include "BossBattle.h"

BossBattle::BossBattle(Player* player) 
    : player(player), active(false), won(false), lost(false), 
      timer(0), requiredTaps(0), currentTaps(0) {}

void BossBattle::start() {
    active = true;
    won = false;
    lost = false;
    timer = 60.0f; // Đặt thời gian mặc định là 60 giây (sẽ được set lại trong setBossStats)
    currentTaps = 0;
}

void BossBattle::setBossStats(float health, float damage) {
    // Theo logic trong Game.cpp, health là thời gian (60 giây), damage không dùng
    timer = health; // Thời gian bossfight (60 giây)
    requiredTaps = 727; // Số lần tap cần thiết (theo Game.cpp)
    currentTaps = 0;
}

void BossBattle::update(float deltaTime) {
    if (!active) return;

    timer -= deltaTime;
    if (timer <= 0) {
        if (currentTaps >= requiredTaps) {
            won = true;
        } else {
            lost = true;
        }
        active = false;
    }
}

void BossBattle::addTap() {
    currentTaps++;
}

int BossBattle::getCurrentTaps() const {
    return currentTaps;
}

int BossBattle::getRequiredTaps() const {
    return requiredTaps;
}

float BossBattle::getTimer() const {
    return timer;
}

void BossBattle::reset() {
    active = false;
    won = false;
    lost = false;
    timer = 0;
    currentTaps = 0;
}

bool BossBattle::isActive() const {
    return active;
}

bool BossBattle::isWon() const {
    return won;
}

bool BossBattle::isLost() const {
    return lost;
}