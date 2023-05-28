#include "pch.h"
#include <SFML/Graphics.hpp>
#include "Unit.h"

#include <iostream>

#define PI 3.14159265

Unit::Unit(sf::RenderWindow& window) : _window(window)
{
	_windowSize = _window.getSize().x;
	this->unitShape.setRadius(4.f);
	this->position = sf::Vector2f{ 0.f, 0.f };
	this->startPosition = sf::Vector2f{ 0.f, 0.f };
	this->unitShape.setFillColor(C_UNIT);
	this->unitShape.setPosition(position);
	this->unitShape.setOutlineThickness(2.f);
	this->unitShape.setOutlineColor(sf::Color(0, 0, 0));

	phase = ToNode;
	active = false;
	returning = false;
}

Unit::~Unit()
{
	delete this;
}

void Unit::toDefault()
{
	active = false;
	returning = true;
	this->position = startPosition;
	this->unitShape.setPosition(position);

	this->velocity = sf::Vector2f{ 0.f, 0.f };
	this->desiredDirection = sf::Vector2f{ 0.f, 0.f };
	this->phase = ToNode;
}

void Unit::update(float dt, MapGrid& mapGrid)
{
	if (startPosition == sf::Vector2f(0.f, 0.f))
	{
		startPosition = mapGrid.getStartPos();
		position = startPosition;
	}
	if (!active)
		active = true;
	int markersDirection = mapGrid.dirByMarkers(position, phase);
	desiredDirection.x += (float)cos(markersDirection * PI / 180);
	desiredDirection.y += (float)sin(markersDirection * PI / 180);
	desiredDirection = mapGrid.normalize(desiredDirection);

	sf::Vector2f desiredVelocity = moveSpeed * desiredDirection;
	sf::Vector2f desiredSteeringForce = (desiredVelocity - velocity) * steerStrength;

	velocity += desiredSteeringForce * dt;

	sf::Vector2f check_pos = position + dt * velocity;
	// isWall: 0 - not wall, 1 - x hit, 2 - y hit
	short wallPos = mapGrid.isWall(position, check_pos);
	if (wallPos != 0 && !returning)
	{
		if (phase == ToNode)
			phase = ToHub;
		else
			phase = ToNode;
		returning = true;
	}
	while (wallPos != 0)
	{
		if (wallPos == 1 || wallPos == 3)
		{
			velocity.x = -velocity.x;
			desiredDirection.x = -desiredDirection.x;
		}
		if (wallPos == 2 || wallPos == 3)
		{
			velocity.y = -velocity.y;
			desiredDirection.y = -desiredDirection.y;
		}
		if (wallPos == -1)
		{
			toDefault();
			break;
		}
		check_pos = position + dt * velocity;
		wallPos = mapGrid.isWall(position, check_pos);
	}

	position += dt * velocity;
	unitShape.setPosition(position);

	if (mapGrid.isDestination(position, phase))
	{
		if (returning)
		{
			active = false;
			returning = false;
		}
		else
		{
			velocity = -velocity;
			if (phase == ToNode)
				phase = ToHub;
			else
				phase = ToNode;
		}
	}
}

void Unit::render()
{
	_window.draw(unitShape);
}

bool Unit::isActive()
{
	return active;
}