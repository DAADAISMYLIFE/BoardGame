#pragma once
class Player
{
	int x;
	int y;
	int i;

public:
	Player(); 

	void setCord(int x, int y);
	void SetI(int i);
	int getI();
	BOOL Player_Turn = FALSE;
};

