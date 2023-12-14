#include "Player.h"

extern int current_Stage;

Player::Player(SDL_Renderer* renderer)
    : health(1000), dash(0), isColliding(false), isDashing(false), dashDirectionX(0), dashDirectionY(0), dashCollider(false),
    damage(100), shield(0), speed(0), state(0), x(1000), y(100), player_state(0), spacePressed(false), width(256), height(192)
{
    SCREEN_WIDTH = 1280;
    SCREEN_HEIGHT = 720;
    PLAYER_SIZE = 50;
    OBJECT_SIZE = 30;
    DASH_SPEED = 40;
    MAX_DASH = 100;


    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
}

Player::~Player() 
{
    SDL_DestroyTexture(currentTexture);
}

void Player::handleInput(std::vector<Enemy>& objectRects) 
{

    static bool spacePressed = false;
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);


    int moveSpeed = 5;

    if (isDashing) {
        moveSpeed = DASH_SPEED;
    }

    if (!spacePressed) {
        if (keyboardState[SDL_SCANCODE_UP]) 
        {
            y -= (moveSpeed + speed);
            dashDirectionX = 0;
            dashDirectionY = -1;
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) 
        {
            y += (moveSpeed + speed);
            dashDirectionX = 0;
            dashDirectionY = 1;
        }
        if (keyboardState[SDL_SCANCODE_LEFT]) 
        {
            x -= (moveSpeed + speed);
            dashDirectionX = -1;
            dashDirectionY = 0;
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) 
        {
            x += (moveSpeed + speed);
            dashDirectionX = 1;
            dashDirectionY = 0;
        }
    }

    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            closeSDL();
            exit(0);
        }
        else if (e.type == SDL_KEYDOWN) 
        {
            switch (e.key.keysym.sym) 
            {
            case SDLK_SPACE:
                isDashing = true;
                spacePressed = true;
                break;
            //자
            case SDLK_a:
                if (current_Stage == 2)
                {
                    state = 1;
                    shield = 0;
                    speed = 0;
                    damage = 5;
                }
                break;
            //화이트
            case SDLK_s:
                if (current_Stage == 3)
                {
                    state = 2;
                    shield = 0;
                    speed = 5;
                    damage = 0;
                }
                break;
            //딱풀
            case SDLK_d:
                if (current_Stage >= 4)
                {
                    state = 3;
                    shield = 5;
                    speed = 0;
                    damage = 0;
                }
                break;
            }


        }
        else if (e.type == SDL_KEYUP) 
        {
            if (e.key.keysym.sym == SDLK_SPACE) 
            {
                isDashing = false;
                spacePressed = false;
                
                x += dashDirectionX * dash;
                y += dashDirectionY * dash;

                if (dashCollider) 
                {
                    int pushDistance = dash;
                    int pushX = dashDirectionX * pushDistance;
                    int pushY = dashDirectionY * pushDistance;

                    objectRects[collidingTo].setX(objectRects[collidingTo].getX() + pushX - damage);
                    objectRects[collidingTo].setY(objectRects[collidingTo].getY() + pushY);
                }

                dash = 0;
            }
        }
    }

    if (isDashing && spacePressed) 
    {
        if (keyboardState[SDL_SCANCODE_UP]) 
        {
            dashDirectionX = 0;
            dashDirectionY = -1;
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) 
        {
            dashDirectionX = 0;
            dashDirectionY = 1;
        }
        if (keyboardState[SDL_SCANCODE_LEFT]) 
        {
            dashDirectionX = -1;
            dashDirectionY = 0;
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) 
        {
            dashDirectionX = 1;
            dashDirectionY = 0;
        }
    }

    if (isDashing && spacePressed && dash < MAX_DASH) 
    {
        dash += 5;
    }
}

void Player::handleCollision(std::vector<Enemy>& objectRects) 
{
    for (int i = 0; i < objectRects.size(); i++)
    {
        if (x < objectRects[i].getX() + objectRects[i].getWidth() &&
            x + PLAYER_SIZE > objectRects[i].getX() &&
            y < objectRects[i].getY() + objectRects[i].getHeight() &&
            y + PLAYER_SIZE > objectRects[i].getY())
        {
            collidingTo = i;
            dashCollider = true;
            isColliding = true;
                if (isColliding && !isDashing)
                {
                    // 피해 입히는 부분
                    health -= (objectRects[i].getDamage() - shield);
                    isColliding = true;
                }
        }
    }

    if (!isColliding) 
    {
        // 모든 객체와의 충돌이 끝났을 때 isColliding 리셋
        isColliding = false;
    }
}

void Player::update(SDL_Renderer* renderer)
{
    if (isDashing && !spacePressed)
    {
        if (state == 0)
        {
            loadTexture((const char* )"assets\\charge.png", renderer);
        }
        else if (state == 1)
        {
            loadTexture((const char*)"assets\\rulerDash.png", renderer);
        }
        else if (state == 2)
        {
            loadTexture((const char*)"assets\\whiteDash.png", renderer);
        }
        else if (state == 3)
        {
            loadTexture((const char*)"assets\\ddakDash.png", renderer);
        }
    }

    if (!isDashing)
    {
        if (state == 0)
        {
            loadTexture((const char*)"assets\\eraser.png", renderer);
        }
        else if (state == 1)
        {
            loadTexture((const char*)"assets\\ruler\\rulerWalk.png", renderer);
        }
        else if (state == 2)
        {
            loadTexture((const char*)"assets\\white\\whiteCharge.png", renderer);
        }
        else if (state == 3)
        {
            loadTexture((const char*)"assets\\ddak\\ddakwalk.png", renderer);
        }
    }
}

SDL_Texture*  Player::loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        cerr << "이미지를 로드하는데 실패했습니다: " << IMG_GetError() << endl;
        return nullptr;
    }
    currentTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return currentTexture;
}

void Player::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_Rect src, dst;
    src.x = src.y = 0;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
    dst.w = src.w;
    dst.h = src.h;
    SDL_RenderCopy(renderer, texture, &src, &dst);
}