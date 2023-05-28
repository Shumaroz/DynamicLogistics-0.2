#include "pch.h"
#include "ModuleDB.h"

ModuleDB::ModuleDB(sf::RenderWindow& window) : _window(window)
{
	mapGrid = new MapGrid(window);
	unit = new Unit(window);
}

ModuleDB::~ModuleDB()
{
	delete mapGrid;
	delete unit;
}

void ModuleDB::createAgents(int count)
{
	for (int i = 0; i < count; i++)
	{
		agentsVec.push_back(new Agent(_window));
	}
}

void ModuleDB::deleteAgents()
{
	for (int i = 0; i < agentsVec.size(); i++)
		delete agentsVec[i];
	agentsVec.clear();
}

void ModuleDB::update(float dt)
{
	mapGrid->update(dt);

	if (mapGrid->hub_exists)
	{
		for (int i = 0; i < agentsVec.size(); i++)
			agentsVec[i]->update(dt, *mapGrid);
	}

	isUnitsActive = unit->isActive();
	if (isUnitsActive)
	{
		unit->update(dt, *mapGrid);
	}
	else
	{
		if (mapGrid->getDestReached() == 0)
			mapGrid->countDestReached(true);
		else if (mapGrid->getDestReached() >= 100)
		{
			isUnitsActive = true;
			mapGrid->countDestReached(false);
			unit->update(dt, *mapGrid);
		}
	}
}

void ModuleDB::render()
{
	mapGrid->render();

	if (mapGrid->hub_exists)
	{
		for (int i = 0; i < agentsVec.size(); i++)
			agentsVec[i]->render();
	}

	if (isUnitsActive)
		unit->render();
}