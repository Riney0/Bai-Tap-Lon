#include "Background.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Constants.h" // Để dùng SCREEN_WIDTH, SCREEN_HEIGHT

Background::Background(SDL_Renderer* renderer) 
    : renderer(renderer), backgroundTexture(nullptr), scrollingLayer(nullptr), 
      scrollingOffset(0), scrollingSpeed(50.0f), layerWidth(0), layerHeight(0) {}

Background::~Background() {
    clean();
}

bool Background::loadTextures(const char* bgPath, const char* layerPath) {
    // Tải ảnh nền tĩnh
    SDL_Surface* bgSurface = IMG_Load(bgPath);
    if (!bgSurface) {
        std::cerr << "Failed to load background image: " << IMG_GetError() << std::endl;
        return false;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    if (!backgroundTexture) {
        std::cerr << "Failed to create background texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(bgSurface);
        return false;
    }
    SDL_FreeSurface(bgSurface);

    // Tải layer cuộn
    SDL_Surface* layerSurface = IMG_Load(layerPath);
    if (!layerSurface) {
        std::cerr << "Failed to load scrolling layer: " << IMG_GetError() << std::endl;
        return false;
    }
    scrollingLayer = SDL_CreateTextureFromSurface(renderer, layerSurface);
    if (!scrollingLayer) {
        std::cerr << "Failed to create scrolling layer texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(layerSurface);
        return false;
    }
    // Lấy kích thước của layer
    SDL_QueryTexture(scrollingLayer, nullptr, nullptr, &layerWidth, &layerHeight);
    SDL_FreeSurface(layerSurface);

    return true;
}

void Background::loadBossBackground(const std::string& path) {
    SDL_Surface* bossSurface = IMG_Load(path.c_str());
    if (!bossSurface) {
        std::cerr << "Failed to load boss background image: " << IMG_GetError() << std::endl;
        return;
    }
    bossBackgroundTexture = SDL_CreateTextureFromSurface(renderer, bossSurface);
    if (!bossBackgroundTexture) {
        std::cerr << "Failed to create boss background texture: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(bossSurface);
}

// void Background::loadBossBackground(const std::string& path) {
//     bossBackgroundTexture = IMG_LoadTexture(renderer, path.c_str());
//     if (!bossBackgroundTexture) {
//         std::cerr << "Failed to load boss background texture: " << IMG_GetError() << std::endl;
//     } else {
//         std::cerr << "Boss background texture loaded successfully: " << path << std::endl;
//     }
// }

void Background::setUseBossBackground(bool use) {
    useBossBackground = use;
}

void Background::update(float deltaTime) {
    scrollingOffset -= scrollingSpeed * deltaTime; // Tốc độ cuộn dựa trên deltaTime
    if (scrollingOffset <= -layerWidth) {
        scrollingOffset = 0;
    }
}

void Background::render() {
    // Vẽ background tĩnh
    // Vẽ background (dùng boss background nếu useBossBackground là true)
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    if (useBossBackground && bossBackgroundTexture) {
        SDL_RenderCopy(renderer, bossBackgroundTexture, nullptr, &bgRect);
    } else {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);
    }

    // Vẽ layer cuộn (cuộn tuần hoàn)
    SDL_Rect layerRect1 = {static_cast<int>(scrollingOffset), 0, layerWidth, layerHeight};
    SDL_Rect layerRect2 = {static_cast<int>(scrollingOffset) + layerWidth, 0, layerWidth, layerHeight};
    SDL_RenderCopy(renderer, scrollingLayer, nullptr, &layerRect1);
    SDL_RenderCopy(renderer, scrollingLayer, nullptr, &layerRect2);
}

void Background::clean() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }
    if (scrollingLayer) {
        SDL_DestroyTexture(scrollingLayer);
        scrollingLayer = nullptr;
    }
    if (bossBackgroundTexture) {
        SDL_DestroyTexture(bossBackgroundTexture);
        bossBackgroundTexture = nullptr;
    }
}