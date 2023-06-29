#include "Engine.h"

// Initalizers:

void Engine::initalizeVariables() {
	this->window = nullptr;
	// Init variables used in program:
}

void Engine::initalizeWindow() {
	// video settings
	this->videoMode.width = 800;
	this->videoMode.height = 600;
	//this->videoMode.getDesktopMode() gets window resolution

	this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
}


// SFML Stuff:

Engine::Engine() {
	this->initalizeVariables();
	this->initalizeWindow();
}

Engine::~Engine() {
	delete this->window;
}

// Accessors:

const bool Engine::running() const {
	return this->window->isOpen();
}

void Engine::pollEvents() {
	// Event polling
	while (this->window->pollEvent(this->ev)) {
		switch (this->ev.type) {
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Engine::update() { // Game Logic

	// Debugger:
	this->updateConsole();

	// Game logic:
	this->pollEvents();

	this->updateMousePositions();
}

void Engine::render() { // Rendering

	/*
		@ return void
		- Clear old frame
		- Render objects
		- Display frame in window
		Render the game objects.
	*/

	this->window->clear(sf::Color(0, 0, 0, 255));

	// Draw game objects

	this->window->display();
}


// Updaters:

void Engine::updateConsole() {
	std::cout << "Current Mouse Position (relative to window): " <<
		this->mouseWindowPosition.x << " "
		<< this->mouseWindowPosition.y << std::endl;

	/*std::cout << "Current Mouse Position (relative to screen): " <<
		this->mouseScreenPosition.x << " "
		<< this->mouseScreenPosition.y << std::endl;
	*/

}

void Engine::updateMousePositions() {
	/*
		@ return void
		updates the mouse positions (Vector2i):
			- relative to window
			- relative to screen
	*/

	this->mouseWindowPosition = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mouseWindowPosition);
	this->mouseScreenPosition = sf::Mouse::getPosition();
}