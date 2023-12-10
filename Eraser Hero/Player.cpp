#include "Player.h"


Player::Player(SDL_Renderer* renderer)
    : health(100), dash(0), isColliding(false), isDashing(false), dashDirectionX(0), dashDirectionY(0), dashCollider(false), frameIndex(0),
    FRAME_WIDTH(256), NUM_FRAMES(4) , damage(0), shield(0), speed(0), state(0)
{
    // 플레이어 텍스처 로드
    texture = IMG_LoadTexture(renderer, "player_walk.png");
    if (texture == nullptr) {
        std::cerr << "플레이어 텍스처 로드 실패: " << IMG_GetError() << std::endl;
    }

    // 초기 위치 및 크기 설정
    rect = { SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE };
    spriteRect = { 0, 0, 256, 192 };
}

Player::~Player() 
{
    SDL_DestroyTexture(texture);
}

void Player::anim() 
{
    frameIndex = (frameIndex + 1) % NUM_FRAMES;
    playerSpriteRect.x = frameIndex * FRAME_WIDTH;
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
            player_state = 0;
            anim();
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) 
        {
            x += (moveSpeed + speed);
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
                if (currentStage >= 2)
                {
                    state = 1;
                    shield = 0;
                    speed = 0;
                    damage = 5;
                }
                break;
            //화이트
            case SDLK_s:
                if (currentStage >= 3)
                {
                    state = 2;
                    shield = 0;
                    speed = 5;
                    damage = 0;
                }
                break;
            //딱풀
            case SDLK_d:
                if (currentStage >= 4)
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
                dashCollider = true;
                
                x += dashDirectionX * dash;
                y += dashDirectionY * dash;

                if (isColliding) 
                {
                    int pushDistance = dash;
                    int pushX = dashDirectionX * pushDistance;
                    int pushY = dashDirectionY * pushDistance;

                    objectRects[collidingTo].setX(objectRects[collidingTo].getX() + pushX + damage);
                    objectRects[collidingTo].setY(objectRects[collidingTo].getY() + pushY + damage);
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
            y + PLAYER_SIZE > objectRects[i].getY()) {

            if (!isColliding && isDashing) {
                // 피해 입히는 부분
                collidingTo = i;
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
            texture = IMG_LoadTexture(renderer, "assets\\charge\\Eraser - Sheet.gif");
        }
        else if (state == 1)
        {
            texture = IMG_LoadTexture(renderer, "assets\\ruler\\rulerCharge.png");
        }
        else if (state == 2)
        {
            texture = IMG_LoadTexture(renderer, "assets\\white\\whiteCharge.png");
        }
        else if (state == 3)
        {
            texture = IMG_LoadTexture(renderer, "!assets\\ddak\\ddakCharge.png");
        }
    }

    if (!isDashing)
    {
        if (state == 0)
        {
            texture = IMG_LoadTexture(renderer, "assets\\walk\\Eraser-Sheet.gif");
        }
        else if (state == 1)
        {
            texture = IMG_LoadTexture(renderer, "assets\\ruler\\rulerWalk.png");
        }
        else if (state == 2)
        {
            texture = IMG_LoadTexture(renderer, "assets\\white\\whiteCharge.png");
        }
        else if (state == 3)
        {
            texture = IMG_LoadTexture(renderer, "assets\\ddak\\ddakwalk.png");
        }
    }
    
    // 애니메이션 프레임 업데이트
    frameIndex = (frameIndex + 1) % NUM_FRAMES;
    spriteRect.x = frameIndex * FRAME_WIDTH;
}

void Player::render(SDL_Renderer* renderer) 
{
    // 플레이어 렌더링
    SDL_RenderCopy(renderer, texture, &spriteRect, &rect);
}
