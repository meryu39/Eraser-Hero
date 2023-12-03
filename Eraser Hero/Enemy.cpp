#include "Enemy.h"

void Enemy::charge(int targetX, int targetY)
{

}

Pencil::Pencil()
{
	resistance = 0;
	damage = 10;
	speed = 10;
	shape = { 0, 0 , 30, 100 };
}


Sharp::Sharp()
{
	resistance = 2;
	damage = 15;
	speed = 15;
	shape = { 0, 0 , 30, 100 };
}

Fountain::Fountain()
{
	resistance = 0;
	damage = 10;
	speed = 10;
	shape = { 0, 0 , 30, 100 };
}

Compass::Compass()
{
	resistance = 4;
	damage = 20;
	speed = 15;
	shape = { 0, 0 , 30, 100 };
}