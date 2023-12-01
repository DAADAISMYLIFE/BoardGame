#pragma once
class Blocks
{
	int x;
	int y;
	bool onPlayer1;
	bool onPlayer2;
	int blockType;
public:
	Blocks();
	void setCord(int x, int y);
	void setBlockType(int isSpecial);


	int getX();
	int getY();
	int getBlockType();
};

