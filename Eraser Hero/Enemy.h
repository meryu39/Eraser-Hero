#pragma once
#include <iostream>
#include <cstring>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

class Enemy
{
public:
	Enemy();
	~Enemy();
	int getDamage() { return damage; }
	int getSpeed() { return speed; }
	int getX() { return x; }
	int getY() { return y; }
	int getWidth() { return width; }
	int getHeight() { return height; }

	void setDamage(int newDamage) { damage = newDamage; }
	void setSpeed(int newSpeed) { speed = newSpeed; }
	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }

	void charge(int targetX, int targetY);
	void update(Uint32 lastUpdateTime, Uint32 updateInterval);
	void render(SDL_Renderer* renderer);
protected:
	int resistance; //적 유형에 따른 밀림저항
	int damage; //적이 플레이어에게 공격했을때 주는 데미지값
	int speed; //적의 이동속도
	int x, y; //좌표
	int directionX, directionY; // 이동할 방향
	int width, height;  // 테두리의 폭과 높이
	int rows, columns; //스프라이트의 행, 열
	int currentFrame; //현재 프레임
	SDL_Texture* walkTexture; //걷기 모션
	SDL_Texture* chargeTexture; //돌진 모션
	char* walkSpriteSheetPath; //걷기 모션의 경로
	char* chargeSpriteSheetPath; //돌진 모션의 경로
	bool isCharging; //돌진하고 있는지

	SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
};

//연필
class Pencil : public Enemy
{
public:
	Pencil(SDL_Renderer* renderer, int spwanX, int spwanY);
private:
};
//샤프
class Sharp : public Enemy
{
public:
	Sharp(SDL_Renderer* renderer, int spwanX, int spwanY);
private:
};
//만년필
class Fountain : public Enemy
{
public:
	Fountain(SDL_Renderer* renderer, int spwanX, int spwanY);
	Uint32 lastFootprintTime;
	Uint32 footprintInterval; // 자국을 남기는 간격
	void loadTrailTexture(SDL_Renderer* renderer); // 자국 이미지를 로드하는 함수
	void updateTrail(); // 자국을 업데이트하는 함수 추가
	void charge(int targetX, int targetY); // 돌진 함수를 오버라이드하여 자국 업데이트 포함
	void render(SDL_Renderer* renderer);  // 렌더 함수를 오버라이드하여 자국도 렌더링

private:
	//만년필 잉크자국의 좌표
	struct TrailPoint {
		int x;
		int y;
		Uint32 createTime;  // 자국 생성 시간 기록
	};
	vector<TrailPoint> trail;
	SDL_Texture* trailTexture;  // 자국 이미지 텍스처
	int trailLastTime;
	int trailUpdateTime;
	int trailWidth;
	int trailHeight;
};
//컴퍼스
class Compass : public Enemy
{
public:
	//컴퍼스용 돌진 함수 따로 구현 돌진전에 compassTargetX, Y 지정필요
	void charge(Uint32 currentTime, Uint32 chargeStartTime, Uint32 chargeDuration);
	void update(Uint32 lastUpdateTime, Uint32 updateInterval);
	Compass(SDL_Renderer* renderer, int spwanX, int spwanY);
	void updateLastTarget(int targetX, int targetY);
private:
	int chargeStartTime;
	int chargeDuration;
	int compassTargetX;
	int compassTargetY;
	int lastTargetX;  // 마지막으로 인식한 대상의 X 좌표
	int lastTargetY;  // 마지막으로 인식한 대상의 Y 좌표
};