// AStar.cpp : Defines the entry point for the application.
//

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600


int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "AStar",sf::Style::Close);
	window.setFramerateLimit(0);

	Grid grid(window, 0, 0, 5, 5, 120);

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
			
		}
		// update 
		if (window.hasFocus())
			grid.update(dt);
		// render
		window.clear();
		window.draw(grid);
		window.display();
	}

	return 0;
}
