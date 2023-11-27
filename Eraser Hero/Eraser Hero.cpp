#include <SDL.h>
#include <iostream>
#include <algorithm>



const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
const int PLAYER_SIZE = 50;
const int OBJECT_SIZE = 30;
const int DASH_SPEED = 40;  // 돌진 속도

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

SDL_Rect playerRect = { SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE };
SDL_Rect objectRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, OBJECT_SIZE, OBJECT_SIZE };

int playerHealth = 100;
bool isColliding = false;

bool isDashing = false;
int dashDirectionX = 0;
int dashDirectionY = 0;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL 초기화 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    gWindow = SDL_CreateWindow("Eraser Hero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cerr << "윈도우 생성 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cerr << "렌더러 생성 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void closeSDL() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

void handleCollision() {
    if (playerRect.x < objectRect.x + OBJECT_SIZE &&
        playerRect.x + PLAYER_SIZE > objectRect.x &&
        playerRect.y < objectRect.y + OBJECT_SIZE &&
        playerRect.y + PLAYER_SIZE > objectRect.y) {

        if (!isColliding) {
            playerHealth -= 10;
            std::cout << "플레이어 체력: " << playerHealth << std::endl;
            isColliding = true;

            // 추가: 돌진 중이라면 돌진 상태 해제
            isDashing = false;
        }
    }
    else if (isColliding) {
        isColliding = false;
    }
}

void handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            closeSDL();
            exit(0);
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_UP:
                playerRect.y -= isDashing ? DASH_SPEED : 20;
                break;
            case SDLK_DOWN:
                playerRect.y += isDashing ? DASH_SPEED : 20;
                break;
            case SDLK_LEFT:
                playerRect.x -= isDashing ? DASH_SPEED : 20;
                break;
            case SDLK_RIGHT:
                playerRect.x += isDashing ? DASH_SPEED : 20;
                break;
            case SDLK_SPACE:
                // 스페이스바를 누르면 돌진
                isDashing = true;
                // 돌진 방향 설정 
                dashDirectionX = (e.key.keysym.sym == SDLK_LEFT) ? -1 : (e.key.keysym.sym == SDLK_RIGHT) ? 1 : 0;
                dashDirectionY = (e.key.keysym.sym == SDLK_UP) ? -1 : (e.key.keysym.sym == SDLK_DOWN) ? 1 : 0;
                break;
            }
        }
        else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_SPACE) {
                // 스페이스바를 떼면 돌진 상태 해제
                isDashing = false;
            }
        }
    }

    // 스페이스바를 누르고 있을 때 주변 색상을 빨간색으로 변경
    if (isDashing) {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(gRenderer, &r, &g, &b, &a);

        // 빨간색으로 점차 변경
        r = std::min<Uint8>(255, r + 5);
        g = std::max<Uint8>(0, g - 5);
        b = std::max<Uint8>(0, b - 5);

        SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
    }

}

void renderPlayerHealth() {
    SDL_Rect healthBarRect = { SCREEN_WIDTH - 120, 20, static_cast<int>(100 * (static_cast<float>(playerHealth) / 100)), 20 };
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &healthBarRect);
}

int main(int argc, char* args[]) {
    if (!initSDL()) {
        return -1;
    }

    while (true) {
        std::cout << isDashing;
        Uint32 frameStart = SDL_GetTicks();

        handleInput();
        handleCollision();

        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
        SDL_RenderFillRect(gRenderer, &playerRect);

        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(gRenderer, &objectRect);

        renderPlayerHealth();

        SDL_RenderPresent(gRenderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 16) {
            SDL_Delay(16 - frameTime);
        }
    }

    closeSDL();
    return 0;
}
