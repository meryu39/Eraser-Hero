#include <SDL.h>
#include <iostream>
#include <algorithm>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int PLAYER_SIZE = 50;
const int OBJECT_SIZE = 30;
const int DASH_SPEED = 40;  // 대시 속도
const int MAX_DASH = 100;   // 최대 대시 게이지

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

SDL_Rect playerRect = { SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE };
SDL_Rect objectRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, OBJECT_SIZE, OBJECT_SIZE };

int playerHealth = 100;
int playerDash = 0; // 대시 게이지 초기값
bool isColliding = false;

bool isDashing = false;
int dashDirectionX = 0;
int dashDirectionY = 0;

bool dash_collider = false; // 대시중에 충돌여부

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

        if (!isColliding && !dash_collider) {
            //충돌시 체력감소
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

// 플레이어 입력 처리 함수, 이동 및 대시 포함
void handleInput() {
    // 스페이스바가 눌렸는지 여부를 추적하는 정적 변수
    static bool spacePressed = false;

    // 키보드의 현재 상태를 얻음
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    // 기본 이동 속도를 설정
    int moveSpeed = 5;

    // 현재 대시 중이라면 이동 속도를 조정
    if (isDashing) {
        moveSpeed = DASH_SPEED;
    }

    // 스페이스바가 눌리지 않은 경우 화살표 키 입력 처리
    if (!spacePressed) {
        if (keyboardState[SDL_SCANCODE_UP]) {
            // 위
            playerRect.y -= moveSpeed;
            dashDirectionX = 0;
            dashDirectionY = -1;
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            // 아래
            playerRect.y += moveSpeed;
            dashDirectionX = 0;
            dashDirectionY = 1;
        }
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            // 왼쪽
            playerRect.x -= moveSpeed;
            dashDirectionX = -1;
            dashDirectionY = 0;
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            // 오른쪽
            playerRect.x += moveSpeed;
            dashDirectionX = 1;
            dashDirectionY = 0;
        }
    }

    // 스페이스바
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            //창 닫힐 경우 게임종료
            closeSDL();
            exit(0);
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_SPACE:
                // 스페이스바 눌리는 동안
                isDashing = true;
                spacePressed = true;
                break;
            }
        }
        else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_SPACE) {
                // 스페이스바 떨어지면
                isDashing = false;
                spacePressed = false;
                dash_collider = true; // 대시 충돌 플래그 설정

                // 대쉬 방향과 대시 게이지에 플레이어 위치 이동 
                playerRect.x += dashDirectionX * playerDash;
                playerRect.y += dashDirectionY * playerDash;

                // 대시 중에 다른 오브젝트와 충돌한다면
                if (playerRect.x < objectRect.x + OBJECT_SIZE &&
                    playerRect.x + PLAYER_SIZE > objectRect.x &&
                    playerRect.y < objectRect.y + OBJECT_SIZE &&
                    playerRect.y + PLAYER_SIZE > objectRect.y) {

                    // 충돌 시 적 유닛 밀려남 
                    int pushDistance = playerDash; // 푸쉬 거리는 플레이어 거리
                    int pushX = dashDirectionX * pushDistance;
                    int pushY = dashDirectionY * pushDistance;

                    // 적 유닛 밀려나기 
                    objectRect.x += pushX;
                    objectRect.y += pushY;
                }

                
                playerDash = 0;
            }
        }
    }

    // 스페이스바가 눌린 상태일 때 화살표 키 입력 확인
    if (isDashing && spacePressed) {
        // 대시 방향을 화살표 키로 변경
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

    // 대시 중이면서 스페이스바가 눌린 상태일 때 대시 게이지 증가
    if (isDashing && spacePressed && playerDash < MAX_DASH) {
        playerDash += 5;
    }
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

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
        SDL_RenderFillRect(gRenderer, &playerRect);

        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(gRenderer, &objectRect);

        //플레이어 상태창
        renderPlayerHealth();
        renderPlayerDash(); 

        SDL_RenderPresent(gRenderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 16) {
            SDL_Delay(16 - frameTime);
        }
        printf("x : %d\n", dashDirectionX);
        printf("y : %d\n", dashDirectionY);
    }

    closeSDL();
    return 0;
}
