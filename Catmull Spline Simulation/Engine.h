#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>

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

	std::vector<sf::Vector2i> mousePositions;

	sf::Font TextFont;

	int numberOfPoints = 0;

	bool leftClickDB = false;
	bool rightClickDB = false;

	void initalizeVariables();
	void initalizeWindow();
	void onLeftClick(sf::Vector2i mousePosition);
	void onRightClick();
	void clearPoints();

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