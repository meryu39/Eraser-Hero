﻿#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    gWindow = SDL_CreateWindow("SDL Image Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    return true;
}
bool loadMedia(const std::string& imagePath) {
    gTexture = IMG_LoadTexture(gRenderer, imagePath.c_str());
    if (gTexture == nullptr) {
        std::cerr << "Failed to load image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    return true;
}
void closeSDL() {
    if (gTexture != nullptr) {
        SDL_DestroyTexture(gTexture);
        gTexture = nullptr;
    }
    if (gRenderer != nullptr) {
        SDL_DestroyRenderer(gRenderer);
        gRenderer = nullptr;
    }
    if (gWindow != nullptr) {
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
}
int main(int argc, char* args[]) {
    if (!initSDL()) {
        std::cerr << "Failed to initialize SDL." << std::endl;
        return -1;
    }
    if (!loadMedia("assets/temp_start.png")) {
        std::cerr << "Failed to load media." << std::endl;
        return -1;
    }
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                // 스페이스 키를 누를 때 다음 화면으로 전환합니다.
                if (!loadMedia("assets/temp_next.png")) {
                    std::cerr << "Failed to load media." << std::endl;
                    return -1;
                }
            }
        }
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
        SDL_RenderPresent(gRenderer);
    }
    closeSDL();
    return 0;
}