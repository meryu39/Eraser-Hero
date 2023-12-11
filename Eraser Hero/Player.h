#pragma once
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <vector>
#include "Enemy.h"
#include "stage.h"

// 함수 선언

bool initSDL();
void closeSDL();

class Player {
public:
    Player(SDL_Renderer* renderer);
    ~Player();

    int getHealth() { return health; }
    int getDash() { return dash; }
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
    int player_state;

    int FRAME_WIDTH; //스프라이트 넓이
    int NUM_FRAMES; //스프라이트 개수

    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 720;
    int PLAYER_SIZE = 50;
    int OBJECT_SIZE = 30;
    int DASH_SPEED = 40;
    int MAX_DASH = 100;
};
