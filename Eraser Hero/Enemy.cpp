#include "Enemy.h"

Enemy::Enemy()
{
	currentFrame = 0; 
	directionX = 0;
	directionY = 1;
	isCharging = false;
}

Enemy::~Enemy() 
{
	SDL_DestroyTexture(walkTexture);
	SDL_DestroyTexture(chargeTexture);
}

void Enemy::charge(int targetX, int targetY)
{
		// ��ǥ ��ġ���� �����ߴ��� Ȯ��
		if (this->x != targetX || this->y != targetY) {
			// x ��ǥ�� ����
			if (this->x < targetX) {
				this->x += this->speed;
			}
			else if (this->x > targetX) {
				this->x -= this->speed;
			}

			// y ��ǥ�� ����
			if (this->y < targetY) {
				this->y += this->speed;
			}
			else if (this->y > targetY) {
				this->y -= this->speed;
			}
		}
}

void Enemy::update(Uint32 lastUpdateTime, Uint32 updateInterval) {
	Uint32 currentTime = SDL_GetTicks();
	if (currentTime - lastUpdateTime >= updateInterval) {
		lastUpdateTime = currentTime;

		directionX = rand() % 3 - 1;
		directionY = rand() % 3 - 1;

		isCharging = rand() % 2 == 0;
	}

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

	currentFrame = (currentFrame + 1) % (rows * columns);
}

void Enemy::render(SDL_Renderer* renderer) {
	int frameWidth = SCREEN_WIDTH / columns;
	int frameHeight = SCREEN_HEIGHT / rows;

	SDL_Rect srcRect = { (currentFrame % columns) * frameWidth, (currentFrame / columns) * frameHeight, frameWidth, frameHeight };
	SDL_Rect destRect = { x, y, frameWidth, frameHeight };

	SDL_Texture* currentTexture = isCharging ? chargeTexture : walkTexture;
	SDL_RenderCopy(renderer, currentTexture, &srcRect, &destRect);
}

SDL_Texture* Enemy::loadTexture(const char* path, SDL_Renderer* renderer) {
	SDL_Surface* surface = IMG_Load(path);
	if (!surface) {
		cerr << "�̹����� �ε��ϴµ� �����߽��ϴ�: " << IMG_GetError() << endl;
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}

bool Enemy::checkCollision(Player& other) //�浹 ����� �÷��̾�� ���� , ���� �÷��̾� Ŭ���� �������̶� ������ �̺���... ����;;;
{
	// ���� ������Ʈ�� ��� ����
	SDL_Rect rect1 = { x, y, width, height };

	// �ٸ� ������Ʈ�� ��� ����
	SDL_Rect rect2 = { other.x, other.y, other.width, other.height };

	// �浹 ���θ� SDL�� �Լ��� ����Ͽ� Ȯ��
	return SDL_HasIntersection(&rect1, &rect2) == SDL_TRUE;
}

Pencil::Pencil(SDL_Renderer* renderer, int spwanX, int spwanY)
{
	walkSpriteSheetPath = (char*)"assets\\walk\\Pencil-Sheet.gif";
	chargeSpriteSheetPath = (char*)"assets\\charge\\Pencil-Sheet.gif";
	x = spwanX;
	y = spwanY;
	resistance = 0;
	damage = 10;
	speed = 10;
	walkTexture = loadTexture(walkSpriteSheetPath, renderer);
	chargeTexture = loadTexture(chargeSpriteSheetPath, renderer);
}


Sharp::Sharp(SDL_Renderer* renderer, int spwanX, int spwanY)
{
	walkSpriteSheetPath = (char*)"assets\\walk\\Sharp_pen-Sheet.gif";
	chargeSpriteSheetPath = (char*)"assets\\charge\\Sharp_pen_2-Sheet.gif";
	x = spwanX;
	y = spwanY;
	resistance = 2;
	damage = 15;
	speed = 15;
	walkTexture = loadTexture(walkSpriteSheetPath, renderer);
	chargeTexture = loadTexture(chargeSpriteSheetPath, renderer);
}

Fountain::Fountain(SDL_Renderer* renderer, int spwanX, int spwanY)
{
	walkSpriteSheetPath = (char*)"assets\\walk\\Fountain_pen-Sheet.gif";
	chargeSpriteSheetPath = (char*)"assets\\charge\\Fountain_pen-Sheet.gif";
	x = spwanX;
	y = spwanY;
	resistance = 0;
	damage = 10;
	speed = 10;
	walkTexture = loadTexture(walkSpriteSheetPath, renderer);
	chargeTexture = loadTexture(chargeSpriteSheetPath, renderer);
	loadTrailTexture(renderer);
	trailLastTime = 5;
	trailUpdateTime = 1;
	trailWidth = 10;
	trailHeight = 10;
}


void Fountain::loadTrailTexture(SDL_Renderer* renderer) {
	trailTexture = loadTexture("!�����̹��� ���!", renderer);
	if (!trailTexture) {
		cerr << "�ڱ� �̹����� �ε��ϴµ� �����߽��ϴ�." << endl;
	}
}

void Fountain::updateTrail() 
{
	// ���� �������� �ڱ� ������Ʈ
	if (SDL_GetTicks() % trailUpdateTime == 0)
	{
		TrailPoint newPoint = { x, y, SDL_GetTicks() };
		trail.push_back(newPoint);
	}
}

void Fountain::charge(int targetX, int targetY) 
{
	// �⺻ Ŭ������ charge �Լ� ȣ��
	Enemy::charge(targetX, targetY);

	// �ڱ� ������Ʈ
	updateTrail();
}

void Fountain::render(SDL_Renderer* renderer) 
{
	// �⺻ Ŭ������ render �Լ� ȣ��
	Enemy::render(renderer);

	// ���� �ð�
	Uint32 currentTime = SDL_GetTicks();

	// �ڱ� ������ �� ���� �ð� ����� �ڱ� ����
	for (auto it = trail.begin(); it != trail.end(); ) 
	{
		SDL_Rect trailRect = { it->x, it->y, trailWidth, trailHeight };

		// ���� �ð��� ����ϸ� �ڱ��� ����
		if (currentTime - it->createTime > trailLastTime)
		{
			it = trail.erase(it);
		}
		else 
		{
			SDL_RenderCopy(renderer, trailTexture, nullptr, &trailRect);
			++it;
		}
	}
}

Compass::Compass(SDL_Renderer* renderer, int spwanX, int spwanY)
{
	walkSpriteSheetPath = (char*)"assets\\walk\\Compas_no_Sheet-Sheet.gif";
	chargeSpriteSheetPath = (char*)"assets\\charge\\Compas-Sheet.gif";
	x = spwanX;
	y = spwanY;
	resistance = 4;
	damage = 20;
	speed = 15;
	walkTexture = loadTexture(walkSpriteSheetPath, renderer);
	chargeTexture = loadTexture(chargeSpriteSheetPath, renderer);
}

void Compass::charge(Uint32 currentTime, Uint32 chargeStartTime, Uint32 chargeDuration)
{
	// ���� �ð� ���� ����
	if (currentTime - chargeStartTime <= chargeDuration) {
		// ��� ��ġ���� ���� �������� �̵�
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

	// ������ �̵� ����
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

	currentFrame = (currentFrame + 1) % (rows * columns);

	// ���� ���� �߰�
	if (isCharging) {
		charge(currentTime, chargeStartTime, chargeDuration);
	}
}

void Compass::updateLastTarget(int targetX, int targetY) 
{
	lastTargetX = targetX;
	lastTargetY = targetY;
}