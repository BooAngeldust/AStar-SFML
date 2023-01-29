// AStar.cpp : Defines the entry point for the application.
//

#include <vector>
#include <SFML/Graphics.hpp>
#include "Grid.h"

int main()
{
	const int W_WIDTH = 1000;
	const int W_HEIGHT = 800;
	const int NODE_SIZE = 40;

	sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT), "AStar");
	window.setFramerateLimit(0);

	Grid grid(window, 0, 0, W_WIDTH / NODE_SIZE, W_HEIGHT / NODE_SIZE, NODE_SIZE);

	sf::Clock clock;
	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					grid.solve();
				}
			}
		}
		// update 
		if (window.hasFocus())
			grid.update(dt);

		// render
		grid.draw();
	}

	return 0;
}
