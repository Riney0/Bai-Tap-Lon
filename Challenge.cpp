#include "Challenge.h"
#include <cstdlib>

Challenge::Challenge() 
    : active(false), completed(false), duration(0.0f), timer(0.0f), 
      requiredTaps(0), currentTaps(0), activitiveReward(100.0f), radientReward(50.0f) {}

void Challenge::start() {
    active = true;
    completed = false;

    // Random thời gian thử thách từ 10 đến 30 giây
    duration = 10.0f + (rand() % 21); // rand() % 21 cho giá trị từ 0 đến 20, cộng 10 để được 10-30
    timer = duration;

    // Random số lần tap/phút từ 60 đến 150
    int tapsPerMinute = 60 + (rand() % 151); // rand() % 61 cho giá trị từ 0 đến 60, cộng 40 để được 40-100

    // Tính requiredTaps dựa trên thời gian và tapsPerMinute
    // tapsPerMinute là số lần tap trong 60 giây, nên số lần tap trong duration giây là:
    // requiredTaps = tapsPerMinute * (duration / 60)
    requiredTaps = static_cast<int>(tapsPerMinute * (duration / 60.0f));
    currentTaps = 0; // Reset số lần tap
}

void Challenge::update(float deltaTime) {
    if (!active) return;

    // Giảm thời gian còn lại
    timer -= deltaTime;

    // Kiểm tra nếu hết thời gian
    if (timer <= 0) {
        active = false; // Thử thách kết thúc (thất bại)
        completed = false;
        return;
    }

    // Kiểm tra nếu hoàn thành thử thách
    if (currentTaps >= requiredTaps) {
        active = false; // Thử thách kết thúc
        completed = true; // Đánh dấu đã hoàn thành
    }
}

void Challenge::addTap() {
    if (active) {
        currentTaps++; // Tăng số lần tap
    }
}

void Challenge::complete() {
    //active = false; // Kết thúc thử thách
    completed = false; // Đánh dấu đã hoàn thành
}

int Challenge::getRequiredTaps() const {
    return requiredTaps;
}

int Challenge::getCurrentTaps() const {
    return currentTaps;
}

bool Challenge::isActive() const {
    return active;
}

bool Challenge::isCompleted() const {
    return completed;
}

float Challenge::getActivitiveReward() const {
    return activitiveReward;
}

float Challenge::getRadientReward() const {
    return radientReward;
}

float Challenge::getTimer() const {
    return timer;
}