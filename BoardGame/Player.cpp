#include "pch.h"
#include "Player.h"

Player :: Player() {
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

int Player::getI() {
	return i;
}
