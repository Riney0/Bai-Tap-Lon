
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

    // // giữ console mở
    // std::cout << "Press Enter to exit..." << std::endl;
    // std::cin.get();

    return 0;
}


// ĐM GAME LỒN ĐM GAME LỒN ĐM GAME LÔN
// TOI BI NGu
// g++ main.cpp Game.cpp UI.cpp TappingSystem.cpp BossBattle.cpp Challenge.cpp Player.cpp Item.cpp Background.cpp AudioManager.cpp -o game -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf