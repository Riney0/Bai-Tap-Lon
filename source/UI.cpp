#include "UI.h"
#include "Game.h"
#include "Constants.h"
#include <iostream>


UI::UI(SDL_Renderer* renderer, Game* game) 
    : game(game), renderer(renderer), titleFont(nullptr), textFont(nullptr), 
      moTexture(nullptr), moVipTexture(nullptr), bossMessage(""), messageTimer(0.0f) {
    for (int i = 0; i < 3; i++) {
        itemTextures[i] = nullptr;
    }
    whiteColor = {255, 255, 255, 255}; // Màu trắng
    blueColor = {0, 128, 255, 255};    // Màu xanh lam
    cyanColor = {0, 255, 255, 255};    // Màu xanh lam nhạt
    loadResources(); // Tải font và hình ảnh
}

UI::~UI() {
    if (titleFont) TTF_CloseFont(titleFont);
    if (textFont) TTF_CloseFont(textFont);
    if (moTexture) SDL_DestroyTexture(moTexture);
    if (moVipTexture) SDL_DestroyTexture(moVipTexture);
    for (int i = 0; i < 3; i++) {
        if (itemTextures[i]) SDL_DestroyTexture(itemTextures[i]);
    }
    TTF_Quit();
}

void UI::loadResources() {
    if (!renderer) {
        std::cerr << "Renderer is null! Cannot load resources." << std::endl;
        return;
    }

    // Khởi tạo SDL_ttf
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Tải font
    titleFont = TTF_OpenFont("asset/Antique_Quest_St.ttf", 48);
    if (!titleFont) {
        std::cerr << "Failed to load title font! TTF_Error: " << TTF_GetError() << std::endl;
    }

    textFont = TTF_OpenFont("asset/Raleway_Regular.ttf", 24);
    if (!textFont) {
        std::cerr << "Failed to load text font! TTF_Error: " << TTF_GetError() << std::endl;
    }

    // Tải hình ảnh
    moTexture = IMG_LoadTexture(renderer, "asset/mo.png");
    if (!moTexture) {
        std::cerr << "Failed to load mo texture: " << IMG_GetError() << std::endl;
    }

    moVipTexture = IMG_LoadTexture(renderer, "asset/mo_vjp.png");
    if (!moVipTexture) {
        std::cerr << "Failed to load mo_vjp texture: " << IMG_GetError() << std::endl;
    }

    itemTextures[0] = IMG_LoadTexture(renderer, "asset/x2_tap_item.png");
    itemTextures[1] = IMG_LoadTexture(renderer, "asset/2k_plus_item.png");
    itemTextures[2] = IMG_LoadTexture(renderer, "asset/5k_plus_item.png");
    for (int i = 0; i < 3; i++) {
        if (!itemTextures[i]) {
            std::cerr << "Failed to load item texture " << i << ": " << IMG_GetError() << std::endl;
        }
    }
}

void UI::showBossMessage(const std::string& message) {
    bossMessage = message;
    messageTimer = 5.0f; // Hiển thị thông báo trong 5 giây
}

void UI::showLevelUpMessage(int level) {
    // Hiện tại không dùng, nhưng giữ lại để mở rộng
}

void UI::handleInput(SDL_Event& event, Player& player, std::vector<Item>& items, BossBattle& bossBattle) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int x = event.button.x;
        int y = event.button.y;

        // Kiểm tra nếu người chơi click vào ô vật phẩm
        for (int i = 0; i < 3; i++) {
            SDL_Rect itemRect = {SCREEN_WIDTH - 200, 200 + i * 100, 180, 80}; // Vị trí ô vật phẩm
            if (x >= itemRect.x && x <= itemRect.x + itemRect.w &&
                y >= itemRect.y && y <= itemRect.y + itemRect.h) {
                if (player.getRadient() >= items[i].getCost()) {
                    player.addRadient(-items[i].getCost()); // Trừ radient
                    items[i].activate(); // Kích hoạt vật phẩm
                    game->playBuySound(); // Phát âm thanh mua
                }
            }
        }
    }
}

