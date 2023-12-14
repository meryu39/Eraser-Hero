#include "Enemy.h"


Enemy::Enemy()
{
	isOut = false;
	width = 256;
	height = 192;
	directionX = 0;
	directionY = 1;
	isCharging = false;

	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
}

Enemy::~Enemy() 
{
	SDL_DestroyTexture(currentTexture);
}

void Enemy::charge(int targetX, int targetY)
{
		// 목표 위치까지 도달했는지 확인
		if (this->x != targetX || this->y != targetY) {
			// x 좌표로 도달
			if (this->x < targetX) {
				this->x += this->speed;
			}
			else if (this->x > targetX) {
				this->x -= this->speed;
			}

			// y 좌표로 도달
			if (this->y < targetY) {
				this->y += this->speed;
			}
			else if (this->y > targetY) {
				this->y -= this->speed;
			}
		}
}

void Enemy::update(Uint32 lastUpdateTime, Uint32 updateInterval, SDL_Renderer* renderer) {
	Uint32 currentTime = SDL_GetTicks();
	currentSpriteSheetPath = isCharging ? chargeSpriteSheetPath : walkSpriteSheetPath;
	loadTexture(currentSpriteSheetPath, renderer);
	if (currentTime - lastUpdateTime >= updateInterval) 
	{
		lastUpdateTime = currentTime;

		directionX = rand() % 2;
		directionY = rand() % 2;

		isCharging = (rand() % 2 == 0);
	}
	x += directionX;
	y += directionY;

	if (x < 0) {
		isOut = true;
	}
	else if (x > SCREEN_WIDTH - 50) {
		isOut = true;
	}

	if (y < 0) {
		isOut = true;
	}
	else if (y > SCREEN_HEIGHT - 50) {
		isOut = true;
	}
}

void Enemy::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture) 
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

SDL_Texture* Enemy::loadTexture(const char* path, SDL_Renderer* renderer) {
	SDL_Surface* surface = IMG_Load(path);
	if (!surface) {
		cerr << "이미지를 로드하는데 실패했습니다: " << IMG_GetError() << endl;
		return nullptr;
	}
	currentTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return currentTexture;
}

Pencil::Pencil(SDL_Renderer* renderer, int spwanX, int spwanY)
{
	walkSpriteSheetPath = (char*)"assets\\pencil.png";
	chargeSpriteSheetPath = (char*)"assets\\pencil.png";
	x = spwanX;
	y = spwanY;
	resistance = 0;
	damage = 1;
	speed = 10;
}


Sharp::Sharp(SDL_Renderer* renderer, int spwanX, int spwanY)
{
 	walkSpriteSheetPath = (char*)"assets\\sharp.png";
	chargeSpriteSheetPath = (char*)"assets\\sharp.png";
	x = spwanX;
	y = spwanY;
	resistance = 2;
	damage = 2;
	speed = 15;
}

Fountain::Fountain(SDL_Renderer* renderer, int spwanX, int spwanY)
{
	walkSpriteSheetPath = (char*)"assets\\fountain.png";
	chargeSpriteSheetPath = (char*)"assets\\fountain.png";
	x = spwanX;
	y = spwanY;
	resistance = 0;
	damage = 5;
	speed = 10;
	loadTrailTexture(renderer);
	trailLastTime = 5;
	trailUpdateTime = 1;
	trailWidth = 10;
	trailHeight = 10;
}


void Fountain::loadTrailTexture(SDL_Renderer* renderer) {
	trailTexture = loadTexture("assets\\trail.png", renderer);
	if (!trailTexture) {
		cerr << "자국 이미지를 로드하는데 실패했습니다." << endl;
	}
}

void Fountain::updateTrail() 
{
	// 일정 간격으로 자국 업데이트
	if (SDL_GetTicks() % trailUpdateTime == 0)
	{
		TrailPoint newPoint = { x, y, SDL_GetTicks() };
		trail.push_back(newPoint);
	}
}

void Fountain::charge(int targetX, int targetY) 
{
	// 기본 클래스의 charge 함수 호출
	Enemy::charge(targetX, targetY);

	// 자국 업데이트
	updateTrail();
}

void Fountain::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture) 
{
	// 기본 클래스의 draw 함수 호출
	Enemy::drawTexture(renderer, texture);

	// 현재 시간
	Uint32 currentTime = SDL_GetTicks();

	// 자국 렌더링 및 일정 시간 경과한 자국 제거
	for (auto it = trail.begin(); it != trail.end(); ) 
	{
		SDL_Rect trailRect = { it->x, it->y, trailWidth, trailHeight };

		// 일정 시간이 경과하면 자국을 제거
		if (static_cast<int>(currentTime - it->createTime) > trailLastTime)
		{
			it = trail.erase(it);
		}
		else 
		{
			drawTexture(renderer, trailTexture);
			++it;
		}
	}
}

Compass::Compass(SDL_Renderer* renderer, int spwanX, int spwanY)
{
	walkSpriteSheetPath = (char*)"assets\\compass.png";
	chargeSpriteSheetPath = (char*)"assets\\compass.png";
	x = spwanX;
	y = spwanY;
	resistance = 4;
	damage = 10;
	speed = 12;
}

void Compass::charge(Uint32 currentTime, Uint32 chargeStartTime, Uint32 chargeDuration)
{
	// 일정 시간 동안 돌진
	if (currentTime - chargeStartTime <= chargeDuration) {
		// 대상 위치까지 일정 간격으로 이동
		int deltaX = compassTargetX - this->x;
		int deltaY = compassTargetY - this->y;
		double distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);

		if (distance > 0) {
			double normalizedX = static_cast<double>(deltaX) / distance;
			double normalizedY = static_cast<double>(deltaY) / distance;

			this->x += static_cast<int>(normalizedX * speed);
			this->y += static_cast<int>(normalizedY * speed);
		}
	}
	else
	{
		Enemy::charge(lastTargetX, lastTargetY);
	}
}

void Compass::update(Uint32 lastUpdateTime, Uint32 updateInterval) {
	Uint32 currentTime = SDL_GetTicks();
	if (currentTime - lastUpdateTime >= updateInterval) {
		lastUpdateTime = currentTime;

		directionX = rand() % 3 - 1;
		directionY = rand() % 3 - 1;

		isCharging = rand() % 2 == 0;

		if (isCharging) {
			chargeStartTime = currentTime;
			chargeDuration = 3000;
			updateLastTarget(compassTargetX, compassTargetY);
		}
	}

	// 기존의 이동 로직
	x += directionX;
	y += directionY;

	if (x < 0) {
		x = 0;
		directionX *= -1;
	}
	else if (x > SCREEN_WIDTH - 50) {
		x = SCREEN_WIDTH - 50;
		directionX *= -1;
	}

	if (y < 0) {
		y = 0;
		directionY *= -1;
	}
	else if (y > SCREEN_HEIGHT - 50) {
		y = SCREEN_HEIGHT - 50;
		directionY *= -1;
	}

	// 돌진 로직 추가
	if (isCharging) {
		charge(currentTime, chargeStartTime, chargeDuration);
	}
}

void Compass::updateLastTarget(int targetX, int targetY) 
{
	lastTargetX = targetX;
	lastTargetY = targetY;
}