#include "Game.h"
#include "Constants.h"
#include <ctime>
#include <iostream>

Game::Game() 
    : window(nullptr), renderer(nullptr), isRunning(false), background(nullptr), 
      player(this), tappingSystem(&player, &bossBattle), ui(nullptr, nullptr), bossBattle(&player), 
      challenge(), challengeTimer(0), isBossMusicPlaying(false), 
      gameCompleteTexture(nullptr), gameComplete(false) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

Game::~Game() {
    if (gameCompleteTexture) SDL_DestroyTexture(gameCompleteTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Khởi tạo SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("Chilling Leveling", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    std::cerr << "Renderer created successfully: " << renderer << std::endl; // Debug

    if (!audioManager.init()) {
        std::cerr << "AudioManager could not initialize!" << std::endl;
        return false;
    }

    audioManager.loadBackgroundMusic("asset/bgm.flac");
    audioManager.loadBossMusic("asset/bossfight_music.flac");
    audioManager.loadEndGameMusic("asset/endgame_music.mp3");
    audioManager.loadSoundEffect("tap", "asset/tap_sound.wav");
    audioManager.loadSoundEffect("buy", "asset/buy_sound.wav");
    audioManager.playBackgroundMusic(-1);

    background = Background(renderer);
    if (!background.loadTextures("asset/background.png", "asset/background_overlay.png")) {
        std::cerr << "Failed to load background textures!" << std::endl;
        return false;
    }

    if (bossBattle.isActive()) {
        // background.setUseBossBackground(true);
        background.loadBossBackground("asset/bossfight_background.png");
    } else {
        background.setUseBossBackground(false);
    }
    // Tải hình nền bossfight

    // background.loadBossBackground("asset/bossfight_background.png");

    gameCompleteTexture = IMG_LoadTexture(renderer, "asset/game_complete.png");
    if (!gameCompleteTexture) {
        std::cerr << "Failed to load game complete texture: " << IMG_GetError() << std::endl;
        return false;
    }

    ui = UI(renderer, this);
    ui.loadResources();

    items.push_back(Item("Double Activitive", 2.0f, 45, 100));
    items.push_back(Item("Radient Boost", 2.0f, 60, 50));
    items.push_back(Item("Radient Hyper Boost", 5.0f, 30, 55));

    tappingSystem.setGame(this);

    isRunning = true;
    return true;
}

void Game::reset() {
    player = Player(this);
    challenge = Challenge();
    bossBattle = BossBattle(&player);
    items.clear();
    items.push_back(Item("Double Activitive", 2.0f, 45, 100));
    items.push_back(Item("Radient Boost", 2.0f, 60, 50));
    items.push_back(Item("Radient Hyper Boost", 5.0f, 30, 55));
    challengeTimer = 0;
    isBossMusicPlaying = false;
    gameComplete = false;
    audioManager.playBackgroundMusic(-1);
    background.setUseBossBackground(false);
}

void Game::update(float deltaTime) {
    background.update(deltaTime);

    if (!gameComplete) {  //note
        challengeTimer += deltaTime;
        if (challengeTimer >= 60.0f && !challenge.isActive() && !bossBattle.isActive()) {
            challenge.start();
            challengeTimer = 0.0f;
        }

        if (challenge.isActive()) {
            challenge.update(deltaTime);
            if (challenge.isCompleted()) {
                player.addRadient(challenge.getRadientReward());
                player.addActivitive(challenge.getActivitiveReward());
                ui.showLevelUpMessage(0);
                player.checkLevelUp();
            }
        }

        if (player.getLevel() >= 5 && !bossBattle.isActive() && !bossBattle.isWon() && !bossBattle.isLost()) {
            bossBattle.start();
            bossBattle.setBossStats(1000, 20);
            if (!isBossMusicPlaying) {
                audioManager.playBossMusic(-1);
                isBossMusicPlaying = true;
            }
            background.setUseBossBackground(true);
            ui.showBossMessage("Boss Level 5 Appears! Tap 727 times in 60 seconds!");
        }

        if (bossBattle.isActive()) {
            bossBattle.update(deltaTime);
            if (bossBattle.isWon()) {
                gameComplete = true;
                audioManager.stopMusic();
                audioManager.playEndGameMusic(0);
                isBossMusicPlaying = false;
                background.setUseBossBackground(false);
            } else if (bossBattle.isLost()) {
                // Không dừng game ngay, mà hiển thị lựa chọn
                audioManager.stopMusic();
                background.setUseBossBackground(false);
                ui.showBossMessage("You failed! Press R to retry, Q to restart game.");
            }
        } else {
            tappingSystem.update(deltaTime);
        }

        for (auto& item : items) {
            item.update(deltaTime);
        }

        player.checkLevelUp();

    }

    if (gameComplete && !Mix_PlayingMusic()) { //note
        isRunning = false;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    background.render();
    ui.render(player, tappingSystem, items, bossBattle, challenge);

    if (gameComplete) {  //note
        SDL_Rect dstRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, gameCompleteTexture, nullptr, &dstRect);
    }

    SDL_RenderPresent(renderer);
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    const float TARGET_FPS = 60.0f;
    const float TARGET_FRAME_TIME = 1000.0f / TARGET_FPS;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            tappingSystem.handleTapInput(event);
            ui.handleInput(event, player, items, bossBattle);

            // Xử lý lựa chọn khi thua bossfight
            if (!bossBattle.isActive() && bossBattle.isLost()) {
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_r) { // R: Retry bossfight
                        bossBattle.reset();
                        audioManager.playBossMusic(-1);
                        isBossMusicPlaying = true;
                        background.setUseBossBackground(true);
                        ui.showBossMessage("Boss Level 5 Appears! Tap 727 times in 60 seconds!");
                    } else if (event.key.keysym.sym == SDLK_q) { // Q: Restart game
                        reset();
                    }
                }
            }
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_u) {
            player.upgradeMo(50.0f, 2.0f);
        }

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        update(deltaTime);
        render();

        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < TARGET_FRAME_TIME) {
            SDL_Delay(static_cast<Uint32>(TARGET_FRAME_TIME - frameTime));
        }
    }
}

void Game::playTapSound() {
    audioManager.playSoundEffect("tap");
}

void Game::playBuySound() {
    audioManager.playSoundEffect("buy");
}

UI* Game::getUI() {
    return &ui;
}

Challenge* Game::getChallenge() {
    return &challenge;
}

std::vector<Item>& Game::getItems() {
    return items;
}