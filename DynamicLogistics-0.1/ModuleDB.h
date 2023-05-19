#pragma once

#include <SFML/Graphics.hpp>
#include "framework.h"
#include <vector>
#include "Agent.h"
#include "Unit.h"
#include "MapGrid.h"

class ModuleDB
{
private:
	std::vector<Agent*> agentsVec;
	sf::RenderWindow& _window;
	MapGrid* mapGrid;
	Unit* unit;
	bool isUnitsActive = false;

public:
	ModuleDB(sf::RenderWindow& window);
	~ModuleDB();
	void createAgents(int count);
	void deleteAgents();
	void update(float dt);
	void render();
};