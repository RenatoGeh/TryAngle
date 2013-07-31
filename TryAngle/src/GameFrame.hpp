/*
 * GameFrame.hpp
 *
 *  Created on: Jul 30, 2013
 *      Author: Renato Lui Geh
 */

#ifndef GAMEFRAME_HPP_
#define GAMEFRAME_HPP_

#include <string>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Vector2D.hpp"
#include "Player.hpp"

class GameFrame {
	private:
		std::string title;

		sf::RenderWindow* window;
		sf::Font font;

		std::ostringstream convert_stream;
	private:
		Player* player;
		sf::Text* debug;
	public:
		GameFrame(std::string, unsigned short int, unsigned short int);
		~GameFrame();
	public:
		bool onInit(void);
		int onExecute(void);
		void onEvent(void);
		void onRender(void);
		void onUpdate(void);
		int onCleanup(void);
	public:
		template <typename T> std::string toString(T);
};

GameFrame::GameFrame(std::string title, unsigned short int width, unsigned short int height) {
	this->title = title;
	this->window = new sf::RenderWindow(sf::VideoMode(width, height),
			this->title, sf::Style::Close, sf::ContextSettings(0, 0, 8));

	this->window->setVerticalSyncEnabled(true);

	Settings::Width = width;
	Settings::Height = height;

	this->player = NULL;
	this->debug = NULL;
}

GameFrame::~GameFrame() {
	delete player;
	delete debug;
	delete window;
}

bool GameFrame::onInit() {
	if(!font.loadFromFile("src/resources/fonts/LTYPE.ttf"))
		return false;

	this->player = new Player("Francis J. Underwood",
			Settings::Width/2-30, Settings::Height/2-30, 30);

	Entity::add(this->player);

	this->debug = new sf::Text();
	this->debug->setPosition(50, Settings::Height-200);
	this->debug->setFont(font);
	this->debug->setCharacterSize(15);
	this->debug->setColor(sf::Color::Green);
	Entity::add(this->debug);

	return true;
}

int GameFrame::onExecute() {
	if(!this->onInit())
		return -1;

	while(window->isOpen()) {
		this->onEvent();
		this->onUpdate();
		this->onRender();
	}

	return this->onCleanup();
}

void GameFrame::onEvent() {
	sf::Event event;

	while(window->pollEvent(event)) {
		if(event.type == sf::Event::Closed) {
			window->close();
			break;
		} else {
			if(event.type == sf::Event::MouseMoved)
				Settings::mouse_position->set(
						event.mouseMove.x, event.mouseMove.y);
			this->player->onEvent(event);
		}
	}
}

void GameFrame::onRender() {
	window->clear(sf::Color::Black);

	//TODO: Rendering
	Entity::paint(window);

	window->display();
}

void GameFrame::onUpdate() {
	//TODO: Updating
	Entity::onUpdate();

	debug->setString("Player's position: [" + toString(player->getPosition().x)
			+ ", " + toString(player->getPosition().y) + "]"
			+ "\nPlayer's speed: " + toString(player->getSpeed())
			+ "\nMouse position: " + toString(Settings::mouse_position)
			+ "\nEntities: " + toString(Entity::getEntities()->size())
			+ "\nPaintables: " + toString(Entity::getPaintables()->size()));
}

int GameFrame::onCleanup() {
	try {
		Entity::clear();
	} catch(...) {
		std::cerr << "Cleanup gone wrong. That's bad yo." << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

template <typename T> std::string GameFrame::toString(T e) {
	convert_stream << e;
	std::string cpy (convert_stream.str());
	convert_stream.str("");
	convert_stream.clear();
	return cpy;
}

#endif