#include <SFML/Graphics.hpp>
#include "Agent.h"

#include <iostream>

#define PI 3.14159265

Agent::Agent(sf::RenderWindow& window) : _window(window)
{
	_windowSize = _window.getSize().x;
	this->agentShape.setRadius(2.f);
	startPosition = sf::Vector2f{ 0.f, 0.f };
	this->position = startPosition;
	this->agentShape.setFillColor(C_HUB);
	this->agentShape.setPosition(position);

	phase = ToNode;
}

Agent::~Agent() 
{
	delete this;
}

void Agent::toDefault()
{
	this->position = startPosition;
	this->agentShape.setFillColor(C_HUB);
	this->agentShape.setPosition(position);

	this->velocity = sf::Vector2f{ 0.f, 0.f };
	this->desiredDirection = sf::Vector2f{ 0.f, 0.f };
	this->phase = ToNode;
}

void Agent::update(float dt, MapGrid& mapGrid)
{
	//if (cooldown > 0.f)
	//	cooldown -= dt;
	//else
	//{
	if (startPosition == sf::Vector2f{ 0.f, 0.f })
	{
		startPosition = mapGrid.getStartPos();
		position = startPosition;
	}

		int markersDirection = mapGrid.dirByMarkers(position, phase);
		if (markersDirection != -1 && (rand() % 100) < 90)
		{
			//cooldown = 0.5f;
			desiredDirection.x += (float)cos(markersDirection * PI / 180);
			desiredDirection.y += (float)sin(markersDirection * PI / 180);
			//std::cout << markersDirection.x << " " << markersDirection.y << '\n';
		}
		else
		{
			desiredDirection.x += (float)cos(rand() % 360 * PI / 180) * wanderStrength;
			desiredDirection.y += (float)sin(rand() % 360 * PI / 180) * wanderStrength;
		}
		desiredDirection = mapGrid.normalize(desiredDirection);
	//}

	sf::Vector2f desiredVelocity = moveSpeed * desiredDirection;
	sf::Vector2f desiredSteeringForce = (desiredVelocity - velocity) * steerStrength;

	velocity += desiredSteeringForce * dt;

	sf::Vector2f check_pos = position + dt * velocity;
	// isWall: 0 - not wall, 1 - x hit, 2 - y hit
	short wallPos = mapGrid.isWall(position, check_pos);
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
	agentShape.setPosition(position);

	if (mapGrid.isDestination(position, phase))
	{
		relevanceTime = 50.f;
		velocity = -velocity;
		if (phase == ToNode)
		{
			phase = ToHub;
			this->agentShape.setFillColor(C_NODE);
		}
		else
		{
			phase = ToNode;
			this->agentShape.setFillColor(C_HUB);
		}
	}

	mapGrid.addMarker(position, phase, relevanceTime);
	if (relevanceTime > 0)
		relevanceTime -= 2*dt;
	else
		relevanceTime = 0;
}

void Agent::render()
{
	_window.draw(agentShape);
}

sf::Vector2f Agent::getPos()
{
	return position;
}