#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "Player.h"
#include "TappingSystem.h"
#include "BossBattle.h"
#include "Challenge.h"
#include "Item.h"

class Game; // Forward declaration

class UI {
private:
    Game* game; // Con trỏ tới Game để truy cập các thành phần khác
    SDL_Renderer* renderer;
    TTF_Font* titleFont; // Font cho tiêu đề "CHILLING LEVELING"
    TTF_Font* textFont;  // Font cho các chữ khác (Raleway_Regular)
    SDL_Texture* moTexture; // Hình ảnh cái mõ thường
    SDL_Texture* moVipTexture; // Hình ảnh cái mõ VIP (trong bossfight)
    SDL_Texture* moSupaVipTexture; // Hình ảnh cái mõ Supa VIP (trong bossfight)
    SDL_Texture* itemTextures[3]; // Hình ảnh của 3 vật phẩm (Double Activitive, Radient Boost, Radient Hyper Boost)
    SDL_Texture* memeTexture; // Hình ảnh meme
    SDL_Color whiteColor; // Màu trắng cho chữ
    SDL_Color blackColor; // Màu đen cho chữ
    SDL_Color blueColor;  // Màu xanh lam cho hộp activitive
    SDL_Color cyanColor;  // Màu xanh lam nhạt cho hộp radient
    SDL_Color orangeColor; // Màu cam cho chữ
    std::string bossMessage; // Thông báo bossfight
    float messageTimer; // Thời gian hiển thị thông báo
    static constexpr float MEME_FADE_DURATION = 0.25f; // Thời gian mờ dần của meme
    float memeFadeTimer; // Thời gian còn lại để hiển thị meme
    static constexpr float SUPA_MO_APPEAR_TIME = 0.1f; // Thời gian hiển thị của cái mõ siêu vjp trước khi biến mất
    float moSupaVipTimer; // Thời gian còn lại để hiển thị cái mõ Supa VIP (trong bossfight)

public:
    UI(SDL_Renderer* renderer, Game* game);
    ~UI();

    void loadResources(); // Tải font và hình ảnh
    void showBossMessage(const std::string& message); // Hiển thị thông báo bossfight
    void showLevelUpMessage(int level); // Hiển thị thông báo lên level (hiện tại không dùng, nhưng giữ lại)
    void handleInput(SDL_Event& event, Player& player, std::vector<Item>& items, BossBattle& bossBattle); // Xử lý input (mua vật phẩm)
    void render(Player& player, TappingSystem& tappingSystem, std::vector<Item>& items, BossBattle& bossBattle, Challenge& challenge, float deltaTime); // Vẽ giao diện
    void triggerMemeEffect(); // Kích hoạt hiệu ứng meme
    void triggerSupaMoEffect(); // Kích hoạt hiệu ứng cái mõ Supa VIP
};

#endif