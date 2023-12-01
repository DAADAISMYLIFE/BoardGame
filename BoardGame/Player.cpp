#include "pch.h"
#include "Player.h"

Player :: Player() {
	x = 0;
	y = 0;
	i = 0;
}

void Player::setCord(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Player::SetI(int i)
{
	this->i = i;
}

int Player::getX()
{
	return x;
}

int Player::getY()
{
	return y;
}

int Player::getI() {
	return i;
}
