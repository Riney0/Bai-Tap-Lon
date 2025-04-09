#ifndef CHALLENGE_H
#define CHALLENGE_H

class Challenge {
private:
    bool active;           // Trạng thái thử thách (đang diễn ra hay không)
    bool completed;        // Trạng thái hoàn thành
    float duration;        // Thời gian tồn tại của thử thách (giây)
    float timer;           // Thời gian còn lại của thử thách
    int requiredTaps;      // Số lần tap cần để hoàn thành thử thách
    int currentTaps;       // Số lần tap hiện tại
    float activitiveReward; // Phần thưởng activitive khi hoàn thành
    float radientReward;    // Phần thưởng radient khi hoàn thành

public:
    Challenge();
    
    void start();          // Bắt đầu thử thách
    void update(float deltaTime); // Cập nhật trạng thái thử thách
    void addTap();         // Tăng số lần tap khi người chơi gõ
    int getCurrentTaps() const; //Lấy số lần tap hiện tại
    bool isActive() const; // Kiểm tra thử thách có đang diễn ra không
    bool isCompleted() const; // Kiểm tra thử thách đã hoàn thành chưa
    float getActivitiveReward() const; // Lấy phần thưởng activitive
    float getRadientReward() const;    // Lấy phần thưởng radient
};

#endif