#pragma once

#include <iostream>
#include <vector>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Engine {

	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	sf::Vector2i mouseWindowPosition;
	sf::Vector2i mouseScreenPosition;
	sf::Vector2f mousePosView;

	void initalizeVariables();
	void initalizeWindow();

public:

	// Constructors / Destructors
	Engine();
	virtual ~Engine();

	// Accessors
	const bool running() const;

	void updateConsole();
	void updateMousePositions();

	void pollEvents();
	void update();
	void render();
};