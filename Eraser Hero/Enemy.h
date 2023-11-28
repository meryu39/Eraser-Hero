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
//����
class Sharp : public Enemy
{
public:
	Sharp();
private:
};
//������
class Fountain : public Enemy
{
public:
	Fountain();
private:
	//������ ��ũ�ڱ��� ��ǥ
	int track[5][2];
};
//���۽�
class Compass : public Enemy
{
public:
	//���۽��� ���� �Լ� ���� ����
	void charge();
	Compass();
private:
};