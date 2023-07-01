#include "Engine.h"
// Initalizers:

void Engine::initalizeVariables() {
	this->window = nullptr;
	this->TextFont.loadFromFile("Resource Files/TimesNewRoman.tff");
}

void Engine::initalizeWindow() {
	// video settings
	this->videoMode.width = 1200;
	this->videoMode.height = 700;
	//this->videoMode.getDesktopMode() gets window resolution

	this->window = new sf::RenderWindow(this->videoMode, "Catmull Splines", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(144);
}

void Engine::initalizePointConfig() {
	const float size = 7.5f;
	this->Point = sf::CircleShape(size);
	this->Point.setPosition(50.f, 50.f);
	this->Point.setScale(sf::Vector2f(2.f, 2.f));
	this->Point.setOrigin(size, size);
}

// SFML Stuff:

Engine::Engine() {
	this->initalizeVariables();
	this->initalizeWindow();
	this->initalizePointConfig();
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
			this->onLeftClick(this->mousePosView);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			this->onRightClick();
		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
			this->onMiddleClick();
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

	this->renderPoints();
	this->rendercPoints();

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

void Engine::drawPoint(sf::Vector2f position, sf::Vector2f scale, bool cPoint) {
	this->Point.setPosition(position);
	this->Point.setScale(scale);
	this->Point.setFillColor(sf::Color(255, 255, 255));

	if (cPoint) { 
		this->cPoints.push_back(this->Point); 
	}
	else {
		this->Points.push_back(this->Point);
	}
}

void Engine::rendercPoints() {
	for (auto& i : this->cPoints) {
		this->window->draw(i);
	}
}

void Engine::renderPoints() { // Rendering all the points
	for (auto& i : this->Points) {
		this->window->draw(i);
	}
}

void Engine::clearcPoints() {
	this->cPoints.clear();
}

void Engine::clearPoints() {
	this->Points.clear();
}

// Catmull Logic:

sf::Vector2f resultant(sf::Vector2f p, sf::Vector2f q) {
	return p - q;
}

double toMagnitude(sf::Vector2f p, sf::Vector2f q) {
	return (sqrt(
		pow(abs((p.x - q.x)), 2.f) +
		pow(abs((p.y - q.y)), 2.f)
	));
}

sf::Vector2f formula(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, float t) {

	// you'll notice a lot of static casts usage but basically:
	// floating vectors are needed since they can be manipulated by arthimetic stuff or something like that
	// unlike Vector2i (integer vector), I can actually multiply a vector by its magnitude
	// so thats what all those static casts are doing
	// a lot of this math comes from online sources and my friend Doctor Cringe (thanks) so i'm not really smart
	// just rewriting lua into c++ (but horribly)

	float alpha = 0.5f;
	float tension = 0.f;

	float t01 = static_cast<float>(pow(toMagnitude(p1, p0), alpha));
	float t12 = static_cast<float>(pow(toMagnitude(p1, p2), alpha));
	float t23 = static_cast<float>(pow(toMagnitude(p2, p3), alpha));

	sf::Vector2f m1 = (1.0f - tension) * (resultant(p2, p1) + t12 * (resultant(p1, p0) / t01 - resultant(p2, p0) / (t01 + t12)));
	sf::Vector2f m2 = (1.0f - tension) * (resultant(p2, p1) + t12 * (resultant(p3, p2) / t23 - resultant(p3, p1) / (t12 + t23)));

	sf::Vector2f c0 = 2.0f * resultant(p1, p2) + m1 + m2;
	sf::Vector2f c1 = -3.0f * resultant(p1, p2) - m1 - m1 - m2;
	sf::Vector2f c2 = m1;
	sf::Vector2f c3 = p1;

	sf::Vector2f point = c0 * t * t * t + c1 * t * t + c2 * t + c3;

	return point;
}

void Engine::createCurve(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3) {
	for (float i = 0; i <= 1; i += 0.05f) {
		sf::Vector2f point = formula(p0, p1, p2, p3, i);

		this->drawPoint(point, sf::Vector2f(0.25f, 0.25f), true);
	}
}

void Engine::onLeftClick(sf::Vector2f mousePosition) {

	if ((this->leftClickDB == true) || (this->rightClickDB == true) || (this->middleClickDB == true)) { return; }

	this->leftClickDB = true;

	this->numberOfPoints += 1;

	std::cout << mousePosition.x << "|" << mousePosition.y << std::endl;

	this->drawPoint(mousePosition); // draw the point on the screen
	this->mousePositions.push_back(mousePosition); // save the point in vector

	std::cout << "Added a point..." << std::endl;

	std::chrono::milliseconds dura(150);

	std::this_thread::sleep_for(dura);

	this->leftClickDB = false;
}

void Engine::onMiddleClick() {

	if ((this->leftClickDB == true) || (this->rightClickDB == true) || (this->middleClickDB == true)) { return; }

	this->middleClickDB = true;

	this->numberOfPoints = 0;
	this->mousePositions.clear();
	this->clearPoints();
	this->clearcPoints();

	std::chrono::milliseconds dura(150);

	std::this_thread::sleep_for(dura);

	this->middleClickDB = false;

}

void Engine::onRightClick() {

	if ((this->leftClickDB == true) || (this->rightClickDB == true) || (this->middleClickDB == true)) { return; }

	this->rightClickDB = true;

	size_t size = this->mousePositions.size();

	std::cout << "# of Mouse positions: " << size << std::endl;

	if (size >= 2) {

		this->clearcPoints();

		sf::Vector2f p0 = sf::Vector2f(this->mousePositions.front().x - 50.f, this->mousePositions.front().y);
		sf::Vector2f p3 = sf::Vector2f(this->mousePositions.back().x + 50.f, this->mousePositions.back().y);

		for (size_t i = 0; i < (size - 1); i++) {
			if (((i - 1) == -1) && ((i + 2) == size)) {
				std::cout << "This vector only has 2 indexes." << std::endl;
				this->createCurve(p0, this->mousePositions.at(i), this->mousePositions.at(i + 1), p3);
			} 
			else if ((i - 1) == -1) {
				std::cout << "This index (" << i << ") is the beginning of the domain." << std::endl;
				this->createCurve(p0, this->mousePositions.at(i), this->mousePositions.at(i + 1), this->mousePositions.at(i + 2));
			}
			else if (((i + 2) == size) || ((i + 1) == size)) {
				std::cout << "This index (" << i << ") is the end of the domain." << std::endl;
				this->createCurve(this->mousePositions.at(i - 1), this->mousePositions.at(i), this->mousePositions.at(i + 1), p3);
			}
			else {
				std::cout << "Index: " << i << std::endl;
				this->createCurve(this->mousePositions.at(i - 1), this->mousePositions.at(i), this->mousePositions.at(i + 1), this->mousePositions.at(i + 2));
			}
		}
	}

	std::chrono::milliseconds dura(150);

	std::this_thread::sleep_for(dura);

	this->rightClickDB = false;
}