
#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Game game;

    // Khởi tạo game
    if (!game.init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }

    // Chạy game
    game.run();

    return 0;
}



// g++ main.cpp Game.cpp UI.cpp TappingSystem.cpp BossBattle.cpp Challenge.cpp Player.cpp Item.cpp Background.cpp AudioManager.cpp -o game -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
// Trung bình idle game không có menu🗣️