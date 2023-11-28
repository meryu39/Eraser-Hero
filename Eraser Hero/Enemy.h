#pragma once
#include <iostream>
#include <SDL.h>

using namespace std;

class Enemy
{
public:
	void charge();
protected:
	int HP;
	int damage;
	int speed;
	SDL_Rect shape;
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