#pragma once
#include <iostream>

class Weapon
{
protected:
	int damage;
	int gaugeBonus;
	//??? effect;
};

class Ruler : public Weapon
{
public:
	Ruler();
private:

};

class White : public Weapon
{
public:
	White();
private:

};

