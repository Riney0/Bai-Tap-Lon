#include "UI.h"
#include "Game.h"
#include "Constants.h"
#include <iostream>


UI::UI(SDL_Renderer* renderer, Game* game) 
    : game(game), renderer(renderer), titleFont(nullptr), textFont(nullptr), 
      moTexture(nullptr), moVipTexture(nullptr), moSupaVipTexture(nullptr), memeTexture(nullptr), bossMessage(""), messageTimer(0.0f), memeFadeTimer(0.0f) {
    for (int i = 0; i < 3; i++) {
        itemTextures[i] = nullptr;
    }
    whiteColor = {255, 255, 255, 255}; // Màu trắng
    blueColor = {0, 128, 255, 255};    // Màu xanh lam
    cyanColor = {0, 255, 255, 255};    // Màu xanh lam nhạt
    blackColor = {31, 46, 51, 255}; // Màu đen than
    orangeColor = {251, 175, 0, 255}; // Màu cam
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
        // std::cerr << "Renderer is null! Cannot load resources." << std::endl;
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
    moBackgoundTexture = IMG_LoadTexture(renderer, "asset/mo_background.png");

    moTexture = IMG_LoadTexture(renderer, "asset/mo.png");
    if (!moTexture) {
        std::cerr << "Failed to load mo texture: " << IMG_GetError() << std::endl;
    }

    moVipTexture = IMG_LoadTexture(renderer, "asset/mo_vjp.png");
    if (!moVipTexture) {
        std::cerr << "Failed to load mo_vjp texture: " << IMG_GetError() << std::endl;
    }

    moSupaVipTexture = IMG_LoadTexture(renderer, "asset/mo_supa_vjp.jpg");
    if (!moSupaVipTexture) {
        std::cerr << "Failed to load mo_supa_vjp texture: " << IMG_GetError() << std::endl;
    }

    memeTexture = IMG_LoadTexture(renderer, "asset/bocchi_cum.jpg");
    if (!memeTexture) {
        std::cerr << "Failed to load meme texture: " << IMG_GetError() << std::endl;
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
    messageTimer = 60.0f; // Hiển thị thông báo trong 5 giây
}

void UI::showLevelUpMessage(int level) {
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

void UI::render(Player& player, TappingSystem& tappingSystem, std::vector<Item>& items, BossBattle& bossBattle, Challenge& challenge, float deltaTime) {
    // Nếu game đã hoàn thành, không vẽ UI
    if (game->isGameComplete()) {
        return;
    }

    // Kiểm tra deltaTime hợp lệ
    if (deltaTime <= 0.0f) {
        deltaTime = 0.016f; // 60 FPS nếu không hợ lệ
    }

    // Đảo 6 lên trước để không bị đè lên các thành phần khác

    // 6. Vẽ hình ảnh meme mỗi khi tap

    if (!bossBattle.isActive()){
        if (memeFadeTimer > 0 && memeTexture) {
            memeFadeTimer -= deltaTime;
            if (memeFadeTimer < 0) memeFadeTimer = 0;
        
            float alpha = (memeFadeTimer / MEME_FADE_DURATION) * 200.0f;
            if (alpha > 200.0f) alpha = 200.0f;
            if (alpha < 0.0f) alpha = 0.0f;

            Uint8 alphaByte = static_cast<Uint8>(alpha);
            // std::cout << "Rendering meme: memeFadeTimer=" << memeFadeTimer 
            //           << ", alpha=" << alpha << ", alphaByte=" << static_cast<int>(alphaByte) 
            //           << ", deltaTime=" << deltaTime << std::endl;

            // Alpha blending
            SDL_SetTextureBlendMode(memeTexture, SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(memeTexture, alphaByte);
            SDL_Rect memeRect = {0, 0, 1340, 750};
            SDL_RenderCopy(renderer, memeTexture, nullptr, &memeRect);
        
            // Reset alpha để tránh ảnh hưởng texture khác
            SDL_SetTextureAlphaMod(memeTexture, 200);
        }
    }


    // 1. Vẽ tiêu đề "CHILLING LEVELING"
    SDL_Surface* titleSurface1 = TTF_RenderText_Blended(titleFont, "CHILLING", blackColor);
    SDL_Surface* titleSurface2 = TTF_RenderText_Blended(titleFont, "       LEVELING", blackColor); // 7 dấu cách
    if (titleSurface1 && titleSurface2) {
        SDL_Texture* titleTexture1 = SDL_CreateTextureFromSurface(renderer, titleSurface1);
        SDL_Texture* titleTexture2 = SDL_CreateTextureFromSurface(renderer, titleSurface2);
        SDL_Rect titleRect1 = {15, 10, titleSurface1->w, titleSurface1->h};
        SDL_Rect titleRect2 = {15, 10 + titleSurface1->h, titleSurface2->w, titleSurface2->h};
        SDL_RenderCopy(renderer, titleTexture1, nullptr, &titleRect1);
        SDL_RenderCopy(renderer, titleTexture2, nullptr, &titleRect2);
        SDL_DestroyTexture(titleTexture1);
        SDL_DestroyTexture(titleTexture2);
        SDL_FreeSurface(titleSurface1);
        SDL_FreeSurface(titleSurface2);
    }

    // Vẽ background mõ
    SDL_Texture* moBackgroundTexture = moBackgoundTexture;
    if (moBackgroundTexture) {
        SDL_SetTextureAlphaMod(moBackgroundTexture, 170); // Đặt alpha cho background
        SDL_Rect moBackgroundRect = {530, 235, 280, 280};
        SDL_RenderCopy(renderer, moBackgroundTexture, nullptr, &moBackgroundRect);
    }

    // 2. Vẽ hình ảnh cái mõ
    SDL_Texture* currentMoTexture = bossBattle.isActive() ? moVipTexture : moTexture;
    if (currentMoTexture) {
        SDL_Rect moRect = {545, 250, 250, 250}; // Kích thước 250x250
        SDL_RenderCopy(renderer, currentMoTexture, nullptr, &moRect);
    }

    // 3. Vẽ hình cái mõ Supa VIP đè lên cái mõ vip khi vào bossfight
    
    if (bossBattle.isActive()){
        if (moSupaVipTimer > 0 && moSupaVipTexture) {
            moSupaVipTimer -= deltaTime;
            if (moSupaVipTimer < 0) moSupaVipTimer = 0;
            
            float alpha = (moSupaVipTimer / SUPA_MO_APPEAR_TIME) * 255.0f;
            if (alpha > 255.0f) alpha = 255.0f;
            if (alpha < 0.0f) alpha = 0.0f;

            Uint8 alphaByte = static_cast<Uint8>(alpha);
            SDL_SetTextureAlphaMod(moSupaVipTexture, alphaByte);
            SDL_Rect moSupaVipRect = {545, 250, 250, 250}; // Kích thước 250x250
            SDL_RenderCopy(renderer, moSupaVipTexture, nullptr, &moSupaVipRect);
        }
    }

    // 4. Vẽ số lần tap và level bên trên cái mõ
    std::string tapText = "Taps: " + std::to_string(player.getTapCount());
    std::string levelText = "Level: " + std::to_string(player.getLevel());
    SDL_Surface* tapSurface = TTF_RenderText_Blended(textFont, tapText.c_str(), blackColor);
    SDL_Surface* levelSurface = TTF_RenderText_Blended(textFont, levelText.c_str(), blackColor);
    if (tapSurface && levelSurface) {
        SDL_Texture* tapTexture = SDL_CreateTextureFromSurface(renderer, tapSurface);
        SDL_Texture* levelTexture = SDL_CreateTextureFromSurface(renderer, levelSurface);
        SDL_Rect tapRect = {545, 200, tapSurface->w, tapSurface->h};
        SDL_Rect levelRect = {670, 200, levelSurface->w, levelSurface->h};
        SDL_RenderCopy(renderer, tapTexture, nullptr, &tapRect);
        SDL_RenderCopy(renderer, levelTexture, nullptr, &levelRect);
        SDL_DestroyTexture(tapTexture);
        SDL_DestroyTexture(levelTexture);
        SDL_FreeSurface(tapSurface);
        SDL_FreeSurface(levelSurface);
    }

    // 5. Vẽ hộp trắng bên phải và 3 ô vật phẩm
    SDL_Rect whiteBox = {SCREEN_WIDTH - 200, 150, 180, 350}; // Hộp trắng
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &whiteBox);

    for (int i = 0; i < 3; i++) {
        SDL_Rect itemRect = {SCREEN_WIDTH - 190, 200 + i * 100, 160, 80};
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Màu xám nhạt cho ô vật phẩm
        SDL_RenderFillRect(renderer, &itemRect);

        // Vẽ tên vật phẩm và giá
        std::string itemText = items[i].getName(); // + " (" + std::to_string(items[i].getCost()) + " Radient)";
        std::string itemPrice = std::to_string(items[i].getCost());
        SDL_Surface* itemSurface1 = TTF_RenderText_Blended(textFont, itemText.c_str(), blackColor);

        // In ra tên vật phẩm
        if (itemSurface1) {
            SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(renderer, itemSurface1);
            SDL_Rect itemTextRect = {SCREEN_WIDTH - 180, 210 + i * 100, itemSurface1->w, itemSurface1->h};
            SDL_RenderCopy(renderer, itemTexture, nullptr, &itemTextRect);
            SDL_DestroyTexture(itemTexture);
            SDL_FreeSurface(itemSurface1);
        }

        // In ra tiền vật phẩm
        SDL_Surface* itemSurface2 = TTF_RenderText_Blended(textFont, itemPrice.c_str(), blackColor);
        if (itemSurface2) {
            SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(renderer, itemSurface2);
            SDL_Rect itemTextRect = {SCREEN_WIDTH - 180, 235 + i * 100, itemSurface2->w, itemSurface2->h};
            SDL_RenderCopy(renderer, itemTexture, nullptr, &itemTextRect);
            SDL_DestroyTexture(itemTexture);
            SDL_FreeSurface(itemSurface2);
        }
    }

    // 7. Vẽ hình ảnh vật phẩm đã mua (bên trái, mờ dần)
    for (int i = 0; i < 3; i++) {
        if (items[i].isActive() && itemTextures[i]) {
            float alpha = (items[i].getRemainingTime() / items[i].getDuration()) * 255; // Tính độ mờ
            SDL_SetTextureAlphaMod(itemTextures[i], static_cast<Uint8>(alpha));
            SDL_Rect itemEffectRect = {SCREEN_WIDTH / 2 - 650 + i * 120, 150 + i * 160, 230, 230};
            SDL_RenderCopy(renderer, itemTextures[i], nullptr, &itemEffectRect);
        }
    }

    // 8. Vẽ hộp activitive và radient bên trên hộp trắng
    SDL_Rect activitiveBox = {SCREEN_WIDTH - 200, 100, 180, 40};
    SDL_Rect radientBox = {SCREEN_WIDTH - 200, 135, 180, 40};
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
        SDL_Rect activitiveTextRect = {SCREEN_WIDTH - 190, 102, activitiveSurface->w, activitiveSurface->h};
        SDL_Rect radientTextRect = {SCREEN_WIDTH - 190, 140, radientSurface->w, radientSurface->h};
        SDL_RenderCopy(renderer, activitiveTexture, nullptr, &activitiveTextRect);
        SDL_RenderCopy(renderer, radientTexture, nullptr, &radientTextRect);
        SDL_DestroyTexture(activitiveTexture);
        SDL_DestroyTexture(radientTexture);
        SDL_FreeSurface(activitiveSurface);
        SDL_FreeSurface(radientSurface);
    }

    // 9. Vẽ thông báo bossfight (nếu có)
    if (messageTimer > 0) {
        SDL_Surface* messageSurface = TTF_RenderText_Blended(textFont, bossMessage.c_str(), blackColor);
        if (messageSurface) {
            SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
            SDL_Rect messageRect = {(SCREEN_WIDTH - messageSurface->w) / 2, 50, messageSurface->w, messageSurface->h};
            SDL_RenderCopy(renderer, messageTexture, nullptr, &messageRect);
            SDL_DestroyTexture(messageTexture);
            SDL_FreeSurface(messageSurface);
        }
        messageTimer -= 0.016f; // 60 FPS
    }

    // 10. Viết hướng dẫn game
    std::string guideText1 = "Press Space or left mouse to tap!";
    std::string guideText2 = "Click on the items to buy!";
    SDL_Surface* guideSurface1 = TTF_RenderText_Blended(textFont, guideText1.c_str(), blueColor);
    SDL_Surface* guideSurface2 = TTF_RenderText_Blended(textFont, guideText2.c_str(), blueColor);
    if(guideSurface1 && guideSurface2) {
        SDL_Texture* guideTexture1 = SDL_CreateTextureFromSurface(renderer, guideSurface1);
        SDL_Texture* guideTexture2 = SDL_CreateTextureFromSurface(renderer, guideSurface2);
        SDL_Rect guideRect1 = {SCREEN_WIDTH - 400, SCREEN_HEIGHT - 80, guideSurface1->w, guideSurface1->h};
        SDL_Rect guideRect2 = {SCREEN_WIDTH - 400, SCREEN_HEIGHT - 50, guideSurface2->w, guideSurface2->h};
        SDL_RenderCopy(renderer, guideTexture1, nullptr, &guideRect1);
        SDL_RenderCopy(renderer, guideTexture2, nullptr, &guideRect2);
        SDL_DestroyTexture(guideTexture1);
        SDL_DestroyTexture(guideTexture2);
        SDL_FreeSurface(guideSurface1);
        SDL_FreeSurface(guideSurface2);
    }

    // 11. Viết trạng thái thử thách (nếu có)
    if (!challenge.isActive()) return; // Chỉ hiển thị khi thử thách đang active

    // Tạo chuỗi văn bản: "Challenge: Tap XX times in XX.0s (xx/XX)"
    std::string challengeText = "Challenge: Tap " + std::to_string(challenge.getRequiredTaps()) +
                                " times in " + std::to_string(static_cast<int>(challenge.getTimer())) +
                                "s (" + std::to_string(challenge.getCurrentTaps()) + "/" +
                                std::to_string(challenge.getRequiredTaps()) + ")";

    // Render văn bản
    SDL_Surface* challengeSurface = TTF_RenderText_Blended(textFont, challengeText.c_str(), blackColor);
    if (challengeSurface) {
        SDL_Texture* challengeTexture = SDL_CreateTextureFromSurface(renderer, challengeSurface);
        if (challengeTexture) {
            // Hiển thị ở giữa trên cùng của màn hình
            SDL_Rect challengeRect = {
                (SCREEN_WIDTH - challengeSurface->w) / 2, // Căn giữa theo chiều ngang
                120, // Cách đỉnh màn hình 120 pixel
                challengeSurface->w,
                challengeSurface->h
            };
            SDL_RenderCopy(renderer, challengeTexture, nullptr, &challengeRect);
            SDL_DestroyTexture(challengeTexture);
        }
        SDL_FreeSurface(challengeSurface);
    }
}

void UI::triggerMemeEffect() {
    memeFadeTimer = MEME_FADE_DURATION;
    // std::cout << "UI::triggerMemeEffect called, memeFadeTimer set to " << MEME_FADE_DURATION << std::endl;
}

void UI::triggerSupaMoEffect() {
    moSupaVipTimer = SUPA_MO_APPEAR_TIME;
    // std::cout << "UI::triggerSupaMoEffect called, moSupaVipTimer set to " << SUPA_MO_APPEAR_TIME << std::endl;
}