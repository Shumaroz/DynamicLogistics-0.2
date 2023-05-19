#include "Cell.h"
#include <iostream>

Cell::Cell()
{
	type = new CellType[2]{ Pass, Pass };
	intensity = new float[2]{ 0.f, 0.f };
}

Cell::Cell(sf::Vector2i pos)
{
	type = new CellType[2]{ Pass, Pass };
	intensity = new float[2]{ 0.f, 0.f };
	position = pos;
}

Cell::~Cell()
{
	delete type;
	delete intensity;
}

void Cell::changeCell(CellType t, float relevance)
{
	if (permanent && t != Pass)
		return;
	else if (permanent && t == Pass)
		permanent = false;
	if (t == Hub || t == Node || t == Wall)
		permanent = true;

	switch (t)
	{
	case ToHub:
		type[0] = t;
		if (relevance > intensity[0])
			intensity[0] = relevance;
		break;
	case ToNode:
		type[1] = t;
		if (relevance > intensity[1])
			intensity[1] = relevance;
		break;
	case Hub:
		type[0] = Hub;
		type[1] = Pass;
		intensity[0] = 100.f;
		intensity[1] = 0.f;
		break;
	case Node:
		type[0] = Node;
		type[1] = Pass;
		intensity[0] = 0.f;
		intensity[1] = 100.f;
		break;
	case Wall:
		type[0] = Wall;
		type[1] = Wall;
		intensity[0] = 0.f;
		intensity[1] = 0.f;
	case Pass:
		type[0] = Pass;
		type[1] = Pass;
		intensity[0] = 0.f;
		intensity[1] = 0.f;
	default:
		type[0] = t;
		type[1] = Pass;
		intensity[0] = 0.f;
		intensity[1] = 0.f;
		break;
	}
}

bool Cell::decreaseIntensity(float dt)
{
	if (!permanent)
	{
		if (intensity[0] > 0)
			intensity[0] -= dt;
		if (intensity[1] > 0)
			intensity[1] -= dt;
		if (type[0] == ToHub && intensity[0] < 0.02f)
		{
			intensity[0] = 0.f;
			type[0] = Pass;
			return false;
		}
		if (type[1] == ToNode && intensity[1] < 0.02)
		{
			intensity[1] = 0.f;
			type[1] = Pass;
		}
	}
	return true;
	//std::cout << intensity << '\n';
}

sf::Vector2i Cell::getPos()
{
	return position;
}

CellType Cell::getType()
{
	return type[0];
}

CellType Cell::getType(CellType t)
{
	if (t == ToNode)
		return type[1];
	else
		return type[0];
}

float Cell::getIntensity(CellType t)
{
	if (t == ToNode)
		return intensity[1];
	else
		return intensity[0];
}


void Cell::debugParams()
{
	std::cout << position.x << ", " << position.y << " | " << intensity << ", " << type << " | " << d_param << '\n';
}