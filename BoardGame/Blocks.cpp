#include "pch.h"
#include "Blocks.h"

Blocks::Blocks()
{
	this->x = 50;
	this->y = 50;
	onPlayer1 = false;
	onPlayer2 = false;
	blockType = 0;
}

void Blocks::setCord(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Blocks::setBlockType(int blockType)
{
	this->blockType = blockType;
}

int Blocks::getX()
{
	return x;
}

int Blocks::getY()
{
	return y;
}

int Blocks::getBlockType()
{
	return blockType;
}
