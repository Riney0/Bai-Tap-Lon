#ifndef ITEM_H
#define ITEM_H

#include <string>


class Item {
private:
    std::string name;      // Tên vật phẩm
    float multiplier;      // Hệ số (ví dụ: nhân đôi activitive, tăng radient)
    float duration;        // Thời gian hiệu lực (giây)
    float remainingTime;   // Thời gian còn lại (giây)
    int cost;              // Giá (radient)
    bool active;           // Trạng thái kích hoạt

public:
    Item(const std::string& name, float multiplier, float duration, int cost);
    
    void activate();       // Kích hoạt vật phẩm
    void update(float deltaTime); // Cập nhật thời gian hiệu lực
    bool isActive() const; // Kiểm tra vật phẩm có đang active không
    std::string getName() const; // Lấy tên vật phẩm
    int getCost() const;   // Lấy giá vật phẩm
    float getMultiplier() const; // Lấy hệ số
    float getDuration() const;   // Lấy thời gian hiệu lực
    float getRemainingTime() const; // Lấy thời gian còn lại
};

#endif