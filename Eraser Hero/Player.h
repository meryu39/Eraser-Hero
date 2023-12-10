#pragma once
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <vector>
#include "Enemy.h"
#include "stage.h"

// 상수 정의
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int PLAYER_SIZE = 50;
const int OBJECT_SIZE = 30;
const int DASH_SPEED = 40;
const int MAX_DASH = 100;

SDL_Rect playerRect = { SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE };


const int FRAME_WIDTH = 256;
const int NUM_FRAMES = 4;
int frameIndex = 0; // 현재 프레임 인덱스
SDL_Rect playerSpriteRect = { 0, 0, 256, 192 }; // 각 프레임의 크기에 맞게 조절
// 함수 선언

bool initSDL();
void closeSDL();
void handleCollision();
void handleInput();
void renderPlayer();
void renderPlayerHealth();
void renderPlayerDash();

int player_state;

class Player {
public:
    Player(SDL_Renderer* renderer);
    ~Player();

    void handleInput(std::vector<Enemy>& objectRects);
    void handleCollision(std::vector<Enemy>& objectRects);
    void update(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void anim();

private:
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Rect spriteRect;

    int collidingTo;
    int x;
    int y;
    int health;
    int dash;
    int state;
    int shield;
    int speed;
    int damage;
    bool isColliding;
    bool isDashing;
    bool spacePressed;
    int dashDirectionX;
    int dashDirectionY;
    int dashCollider;
    int frameIndex;

    const int FRAME_WIDTH;
    const int NUM_FRAMES;
};