void UI::render(Player& player, TappingSystem& tappingSystem, std::vector<Item>& items, BossBattle& bossBattle, Challenge& challenge) {
    // Nếu game đã hoàn thành, không vẽ UI
    if (game->isGameComplete()) {
        return;
    }

    // 1. Vẽ tiêu đề "CHILLING LEVELING"
    SDL_Surface* titleSurface1 = TTF_RenderText_Blended(titleFont, "CHILLING", blackColor);
    SDL_Surface* titleSurface2 = TTF_RenderText_Blended(titleFont, "       LEVELING", blackColor); // 7 dấu cách
    if (titleSurface1 && titleSurface2) {
        SDL_Texture* titleTexture1 = SDL_CreateTextureFromSurface(renderer, titleSurface1);
        SDL_Texture* titleTexture2 = SDL_CreateTextureFromSurface(renderer, titleSurface2);
        SDL_Rect titleRect1 = {10, 10, titleSurface1->w, titleSurface1->h};
        SDL_Rect titleRect2 = {10, 10 + titleSurface1->h, titleSurface2->w, titleSurface2->h};
        SDL_RenderCopy(renderer, titleTexture1, nullptr, &titleRect1);
        SDL_RenderCopy(renderer, titleTexture2, nullptr, &titleRect2);
        SDL_DestroyTexture(titleTexture1);
        SDL_DestroyTexture(titleTexture2);
        SDL_FreeSurface(titleSurface1);
        SDL_FreeSurface(titleSurface2);
    }

    // 2. Vẽ hình ảnh cái mõ
    SDL_Texture* currentMoTexture = bossBattle.isActive() ? moVipTexture : moTexture;
    if (currentMoTexture) {
        SDL_Rect moRect = {545, 250, 250, 250}; // Kích thước giả định 200x200
        SDL_RenderCopy(renderer, currentMoTexture, nullptr, &moRect);
    }

    // 3. Vẽ số lần tap và level bên dưới cái mõ
    std::string tapText = "Taps: " + std::to_string(static_cast<int>(player.getActivitive()));
    std::string levelText = "Level: " + std::to_string(player.getLevel());
    SDL_Surface* tapSurface = TTF_RenderText_Blended(textFont, tapText.c_str(), blackColor);
    SDL_Surface* levelSurface = TTF_RenderText_Blended(textFont, levelText.c_str(), blackColor);
    if (tapSurface && levelSurface) {
        SDL_Texture* tapTexture = SDL_CreateTextureFromSurface(renderer, tapSurface);
        SDL_Texture* levelTexture = SDL_CreateTextureFromSurface(renderer, levelSurface);
        SDL_Rect tapRect = {545, 170, tapSurface->w, tapSurface->h};
        SDL_Rect levelRect = {545, 200, levelSurface->w, levelSurface->h};
        SDL_RenderCopy(renderer, tapTexture, nullptr, &tapRect);
        SDL_RenderCopy(renderer, levelTexture, nullptr, &levelRect);
        SDL_DestroyTexture(tapTexture);
        SDL_DestroyTexture(levelTexture);
        SDL_FreeSurface(tapSurface);
        SDL_FreeSurface(levelSurface);
    }

    // 4. Vẽ hộp trắng bên phải và 3 ô vật phẩm
    SDL_Rect whiteBox = {SCREEN_WIDTH - 200, 150, 180, 350}; // Hộp trắng
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &whiteBox);

    for (int i = 0; i < 3; i++) {
        SDL_Rect itemRect = {SCREEN_WIDTH - 190, 200 + i * 100, 160, 80};
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Màu xám nhạt cho ô vật phẩm
        SDL_RenderFillRect(renderer, &itemRect);

        // Vẽ tên vật phẩm và giá
        std::string itemText = items[i].getName() + " (" + std::to_string(items[i].getCost()) + " Radient)";
        SDL_Surface* itemSurface = TTF_RenderText_Blended(textFont, itemText.c_str(), whiteColor);
        if (itemSurface) {
            SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(renderer, itemSurface);
            SDL_Rect itemTextRect = {SCREEN_WIDTH - 180, 210 + i * 100, itemSurface->w, itemSurface->h};
            SDL_RenderCopy(renderer, itemTexture, nullptr, &itemTextRect);
            SDL_DestroyTexture(itemTexture);
            SDL_FreeSurface(itemSurface);
        }
    }

    // 5. Vẽ hình ảnh vật phẩm đã mua (bên trái, mờ dần)
    for (int i = 0; i < 3; i++) {
        if (items[i].isActive() && itemTextures[i]) {
            float alpha = (items[i].getRemainingTime() / items[i].getDuration()) * 255; // Tính độ mờ
            SDL_SetTextureAlphaMod(itemTextures[i], static_cast<Uint8>(alpha));
            SDL_Rect itemEffectRect = {SCREEN_WIDTH / 2 - 250, 200 + i * 100, 200, 200}; // Kích thước giả định 100x100
            SDL_RenderCopy(renderer, itemTextures[i], nullptr, &itemEffectRect);
        }
    }

    // 6. Vẽ hộp activitive và radient bên trên hộp trắng
    SDL_Rect activitiveBox = {SCREEN_WIDTH - 200, 80, 180, 40};
    SDL_Rect radientBox = {SCREEN_WIDTH - 200, 110, 180, 40};
    SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255); // Màu xanh lam
    SDL_RenderFillRect(renderer, &activitiveBox);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Màu xanh lam nhạt
    SDL_RenderFillRect(renderer, &radientBox);

    std::string activitiveText = "Activitive: " + std::to_string(static_cast<int>(player.getActivitive()));
    std::string radientText = "Radient: " + std::to_string(static_cast<int>(player.getRadient()));
    SDL_Surface* activitiveSurface = TTF_RenderText_Blended(textFont, activitiveText.c_str(), blackColor);
    SDL_Surface* radientSurface = TTF_RenderText_Blended(textFont, radientText.c_str(), blackColor);
    if (activitiveSurface && radientSurface) {
        SDL_Texture* activitiveTexture = SDL_CreateTextureFromSurface(renderer, activitiveSurface);
        SDL_Texture* radientTexture = SDL_CreateTextureFromSurface(renderer, radientSurface);
        SDL_Rect activitiveTextRect = {SCREEN_WIDTH - 190, 85, activitiveSurface->w, activitiveSurface->h};
        SDL_Rect radientTextRect = {SCREEN_WIDTH - 190, 115, radientSurface->w, radientSurface->h};
        SDL_RenderCopy(renderer, activitiveTexture, nullptr, &activitiveTextRect);
        SDL_RenderCopy(renderer, radientTexture, nullptr, &radientTextRect);
        SDL_DestroyTexture(activitiveTexture);
        SDL_DestroyTexture(radientTexture);
        SDL_FreeSurface(activitiveSurface);
        SDL_FreeSurface(radientSurface);
    }

    // 7. Vẽ thông báo bossfight (nếu có)
    if (messageTimer > 0) {
        SDL_Surface* messageSurface = TTF_RenderText_Blended(textFont, bossMessage.c_str(), blackColor);
        if (messageSurface) {
            SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
            SDL_Rect messageRect = {(SCREEN_WIDTH - messageSurface->w) / 2, 50, messageSurface->w, messageSurface->h};
            SDL_RenderCopy(renderer, messageTexture, nullptr, &messageRect);
            SDL_DestroyTexture(messageTexture);
            SDL_FreeSurface(messageSurface);
        }
        messageTimer -= 0.016f; // Giả định 60 FPS
    }
}