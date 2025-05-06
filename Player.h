#ifndef PLAYER_H
#define PLAYER_H
#include <SDL2/SDL.h>
class Game;

class Player {
public:
    Player(Game* game);
    void tapMo();
    void addRadient(float amount);
    void addActivitive(float amount);
    float getActivitive() const;
    float getRadient() const;
    int getLevel() const;
    int getTapCount() const;
    void checkLevelUp();
    void incrementTapCount();
    void update(float deltaTime);

private:
    Game* game;
    float activitive;
    float radient;
    int level;
    int tapCount;
    float moMultiplier;
    float activitiveMultiplier;
    float radientMultiplier;
    int previousLevel;
};

#endif