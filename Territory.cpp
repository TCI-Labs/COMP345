#include "Territory.h"


Territory::Territory(void)
{
}


Territory::~Territory(void)
{
}

void Territory::setAdjacent(list<string> cAdjacent)
{
	adjacent = cAdjacent;
}

void Territory::setX(int cx)
{
	x = cx;
}

void Territory::setY(int cy)
{
	y = cy;
}

void Territory::setParentCont(string sParent)
{
	parentCont = sParent;
}

void Territory::setName(string sName)
{
	name = sName;
}

list<string> Territory::getAdjacent()
{
	return adjacent;
}

int Territory::getX()
{
	return x;
}

int Territory::getY()
{
	return y;
}

string Territory::getParentCont()
{
	return parentCont;
}

string Territory::getName()
{
	return name;
}