#pragma once
class Player
{
	int x;
	int y;
	int i;

public:
	Player(); 

	void Player::setCord(int x, int y);
	void Player::SetI(int i);
	int Player::getX();
	int Player::getY();
	int Player::getI();
	BOOL Player_Turn = FALSE;
};

