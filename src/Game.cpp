//
// Created by Dawid Osuchowski on 21/12/2019.
//

#include "Game.h"
#include "Player.h"
#include "Ball.h"

#include <SDL.h>

Game::Game(const std::string& windowTitle, const int& windowWidth, const int& windowHeight) : mWindowWidth(windowWidth), mWindowHeight(windowHeight) {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    }

    // create a window
    mWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, SDL_WINDOW_SHOWN);

    if(mWindow == nullptr) {
        SDL_Log("Could not create window: %s\n", SDL_GetError());
    }

    // create a renderer
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

    if(mRenderer == nullptr) {
        SDL_Log("Could not create renderer: %s\n", SDL_GetError());
    }
}

Game::~Game() {
    SDL_DestroyRenderer(mRenderer);

    // destroy window
    SDL_DestroyWindow(mWindow);

    // deactivate subsystems and quit SDL
    SDL_Quit();

}

void Game::run() {
    Player player(this);
    player.setRect({0, 0, 200, 20});
    player.setPos({300, 550});
    player.setVel({500, 0});
    player.setColor({255, 0, 0, 255});

    Ball ball(this);
    ball.setRect({0, 0, 10, 10});
    ball.setPos({200, 500});
    ball.setVel({400, -200});
    ball.setColor({255, 255, 255, 255});


    mGameObjects.push_back(&ball);
    mGameObjects.push_back(&player);

    unsigned int lastTime = SDL_GetTicks();

    // Run game loop
    while (mGameState != STATE_EXIT) {
        unsigned int current = SDL_GetTicks();
        double elapsed = current - lastTime;
        elapsed = elapsed / 1000;

        handleEvents();
        update(elapsed);
        render();

        lastTime = current;
    }
}

void Game::handleEvents() {
    while(SDL_PollEvent(&mEvent)) {
        switch (mEvent.type) {
            case SDL_QUIT:
                mGameState = STATE_EXIT;
                break;

            case SDL_KEYDOWN:
                keyPressed[mEvent.key.keysym.scancode] = true;
                break;

            case SDL_KEYUP:
                keyPressed[mEvent.key.keysym.scancode] = false;
                break;
        }
    }
}

SDL_Renderer *Game::getRenderer() const {
    return mRenderer;
}

void Game::update(const double &elapsed) {
        for(auto gameObject : mGameObjects) {
            gameObject->update(elapsed);
        }

}

void Game::render() {
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    for(auto gameObject : mGameObjects) {
        gameObject->render(mRenderer);
    }

    SDL_RenderPresent(mRenderer);
}

int Game::getWindowWidth() const {
    return mWindowWidth;
}

int Game::getWindowHeight() const {
    return mWindowHeight;
}
