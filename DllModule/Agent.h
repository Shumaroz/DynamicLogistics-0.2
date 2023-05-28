#pragma once

#ifndef AGENT_H
#define AGENT_H
#include "MyDLL.h"

#include <SFML/Graphics.hpp>
#include "framework.h"
#include "CellTypes.h"
#include "MapGrid.h"

class MYDLL_API Agent
{
private:
	sf::RenderWindow& _window;
	sf::CircleShape agentShape;
	sf::Vector2f desiredDirection;
	sf::Vector2f velocity;
	sf::Vector2f position, startPosition;

	CellType phase;
	float relevanceTime = 30.f;

	int _windowSize;
	float wanderStrength = 0.1f;
	float steerStrength = 3.f;
	float moveSpeed = 140.f;
	//float cooldown = 0.f;

public:
	Agent(sf::RenderWindow& window);
	~Agent();
	void toDefault();
	void update(float dt, MapGrid& mapGrid);
	void render();
	sf::Vector2f getPos();
};

#endif