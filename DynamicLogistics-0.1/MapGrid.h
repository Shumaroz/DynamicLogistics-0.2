#pragma once

/*#ifndef MAPGRID_H
#define MAPGRID_H*/
#include <SFML/Graphics.hpp>
#include "framework.h"
#include <vector>
#include "Cell.h"
#include "CellTypes.h"

class MapGrid
{
private:
	const int pass = 0, marker = 1;
	const int height = 200, width = 200;
	sf::Vector2f startPos = sf::Vector2f{ 0.f, 0.f };
	const sf::Vector2f finishPos = sf::Vector2f{ 650.f, 550.f };
	int destReached = 0;
	bool doCountDestReached = false;

	sf::RenderWindow& _window;
	std::vector<std::vector<Cell*>> map;
	sf::RectangleShape cellShape;
	sf::RenderTexture markerTexture;

	//std::vector <sf::CircleShape*> pointShapes;
	//std::vector<sf::RectangleShape*> markerShapeVec;

	int _windowSize, coef_x, coef_y;

public:
	bool hub_exists = false, node_exists = false;
	MapGrid(sf::RenderWindow& window);
	~MapGrid();
	sf::Vector2f getStartPos();
	void update(float dt);
	void render();
	void updateTexture(CellType type, sf::Vector2i pos);
	void renderMarker();

	void addMarker(sf::Vector2f pos, CellType phase, float relevance);
	bool isDestination(sf::Vector2f pos, CellType phase);
	short isWall(sf::Vector2f cur_pos, sf::Vector2f check_pos);
	int dirByMarkers(sf::Vector2f pos, CellType phase);

	void addPoint(CellType t, sf::Vector2f pos);
	void initWalls();
	void addWalls(sf::Vector2i pos);
	void addPass(sf::Vector2i pos);

	int getDestReached();
	void countDestReached(bool doCount);

	sf::Vector2f normalize(const sf::Vector2f& source);
};