#include "BossBattle.h"

BossBattle::BossBattle(Player* player) 
    : player(player), active(false), won(false), lost(false), 
      timer(0.0f), requiredTaps(727.0f), currentTaps(0) {}

void BossBattle::start() {
    active = true;
    won = false;
    lost = false;
    timer = 60.0f; // Đặt thời gian mặc định là 60 giây (sẽ được set lại trong setBossStats)
    currentTaps = 0;
}

void BossBattle::setBossStats(float health, float damage) {
    requiredTaps = 727; // Số lần tap cần thiết
    currentTaps = 0;
}

void BossBattle::update(float deltaTime) {
    if (!active) return;

    timer -= deltaTime;
    if (timer <= 0) {
        if (currentTaps < requiredTaps) {
            active = false;
            won = false;
            lost = true;
        } else {
            active = false;
            won = true;
            lost = false;  //note
        }
    }
}

void BossBattle::addTap() {
    if (active) {
        currentTaps++;
    }
    // Nếu không active, không tăng currentTaps
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
    // Reset trạng thái bossfight để có thể đánh lại
    start();
    currentTaps = 0;
    timer = 60.0f; // Đặt lại thời gian mặc định là 60 giây
    active = false; // Đặt lại trạng thái không hoạt động
    won = false;   // Đặt lại trạng thái thắng
    lost = false;  // Đặt lại trạng thái thua
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