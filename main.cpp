// AStar.cpp : Defines the entry point for the application.
//

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

static const sf::Time TimePerFrame = sf::seconds(1.f / 1200.f);

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "AStar");

	sf::Font font;
	font.loadFromFile("fonts/font.otf");

	Grid grid(window, 0, 0, 20, 20, 30);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	// Stats
	sf::Time statUpdateTime = sf::Time::Zero;
	int framesPassed = 0;
	sf::Text fpsText;
	fpsText.setFont(font);
	fpsText.setPosition(10, 10);
	fpsText.setFillColor(sf::Color::Blue);

	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;

		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			// handle events
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				else if (event.type == sf::Event::MouseButtonReleased)
					grid.solve();
			}
			// update 
			if (window.hasFocus())
				grid.update(dt.asSeconds());
		}

		// Draw FPS
		statUpdateTime += dt;
		framesPassed++;
		if (statUpdateTime > sf::seconds(1.f))
		{
			fpsText.setString("FPS: " + std::to_string(framesPassed));
			framesPassed = 0;
			statUpdateTime -= sf::seconds(1.f);
		}

		// render
		window.clear();
		window.draw(grid);
		window.draw(fpsText);
		window.display();
	}

	return 0;
}

void renderFPS(sf::Time dt)
{

}