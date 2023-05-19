#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include "ModuleDB.h"

class FPS
{
private:
	unsigned int mFrame;
	unsigned int mFps, old_mFps;
	sf::Clock mClock;

public:
	FPS() : mFrame(0), mFps(0), old_mFps(0) {}

	void getFPS()
	{
		if (mFps != old_mFps)
		{
			old_mFps = mFps;
			std::cout << mFps << " ";
		}
	}

	void update()
	{
		if (mClock.getElapsedTime().asSeconds() >= 1.f)
		{
			mFps = mFrame;
			mFrame = 0;
			mClock.restart();
		}

		++mFrame;
	}
};

int main()
{
	const float dt = 0.016f;
	FPS fps;
	const int windowSize = 800;
	int agentCount = 200;
	sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Dynamic Logistics");
	window.setFramerateLimit(60);
	srand(time(NULL));
	
	ModuleDB* m_db = new ModuleDB(window);

	m_db->createAgents(agentCount);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(C_PASS);
		
		m_db->update(dt);
		m_db->render();

		fps.update();
		fps.getFPS();
		
		window.display();
	}

	m_db->deleteAgents();

	delete m_db;

	return 0;
}