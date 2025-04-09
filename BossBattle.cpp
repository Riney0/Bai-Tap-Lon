#include "BossBattle.h"

BossBattle::BossBattle(Player* player) 
    : player(player), active(false), won(false), lost(false), 
      timer(0.0f), requiredTaps(727), currentTaps(0) {}

void BossBattle::start() {
    active = true;
    won = false;
    lost = false;
    timer = 60.0f; // 60 giây để hoàn thành
    currentTaps = 0; // Reset số lần tap
}

void BossBattle::setBossStats(float health, float damage) {
    //không sử dụng nhưng dữ lại để tương thích với game.cpp
}

void BossBattle::update(float deltaTime) {
    if (!active) return;

    // Giảm thời gian còn lại
    timer -= deltaTime;

    // Kiểm tra nếu hết thời gian
    if (timer <= 0) {
        if (currentTaps < requiredTaps) {
            active = false;
            won = false;
            lost = true; // Thua nếu không đủ số lần tap
        }
        return;
    }

    // Kiểm tra nếu hoàn thành
    if (currentTaps >= requiredTaps) {
        active = false;
        won = true;
        lost = false; // Thắng nếu đạt đủ số lần tap
    }
}

void BossBattle::addTap() {
    if (active) {
        currentTaps++; // Tăng số lần tap
    }
}

int BossBattle::getCurrentTaps() const {
    return currentTaps;
}

void BossBattle::reset() {
    start(); // Gọi start() để reset bossfight
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