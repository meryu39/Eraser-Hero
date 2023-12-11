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
int current_Stage = 0;

bool story_start = true;

bool initSDL();
void closeSDL();
void renderPlayerHealth(Player player);
void renderPlayerDash(Player player);


int player_state;

const char* stage0[] = {
    "story\\1_0.png",
    "story\\1_1.png",
    "story\\1_2.png",
    "story\\1_3.png",
    "story\\1_4.png",
    "story\\1_5.png",
    "story\\1_6.png",
    "story\\1_7.png",
    "story\\2_1.png",
    "story\\2_2.png"
};

const char* stage1[] = {
    "story\\3_1.png",
    "story\\3_2.png",
    "story\\3_3.png",
    "story\\3_4.png",
    "story\\4_1.png"
};

const char* stage2[] = {
    "story\\5_1.png",
    "story\\5_2.png",
    "story\\5_3.png",
    "story\\5_4.png",
    "story\\5_5.png"
};

const char* stage3[] = {
    "story\\6_1.png",
    "story\\6_2.png",
    "story\\6_3.png"
    "story\\7_1.png",
    "story\\7_2.png",
    "story\\7_3.png",
    "story\\7_4.png"
};

const char* stage4[] = {
    "story\\8_1.png",
    "story\\8_2.png",
    "story\\8_3.png",
    "story\\8_4.png",
    "story\\8_5.png",
    "story\\8_6.png",
    "story\\8_7.png",
    "story\\8_8.png",
    "story\\8_9.png",
    "story\\8_10.png",
    "story\\8_11.png",
    "story\\8_12.png",
    "story\\8_13.png",
    "story\\8_14.png"

};

Uint32 startTime = 0;
Uint32 elapsedTime = 0;
Uint32 lastUpdateTime = 0; // 초기값 설정
Uint32 updateInterval = 1000; // 1000 밀리초마다 업데이트

//1분
const Uint32 MAX_TIME = 60000;  
void renderStageImage(const char* imagePath);
void conversation();
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
            current_Stage++;
            story_start = true;
        }
        if (story_start) {
            conversation();
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

void renderStageImage(const char* imagePath) {
    SDL_Surface* surface = IMG_Load(imagePath);
    if (surface == NULL) {
        printf("이미지를 불러올 수 없습니다. SDL_image 에러: %s\n", IMG_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    if (texture == NULL) {
        printf("텍스처를 생성할 수 없습니다. SDL 에러: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_FreeSurface(surface);

    // Render the image to the window
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, texture, NULL, NULL);
    SDL_RenderPresent(gRenderer);

    // Free the texture
    SDL_DestroyTexture(texture);
}

void conversation() {
    int currentImageIndex = 0;  
    SDL_Event e;
    int quit = 0;

    while (1) {
        const char** currentStage = nullptr;
        int totalImages = 0;
        int turn = 0;

        if (current_Stage == 0) {
            currentStage = stage0;
            totalImages = sizeof(stage0) / sizeof(stage0[0]);

        }
        else if (current_Stage == 1) {
            currentStage = stage1;
            totalImages = sizeof(stage1) / sizeof(stage1[0]);
        }
        else if (current_Stage == 2) {
            currentStage = stage2;
            totalImages = sizeof(stage2) / sizeof(stage2[0]);
        }
        else if (current_Stage == 3) {
            currentStage = stage3;
            totalImages = sizeof(stage3) / sizeof(stage3[0]);
        }
        else if(current_Stage == 4){
            currentStage = stage4;
            totalImages = sizeof(stage4) / sizeof(stage4[0]);
        }
    
       
    
        renderStageImage(currentStage[currentImageIndex]);
        SDL_RenderPresent(gRenderer);

        while (SDL_PollEvent(&e) != 0) {
   
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
 
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
        
                    currentImageIndex++;

                    if (currentImageIndex >= totalImages) {
                        story_start = false;  
                        return;
                    }
                }
            }
        }
    }
}

