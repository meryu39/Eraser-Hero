#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_timer.h>

// 상수 정의
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int PLAYER_SIZE = 50;
const int OBJECT_SIZE = 30;
const int DASH_SPEED = 40;
const int MAX_DASH = 100;

// 전역 변수
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* playerTexture = nullptr;

SDL_Rect playerRect = { SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE };
SDL_Rect objectRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, OBJECT_SIZE, OBJECT_SIZE };

int playerHealth = 100;
int playerDash = 0;
bool isColliding = false;
bool isDashing = false;
int dashDirectionX = 0;
int dashDirectionY = 0;
bool dashCollider = false;

const int FRAME_WIDTH = 256;
const int NUM_FRAMES = 4;
int frameIndex = 0; // 현재 프레임 인덱스
SDL_Rect playerSpriteRect = { 0, 0, 256, 192 }; // 각 프레임의 크기에 맞게 조절
// 함수 선언

int current = 0;

bool initSDL();
void closeSDL();
void handleCollision();
void handleInput();
void renderPlayer();
void renderPlayerHealth();
void renderPlayerDash();

int player_state;

Uint32 startTime = 0;
Uint32 elapsedTime = 0;
//1분
const Uint32 MAX_TIME = 60000;  

int main(int argc, char* args[]) {
    if (!initSDL()) {
        return -1;
    }

    while (true) {
        Uint32 frameStart = SDL_GetTicks();

        handleInput();
        handleCollision();

        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        elapsedTime = SDL_GetTicks() - startTime;

        // Render the timer gauge
        SDL_Rect timerRect = { 10, 10, static_cast<int>(100 * (static_cast<float>(MAX_TIME - elapsedTime) / MAX_TIME)), 10 };
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
        SDL_RenderFillRect(gRenderer, &timerRect);
        renderPlayer();

        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(gRenderer, &objectRect);

        renderPlayerHealth();
        renderPlayerDash();

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
    playerTexture = IMG_LoadTexture(gRenderer, "player_walk.png");
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

void handleCollision() {
    if (playerRect.x < objectRect.x + OBJECT_SIZE &&
        playerRect.x + PLAYER_SIZE > objectRect.x &&
        playerRect.y < objectRect.y + OBJECT_SIZE &&
        playerRect.y + PLAYER_SIZE > objectRect.y) {

        if (!isColliding && !dashCollider) {
            playerHealth -= 10;
            std::cout << "플레이어 체력: " << playerHealth << std::endl;
            isColliding = true;
            isDashing = false;
        }
    }
    else if (isColliding) {
        isColliding = false;
    }
}

void anim() {
    frameIndex = (frameIndex + 1) % NUM_FRAMES;
    playerSpriteRect.x = frameIndex * FRAME_WIDTH;
}

void handleInput() {
    static bool spacePressed = false;
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);


    int moveSpeed = 5;

    if (isDashing) {
        moveSpeed = DASH_SPEED;
    }

    if (!spacePressed) {
        if (keyboardState[SDL_SCANCODE_UP]) {
            playerRect.y -= moveSpeed;
            dashDirectionX = 0;
            dashDirectionY = -1;
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            playerRect.y += moveSpeed;
            dashDirectionX = 0;
            dashDirectionY = 1;
        }
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            playerRect.x -= moveSpeed;
            dashDirectionX = -1;
            dashDirectionY = 0;
            player_state = 0;
            anim();
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            playerRect.x += moveSpeed;
            dashDirectionX = 1;
            dashDirectionY = 0;
            player_state = 1;
            anim();

        }
        playerSpriteRect.y = 192 * player_state;
    }

    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            closeSDL();
            exit(0);
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_SPACE:
                isDashing = true;
                spacePressed = true;
                break;
            }
        }
        else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_SPACE) {
                isDashing = false;
                spacePressed = false;
                dashCollider = true;

                playerRect.x += dashDirectionX * playerDash;
                playerRect.y += dashDirectionY * playerDash;

                if (playerRect.x < objectRect.x + OBJECT_SIZE &&
                    playerRect.x + PLAYER_SIZE > objectRect.x &&
                    playerRect.y < objectRect.y + OBJECT_SIZE &&
                    playerRect.y + PLAYER_SIZE > objectRect.y) {

                    int pushDistance = playerDash;
                    int pushX = dashDirectionX * pushDistance;
                    int pushY = dashDirectionY * pushDistance;

                    objectRect.x += pushX;
                    objectRect.y += pushY;
                }

                playerDash = 0;
            }
        }
    }

    if (isDashing && spacePressed) {
        if (keyboardState[SDL_SCANCODE_UP]) {
            dashDirectionX = 0;
            dashDirectionY = -1;
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            dashDirectionX = 0;
            dashDirectionY = 1;
        }
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            dashDirectionX = -1;
            dashDirectionY = 0;
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            dashDirectionX = 1;
            dashDirectionY = 0;
        }
    }

    if (isDashing && spacePressed && playerDash < MAX_DASH) {
        playerDash += 5;
    }
}


void renderPlayer() {
    SDL_RenderCopy(gRenderer, playerTexture, &playerSpriteRect, &playerRect);
}

void renderPlayerHealth() {
    SDL_Rect healthBarRect = { SCREEN_WIDTH - 200, 20, static_cast<int>(100 * (static_cast<float>(playerHealth) / 100)), 20 };
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &healthBarRect);
}

void renderPlayerDash() {
    SDL_Rect dashBarRect = { SCREEN_WIDTH - 200, 50, static_cast<int>(100 * (static_cast<float>(playerDash) / 100)), 20 };
    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
    SDL_RenderFillRect(gRenderer, &dashBarRect);
}
