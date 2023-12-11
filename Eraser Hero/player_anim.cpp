#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_timer.h>
#include "Player.h"

// 전역 변수
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* playerTexture = nullptr;

const int OBJECT_SIZE = 30;

// 함수 선언

int current = 0;
int currentStage = 0;
bool story_start = true;
bool initSDL();
void closeSDL();
void renderPlayerHealth(Player player);
void renderPlayerDash(Player player);


int player_state;

Uint32 startTime = 0;
Uint32 elapsedTime = 0;
Uint32 lastUpdateTime = 0; // 초기값 설정
Uint32 updateInterval = 1000; // 1000 밀리초마다 업데이트

//1분
const Uint32 MAX_TIME = 60000;  

int main(int argc, char* args[]) {
    if (!initSDL()) {
        return -1;
    }
    srand(time(0));
    Player player(gRenderer);
    vector<Enemy> enemys;
    Pencil pencil(gRenderer, 100, 100);
    enemys.push_back(pencil);
    Sharp sharp (gRenderer, 100, 200);
    enemys.push_back(sharp);

    while (true) {
        if (enemys.empty())
        {
            
        }

        Uint32 frameStart = SDL_GetTicks();
        SDL_RenderClear(gRenderer);
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        //objects == enemy의 벡터 배열
        player.handleInput(enemys);

        player.handleCollision(enemys);

        //플레이어 상태 업데이트
        player.update(gRenderer);

        // 플레이어 렌더링
        player.drawTexture(gRenderer, player.currentTexture);

        for (int i = 0; i < enemys.size(); i++)
        {
            if (player.getX() - enemys[i].getX() < 0)
            {
                enemys[i].setDirectionX(-1);
            }
            else if (player.getX() - enemys[i].getX() > 0)
            {
                enemys[i].setDirectionX(1);
            }

            if (player.getY() - enemys[i].getY() < 0)
            {
                enemys[i].setDirectionY(-1);
            }
            else if (player.getY() - enemys[i].getY() > 0)
            {
                enemys[i].setDirectionY(1);
            }
            enemys[i].update(lastUpdateTime, updateInterval, gRenderer);
            enemys[i].drawTexture(gRenderer, enemys[i].currentTexture);
            if (enemys[i].getIsCharging())
            {
                enemys[i].charge(player.getX(), player.getY());
            }
            if (enemys[i].isOut)
            {
                enemys.erase(enemys.begin() + i);
            }
        }

        elapsedTime = SDL_GetTicks() - startTime;

        // Render the timer gauge
        SDL_Rect timerRect = { 10, 10, static_cast<int>(100 * (static_cast<float>(MAX_TIME - elapsedTime) / MAX_TIME)), 10 };
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
        SDL_RenderFillRect(gRenderer, &timerRect);

        renderPlayerHealth(player);
        renderPlayerDash(player);
 
        SDL_RenderPresent(gRenderer);
        //시간 지남 
        if (elapsedTime >= MAX_TIME) {
            //스테이지 넘김 함수 추가
            break;
        }
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 16) {
            SDL_Delay(16 - frameTime);
        }

        lastUpdateTime = SDL_GetTicks();
    }

    closeSDL();
    return 0;
}

bool initSDL() {
    // SDL 초기화
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

    // SDL_image 초기화
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image 초기화 실패: " << IMG_GetError() << std::endl;
        return false;
    }

    // 플레이어 텍스처 로드
    playerTexture = IMG_LoadTexture(gRenderer, "assets\\back.png");
    if (playerTexture == nullptr) {
        std::cerr << "플레이어 텍스처 로드 실패: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}

void closeSDL() {
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    IMG_Quit();
}


void renderPlayerHealth(Player player) {
    SDL_Rect healthBarRect = { SCREEN_WIDTH - 200, 20, static_cast<int>(100 * (static_cast<float>(player.getHealth()) / 100)), 20 };
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &healthBarRect);
}

void renderPlayerDash(Player player) {
    SDL_Rect dashBarRect = { SCREEN_WIDTH - 200, 50, static_cast<int>(100 * (static_cast<float>(player.getDash()) / 100)), 20 };
    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
    SDL_RenderFillRect(gRenderer, &dashBarRect);
}
