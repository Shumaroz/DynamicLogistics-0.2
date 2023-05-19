#pragma once

/*#ifndef UNIT_H
#define UNIT_H*/
#include <SFML/Graphics.hpp>
#include "framework.h"
#include "CellTypes.h"
#include "MapGrid.h"

class Unit
{
private:
	sf::RenderWindow& _window;
	sf::CircleShape unitShape;
	sf::Vector2f desiredDirection;
	sf::Vector2f velocity;
	sf::Vector2f position, startPosition;

	CellType phase;
	bool active = false, returning = false;

	int _windowSize;
	float steerStrength = 3.f;
	float moveSpeed = 140.f;

public:
	Unit(sf::RenderWindow& window);
	~Unit();
	void toDefault();
	void update(float dt, MapGrid& mapGrid);
	void render();
	bool isActive();
};