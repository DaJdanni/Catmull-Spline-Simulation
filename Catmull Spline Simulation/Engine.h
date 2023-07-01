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

	std::vector<sf::Vector2f> mousePositions;

	// Engine Objects:
	std::vector<sf::CircleShape> Points;
	std::vector<sf::CircleShape> cPoints;
	sf::CircleShape Point;


	sf::Font TextFont;

	int numberOfPoints = 0;

	bool leftClickDB = false;
	bool rightClickDB = false;
	bool middleClickDB = false;

	void initalizeVariables();
	void initalizeWindow();
	void initalizePointConfig();

public:

	// Constructors / Destructors
	Engine();
	virtual ~Engine();

	// Accessors
	const bool running() const;

	void createCurve(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3);

	void updateMousePositions();
	void onLeftClick(sf::Vector2f mousePosition);
	void onRightClick();
	void onMiddleClick();

	void updateConsole();

	void drawPoint(sf::Vector2f position, sf::Vector2f scale = sf::Vector2f(1.f, 1.f), bool cPoint = false);
	void clearPoints();
	void clearcPoints();
	void renderPoints();
	void rendercPoints();

	void pollEvents();
	void update();
	void render();
};