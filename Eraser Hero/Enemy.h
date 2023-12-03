#pragma once
#include <iostream>
#include <SDL.h>

using namespace std;

class Enemy
{
public:
	void charge(int targetX, int targetY);
protected:
	int resistance; //적 유형에 따른 밀림저항
	int damage; //적이 플레이어에게 공격했을때 주는 데미지값
	int speed; //적의 이동속도
	SDL_Rect shape; //적의 모양
};
//연필
class Pencil : public Enemy
{
public:
	Pencil();
private:
};
//샤프
class Sharp : public Enemy
{
public:
	Sharp();
private:
};
//만년필
class Fountain : public Enemy
{
public:
	Fountain();
private:
	//만년필 잉크자국의 좌표
	int track[5][2];
};
//컴퍼스
class Compass : public Enemy
{
public:
	//컴퍼스용 돌진 함수 따로 구현
	void charge();
	Compass();
private:
};