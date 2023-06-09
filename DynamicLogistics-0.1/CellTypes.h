#pragma once

#define C_HUB sf::Color(255, 0, 0)
#define C_NODE sf::Color(0, 200, 0)
#define C_WALL sf::Color(30, 30, 30)
#define C_PASS sf::Color(255, 255, 255)
#define C_UNIT sf::Color(255, 200, 0)

enum CellType
{
	Pass,
	Wall,
	Hub,
	Node,
	ToHub,
	ToNode
};