#pragma once
#include <cstdlib>
#include <ctime>
#include <vector>
#include <SDL.h>
#include "enemy.h" 

class Stage
{
public:
    Stage(SDL_Renderer* renderer);
    ~Stage();
    void spawnRandomEnemy();

private:
    SDL_Renderer* stageRenderer;
};

