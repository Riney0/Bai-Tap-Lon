#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL2/SDL.h>

// Kích thước màn hình
const int SCREEN_WIDTH = 1340;
const int SCREEN_HEIGHT = 750;

// Các hằng số khác
const float BASE_ACTIVITIVE_PER_TAP = 1.5f;
const float ACTIVITIVE_TO_RADIENT_RATE = 0.1f;
const int MAX_TAP_SPEED = 100;

// Màu sắc
const SDL_Color COLOR_WHITE = {255, 255, 255, 255};
const SDL_Color COLOR_GREEN = {0, 255, 0, 255};
const SDL_Color COLOR_BLACK = {0, 0, 0, 1};

#endif