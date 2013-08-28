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
#include "Button.hpp"
#include "Timer.hpp"
#include "Utility.hpp"
#include "Vector2D.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Background.hpp"

class GameFrame {
	private:
		std::string title;

		sf::RenderWindow* window;
		sf::Font font;
		sf::Clock* thread;

		std::ostringstream convert_stream;
		bool debug_mode;
	private:
		Enemy* pet;
		sf::Text* debug;
		sf::Text* fps;
		ActionTimer<void(void)>* spawner;
	public:
		GameFrame(std::string, unsigned short int, unsigned short int);
		~GameFrame();
	public:
		bool onInit(void);
		int onExecute(void);
		void onEvent(void);
		void onRender(void);
		void onUpdate(const sf::Time&);
		int onCleanup(void);
	public:
		template <typename T> std::string toString(T);
};

GameFrame::GameFrame(std::string title, unsigned short int width, unsigned short int height) {
	this->title = title;
	this->window = new sf::RenderWindow(sf::VideoMode(width, height),
			this->title, sf::Style::Close, sf::ContextSettings(0, 0, 8));
	this->thread = new sf::Clock();

	this->window->setVerticalSyncEnabled(true);
	this->debug_mode = true;

	Settings::Width = width;
	Settings::Height = height;

	this->debug = nullptr;
	this->pet = nullptr;
	this->fps = nullptr;
	this->spawner = nullptr;
}

GameFrame::~GameFrame() {
	delete debug;
	delete window;
	delete spawner;
}

bool GameFrame::onInit() {
	if(!font.loadFromFile("src/resources/fonts/LTYPE.ttf"))
		return false;

	Player::setPlayer(new Player("Your mom",
			Settings::Width/2-30, Settings::Height/2-30, 30));

	Background::onInit();

	this->debug = new sf::Text();
	this->debug->setPosition(50, Settings::Height-200);
	this->debug->setFont(font);
	this->debug->setCharacterSize(15);
	this->debug->setColor(sf::Color::Green);

	this->fps = new sf::Text();
	this->fps->setPosition(Settings::Width-55, 10);
	this->fps->setFont(font);
	this->fps->setCharacterSize(30);
	this->fps->setColor(sf::Color::Yellow);
	this->fps->setStyle(sf::Text::Bold);

	this->spawner = new ActionTimer<void(void)>(sf::seconds, 10.0f,
			true, 0.0f, Utility::Spawn::enemy, true, false);

	this->pet = new Enemy(100, 100);
	Entity::add(this->pet);

	return true;
}

int GameFrame::onExecute() {
	if(!this->onInit())
		return -1;

	this->thread->restart();
	while(window->isOpen()) {
		this->onEvent();
		this->onUpdate(this->thread->restart());
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
			if(event.type == sf::Event::MouseButtonReleased) {
				if(event.mouseButton.button == sf::Mouse::Middle)
					Entity::add(new Enemy((double)event.mouseButton.x,
							(double)event.mouseButton.y));
			} else if(event.type == sf::Event::MouseMoved)
				Settings::mouse_position->set(
						event.mouseMove.x, event.mouseMove.y);
			if(event.type == sf::Event::MouseButtonReleased)
				if(event.mouseButton.button == sf::Mouse::Right)
					this->pet->getPath()->push(
							event.mouseButton.x, event.mouseButton.y);

			if(event.type == sf::Event::KeyReleased) {
				if(event.key.code == sf::Keyboard::F1)
					this->debug_mode = !this->debug_mode;
				else if(event.key.code == sf::Keyboard::F2)
					Utility::Spawn::enemy();
			}

			Player::getPlayer()->onEvent(event);
		}
	}
}

void GameFrame::onRender() {
	window->clear();

	Background::onRender(window);

	//TODO: Rendering
	Entity::paint(window);

	if(debug_mode) {
		window->draw(*debug);
		window->draw(*fps);
	}

	UserInterface::onRender(window);

	window->display();
}

void GameFrame::onUpdate(const sf::Time& dt) {
	//TODO: Updating
	Background::onUpdate(dt);
	Entity::onUpdate(dt);
	Timer::onUpdate(dt);
	UserInterface::onUpdate(dt);

	if(debug_mode) {
		debug->setString("Player's position: [" + toString(Player::getPlayer()->getPosition().x)
				+ ", " + toString(Player::getPlayer()->getPosition().y) + "]"
				+ "\nPlayer's speed: " + toString(Player::getPlayer()->getSpeed())
				+ "\nMouse position: " + toString(Settings::mouse_position)
				+ "\nEntities: " + toString(Entity::getEntities()->size())
				+ "\nPaintables: " + toString(Entity::getPaintables()->size())
				+ "\nPlayer's Health: " + toString(Player::getPlayer()->getHealth())
				+ "\nPlayer's Experience: " + toString(Player::getPlayer()->getExp()));
		fps->setString(toString(1/dt.asSeconds()));
	}
}

int GameFrame::onCleanup() {
	try {
		Entity::clear();
		Timer::clear();
		UserInterface::onCleanup();
		Background::onCleanup();
		Button::onCleanup();
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
