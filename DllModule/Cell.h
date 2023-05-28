#pragma once

#ifndef CELL_H
#define CELL_H
#include "MyDLL.h"

#include <SFML/Graphics.hpp>
#include "framework.h"
#include "CellTypes.h"

class MYDLL_API Cell
{
private:
	int d_param = 0;
	CellType* type;
	float* intensity;
	sf::Vector2i position;
	bool permanent = false;

public:
	Cell();
	Cell(sf::Vector2i pos);
	~Cell();
	void changeCell(CellType t, float relevance);
	bool decreaseIntensity(float dt);
	sf::Vector2i getPos();
	CellType getType();
	CellType getType(CellType t);
	float getIntensity(CellType t);

	void debugParams();
};

#endif