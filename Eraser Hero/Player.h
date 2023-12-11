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

    SDL_Texture* currentTexture;
    int getHealth() { return health; }
    int getDash() { return dash; }
    int getX() { return x; }
    int getY() { return y; }
    void handleInput(std::vector<Enemy>& objectRects);
    void handleCollision(std::vector<Enemy>& objectRects);
    void update(SDL_Renderer* renderer);
    void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture);
    SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

    SDL_Rect rect;
private:

    int collidingTo;
    int x;
    int y;
    int health;
    int dash;
    int state;
    int shield;
    int speed;
    int damage;
    int width;
    int height;
    bool isColliding;
    bool isDashing;
    bool spacePressed;
    int dashDirectionX;
    int dashDirectionY;
    int dashCollider;
    int player_state;
    char* walkSpriteSheetPath; //걷기 모션의 경로
    char* chargeSpriteSheetPath; //돌진 모션의 경로
    char* currentSpriteSheetPath; //현재 모션 경로

    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 720;
    int PLAYER_SIZE = 50;
    int OBJECT_SIZE = 30;
    int DASH_SPEED = 40;
    int MAX_DASH = 100;
};
