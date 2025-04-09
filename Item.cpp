#include "Item.h"

Item::Item(const std::string& name, float multiplier, float duration, int cost)
    : name(name), multiplier(multiplier), duration(duration), remainingTime(0.0f), cost(cost), active(false) {}

void Item::activate() {
    active = true;
    remainingTime = duration; // Đặt thời gian còn lại bằng thời gian hiệu lực
}

void Item::update(float deltaTime) {
    if (active) {
        remainingTime -= deltaTime; // Giảm thời gian còn lại
        if (remainingTime <= 0) {
            active = false; // Hủy kích hoạt khi hết thời gian
            remainingTime = 0;
        }
    }
}

bool Item::isActive() const {
    return active;
}

std::string Item::getName() const {
    return name;
}

int Item::getCost() const {
    return cost;
}

float Item::getMultiplier() const {
    return multiplier;
}

float Item::getDuration() const {
    return duration;
}

float Item::getRemainingTime() const {
    return remainingTime;
}