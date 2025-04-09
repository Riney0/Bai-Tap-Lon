#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>

class Background {
public:
    Background(SDL_Renderer* renderer);
    ~Background();

    bool loadTextures(const char* bgPath, const char* layerPath);
    void loadBossBackground(const std::string& path);
    void setUseBossBackground(bool use); // Phương thức để bật/tắt boss background
    void update(float deltaTime); // Thêm deltaTime để tốc độ cuộn đồng đều
    void render();
    void clean();

private:
    SDL_Renderer* renderer; // Lưu renderer
    SDL_Texture* backgroundTexture;  // Ảnh nền tĩnh
    SDL_Texture* scrollingLayer;     // Ảnh layer cuộn
    SDL_Texture* bossBackgroundTexture; // Texture cho bossfight
    float scrollingOffset;           // Vị trí cuộn của layer (dùng float để mượt hơn)
    float scrollingSpeed;            // Tốc độ cuộn (pixel/giây)
    int layerWidth;                  // Chiều rộng của layer
    int layerHeight;                 // Chiều cao của layer
    bool useBossBackground; // Biến để kiểm soát việc hiển thị boss background
};

#endif