#include "MapGrid.h"
#include <iostream>

MapGrid::MapGrid(sf::RenderWindow& window) : _window(window)
{
	_windowSize = _window.getSize().x;
	coef_x = _windowSize / width;
	coef_y = _windowSize / height;

	map.resize(height);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			map[i].push_back(new Cell(sf::Vector2i(i * coef_x, j * coef_y)));
		}
	cellShape.setSize(sf::Vector2f((float)coef_x, (float)coef_y));

	if (!markerTexture.create(_windowSize, _windowSize))
		std::cout << "BAD";

	markerTexture.display();

	initWalls();
}

MapGrid::~MapGrid()
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			delete map[i][j];
		}
}

sf::Vector2f MapGrid::getStartPos() { return startPos; }

void MapGrid::update(float dt)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			if (!map[i][j]->decreaseIntensity(dt))
				updateTexture(Pass, map[i][j]->getPos());
		}
	//map[80][80]->debugParams();

	// Left Mouse Click
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(_window);
		addWalls(mouse_pos);
	}
	
	// Right Mouse Click
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(_window);
		addPass(mouse_pos);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H) && !hub_exists)
	{
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(_window);
		addPoint(Hub, sf::Vector2f(mouse_pos));
		hub_exists = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && !node_exists)
	{
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(_window);
		addPoint(Node, sf::Vector2f(mouse_pos));
		node_exists = true;
	}
}

void MapGrid::render()
{
	markerTexture.display();
	_window.draw(sf::Sprite(markerTexture.getTexture()));
}

void MapGrid::updateTexture(CellType type, sf::Vector2i pos)
{
	switch (type)
	{
	case Pass:
		cellShape.setFillColor(C_PASS);
		break;
	case ToHub:
		cellShape.setFillColor(sf::Color(100, 100, 100, 10));
		break;
	case ToNode:
		cellShape.setFillColor(sf::Color(100, 100, 100, 10));
		break;
	case Hub:
		cellShape.setFillColor(C_HUB);
		break;
	case Node:
		cellShape.setFillColor(C_NODE);
		break;
	case Wall:
		cellShape.setFillColor(C_WALL);
		break;
	default:
		cellShape.setFillColor(C_PASS);
		break;
	}
	cellShape.setPosition((float)pos.x, (float)pos.y);
	markerTexture.draw(cellShape, sf::BlendAlpha);
}

void MapGrid::addMarker(sf::Vector2f pos, CellType phase, float relevance)
{
	int x = (int)pos.x / coef_x;
	int y = (int)pos.y / coef_y;
	
	if (phase == ToNode)
		map[x][y]->changeCell(ToHub, relevance);
	else
		map[x][y]->changeCell(ToNode, relevance);

	updateTexture(map[x][y]->getType(), map[x][y]->getPos());
}

bool MapGrid::isDestination(sf::Vector2f pos, CellType phase)
{
	int x = (int)pos.x / coef_x;
	int y = (int)pos.y / coef_y;
	if ((phase == ToNode && map[x][y]->getType() == Node) || (phase == ToHub && map[x][y]->getType() == Hub))
	{
		if (doCountDestReached)
			destReached++;
		return true;
	}
	else
		return false;
}

short MapGrid::isWall(sf::Vector2f cur_pos, sf::Vector2f check_pos)
{
	int ch_x = (int)check_pos.x / coef_x;
	int ch_y = (int)check_pos.y / coef_y;
	int cur_x = (int)cur_pos.x / coef_x;
	int cur_y = (int)cur_pos.y / coef_y;
	short ret = 0;

	if (map[cur_x][cur_y]->getType() == Wall)
		return -1;
	if (map[ch_x][ch_y]->getType() != Wall)
		return ret;
	else
	{
		if (map[ch_x][cur_y]->getType() == Wall)
			ret += 1;
		if (map[cur_x][ch_y]->getType() == Wall)
			ret += 2;
		if (map[ch_x][cur_y]->getType() != Wall && map[cur_x][ch_y]->getType() != Wall)
			ret = 3;
	}

	return ret;
}

int MapGrid::dirByMarkers(sf::Vector2f pos, CellType phase)
{
	int x = (int)pos.x / coef_x;
	int y = (int)pos.y / coef_y;
	float maxIntensity = 0.f;
	int intenseDir = -1;
	int k = 0, kx = 0, ky = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (map[x + j][y + i]->getIntensity(phase) > maxIntensity && map[x + j][y + i]->getIntensity(phase) != 0.f)
			{
				maxIntensity = map[x + j][y + i]->getIntensity(phase);
				kx = x + j;
				ky = y + i;
				if (k < 4)
					intenseDir = -45 * i + 45 * j - 135;
				else if (k > 4)
					intenseDir = 45 * i - 45 * j + 45;
			}
			k++;
		}
	}

	return intenseDir;
}

void MapGrid::addPoint(CellType t, sf::Vector2f pos)
{
	int x = (int)pos.x / coef_x;
	int y = (int)pos.y / coef_y;
	for (int i = -2; i <= 2; i++)
		for (int j = -2; j <= 2; j++)
		{
			map[x + j][y + i]->changeCell(t, 0);
			updateTexture(t, map[x + j][y + i]->getPos());
		}

	if (t == Hub)
		startPos = sf::Vector2f(x * coef_x, y * coef_y);
}

void MapGrid::initWalls()
{
	for (int i = 0; i < map.size(); i++)
	{
		map[i][0]->changeCell(Wall, 0);
		map[0][i]->changeCell(Wall, 0);
		map[i][map.size() - 1]->changeCell(Wall, 0);
		map[map.size() - 1][i]->changeCell(Wall, 0);
		updateTexture(Wall, map[i][0]->getPos());
		updateTexture(Wall, map[0][i]->getPos());
		updateTexture(Wall, map[i][map.size() - 1]->getPos());
		updateTexture(Wall, map[map.size() - 1][i]->getPos());
	}
}

void MapGrid::addWalls(sf::Vector2i pos)
{
	int x = (int)pos.x / coef_x;
	int y = (int)pos.y / coef_y;
	if (x > 2 && y > 2 && x < map.size()-2 && y < map.size()-2)
	{
		for (int i = -2; i <= 2; i++)
			for (int j = -2; j <= 2; j++)
			{
				map[x + j][y + i]->changeCell(Wall, 0);
				updateTexture(Wall, map[x + j][y + i]->getPos());
			}
	}
}

void MapGrid::addPass(sf::Vector2i pos)
{
	int x = (int)pos.x / coef_x;
	int y = (int)pos.y / coef_y;
	if (x > 2 && y > 2 && x < map.size() - 2 && y < map.size() - 2)
	{
		for (int i = -2; i <= 2; i++)
			for (int j = -2; j <= 2; j++)
			{
				map[x + j][y + i]->changeCell(Pass, 0);
				updateTexture(Pass, map[x + j][y + i]->getPos());
			}
	}
}

int MapGrid::getDestReached()
{
	return destReached;
}

void MapGrid::countDestReached(bool doCount)
{
	doCountDestReached = doCount;
	if (!doCount)
		destReached = 0;
}

sf::Vector2f MapGrid::normalize(const sf::Vector2f& source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}