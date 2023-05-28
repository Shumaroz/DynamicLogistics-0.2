#pragma once

#ifndef MODULEDB_H
#define MODULEDB_H
#include "MyDLL.h"

#include <SFML/Graphics.hpp>
#include "framework.h"
#include <vector>
#include "Agent.h"
#include "Unit.h"
#include "MapGrid.h"

class MYDLL_API ModuleDB
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

#endif