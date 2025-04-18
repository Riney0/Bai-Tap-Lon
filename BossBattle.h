// #ifndef BOSS_BATTLE_H
// #define BOSS_BATTLE_H

// #include "Player.h"

// class BossBattle {
// private:
//     Player* player;        // Con trỏ tới người chơi
//     bool active;           // Trạng thái bossfight (đang diễn ra hay không)
//     bool won;              // Trạng thái thắng
//     bool lost;             // Trạng thái thua
//     float timer;           // Thời gian còn lại của bossfight (60 giây)
//     int requiredTaps;      // Số lần tap cần thiết (727)
//     int currentTaps;       // Số lần tap hiện tại

// public:
//     BossBattle(Player* player);
    
//     void start();          // Bắt đầu bossfight
//     void setBossStats(float health, float damage); // Giữ lại để tương thích với Game.cpp, nhưng không dùng
//     void update(float deltaTime); // Cập nhật trạng thái bossfight
//     void addTap();         // Tăng số lần tap khi người chơi gõ
//     int getCurrentTaps() const; //lấy số lần tap hiện tại
//     void reset();          // Reset bossfight để đánh lại
//     bool isActive() const; // Kiểm tra bossfight có đang diễn ra không
//     bool isWon() const;    // Kiểm tra người chơi có thắng không
//     bool isLost() const;   // Kiểm tra người chơi có thua không
// };

// #endif

#ifndef BOSS_BATTLE_H
#define BOSS_BATTLE_H

#include "Player.h"

class BossBattle {
private:
    Player* player;        // Con trỏ tới người chơi
    bool active;           // Trạng thái bossfight (đang diễn ra hay không)
    bool won;              // Trạng thái thắng
    bool lost;             // Trạng thái thua
    float timer;           // Thời gian còn lại của bossfight (60 giây)
    int requiredTaps;      // Số lần tap cần thiết (727)
    int currentTaps;       // Số lần tap hiện tại

public:
    BossBattle(Player* player);
    
    void start();          // Bắt đầu bossfight
    void setBossStats(float health, float damage); // Giữ lại để tương thích với Game.cpp, nhưng không dùng
    void update(float deltaTime); // Cập nhật trạng thái bossfight
    void addTap();         // Tăng số lần tap khi người chơi gõ
    int getCurrentTaps() const; // Lấy số lần tap hiện tại
    int getRequiredTaps() const; // Thêm: Lấy số lần tap yêu cầu
    float getTimer() const;     // Thêm: Lấy thời gian còn lại
    void reset();          // Reset bossfight để đánh lại
    bool isActive() const; // Kiểm tra bossfight có đang diễn ra không
    bool isWon() const;    // Kiểm tra người chơi có thắng không
    bool isLost() const;   // Kiểm tra người chơi có thua không
};

#endif