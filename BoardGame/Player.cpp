#include "pch.h"
#include "Player.h"

Player :: Player() {
	i = 0;
}

void Player::SetI(int i)
{
	this->i = i;
}

int Player::getI() {
	return i;
}
