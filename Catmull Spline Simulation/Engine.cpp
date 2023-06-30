#include "Engine.h"
// Initalizers:

void Engine::initalizeVariables() {
	this->window = nullptr;
	this->TextFont.loadFromFile("../Resource Files/TimesNewRoman.tff");
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
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			this->onLeftClick(this->mouseWindowPosition);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			this->onRightClick();
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
	/*std::cout << "Current Mouse Position (relative to window): " <<
		this->mouseWindowPosition.x << " "
		<< this->mouseWindowPosition.y << std::endl;*/

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

// Catmull Logic:

sf::Vector2f resultant(sf::Vector2i p, sf::Vector2i q) {
	return static_cast<sf::Vector2f>(p) - static_cast<sf::Vector2f>(q);
}

double toMagnitude(sf::Vector2i p, sf::Vector2i q) {
	return (sqrt(
		pow(abs((p.x - q.x)), 2.f) +
		pow(abs((p.y - q.y)), 2.f)
	));
}

sf::Vector2i formula(sf::Vector2i p0, sf::Vector2i p1, sf::Vector2i p2, sf::Vector2i p3, float t) {

	// you'll notice a lot of static casts usage but basically:
	// floating vectors are needed since they can be manipulated by arthimetic stuff or something like that
	// unlike Vector2i (integer vector), I can actually multiply a vector by its magnitude
	// so thats what all those static casts are doing
	// a lot of this math comes from online sources and my friend Doctor Cringe (thanks) so i'm not really smart
	// just rewriting lua into c++ (but horribly)

	float alpha = 0.5;
	float tension = 0.f;

	float t01 = static_cast<float>(pow(toMagnitude(p1, p0), alpha));
	float t12 = static_cast<float>(pow(toMagnitude(p1, p2), alpha));
	float t23 = static_cast<float>(pow(toMagnitude(p2, p3), alpha));

	sf::Vector2f m1 = (1.0f - tension) * (resultant(p2, p1) + t12 * (((resultant(p1, p0) / t01) - resultant(p2, p0)) / (t01 + t12)));
	sf::Vector2f m2 = (1.0f - tension) * (resultant(p2, p1) + t12 * (((resultant(p3, p2) / t23) - resultant(p3, p1)) / (t12 + t23)));

	sf::Vector2f c0 = 2.0f * resultant(p1, p2) + m1 + m2;
	sf::Vector2f c1 = -3.0f * resultant(p1, p2) - (2.0f * m1) - m2;
	sf::Vector2f c2 = m1;
	sf::Vector2f c3 = static_cast<sf::Vector2f>(p1); // convert integer vector to floating vector for manipulation

	sf::Vector2f point = c0 * static_cast<float>(pow(t, 3)) + c1 * static_cast<float>(pow(t, 2)) + c2 * t + c3;

	return static_cast<sf::Vector2i>(point);
}

void Engine::onLeftClick(sf::Vector2i mousePosition) {

	if (this->leftClickDB == true) { return; }

	this->leftClickDB = true;

	this->numberOfPoints += 1;

	this->mousePositions.push_back(mousePosition);

	std::cout << "Added a point..." << std::endl;

	std::chrono::milliseconds dura(250);

	std::this_thread::sleep_for(dura);

	this->leftClickDB = false;
}

void Engine::onRightClick() {

	if (this->rightClickDB == true) { return; }

	this->rightClickDB = true;

	size_t size = this->mousePositions.size();

	std::cout << size << std::endl;

	if (size >= 2) {
		sf::Vector2i p0 = sf::Vector2i(this->mousePositions.front().x - 50, this->mousePositions.front().y);
		sf::Vector2i p3 = sf::Vector2i(this->mousePositions.back().x + 50, this->mousePositions.back().y);
	}

	std::chrono::milliseconds dura(250);

	std::this_thread::sleep_for(dura);

	this->rightClickDB = false;
}