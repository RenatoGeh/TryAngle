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
#include "Timer.hpp"
#include "Utility.hpp"
#include "Vector2D.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Background.hpp"
#include "MainMenu.hpp"
#include "PauseMenu.hpp"

class GameFrame {
	private:
		std::string title;

		sf::RenderWindow* window;
		sf::Font font;
		sf::Clock* thread;

		bool debug_mode;
		bool paused;

		float scale;
	private:
		sf::Text* debug;
		sf::Text* fps;
		Menu* menu;
	private:
		GameFrame(std::string, unsigned short int, unsigned short int);
	public:
		~GameFrame();
	public:
		bool onInit(void);
		int onExecute(void);
		void onEvent(void);
		void onRender(void);
		void onUpdate(const sf::Time&);
		int onCleanup(void);
	public:
		void restart(void);
		void terminate(void);
		void pause(void);
		bool isPaused(void);
		void start(void);
	public:
		void setMenu(Menu*);
	public:
		static GameFrame* getInstance(void);
		static GameFrame* getInstance(std::string, unsigned short int, unsigned short int);
	private:
		static std::ostringstream convert_stream;
	public:
		template <typename T> static std::string toString(T);
};

std::ostringstream GameFrame::convert_stream;

GameFrame::GameFrame(std::string title, unsigned short int width, unsigned short int height) :
	title("A Window With Style."), debug_mode(false), paused(false),
	scale(1) {
	this->title = title;
	this->window = new sf::RenderWindow(sf::VideoMode(width, height),
			this->title, sf::Style::Close, sf::ContextSettings(0, 0, 8));
	this->thread = new sf::Clock();

	this->window->setVerticalSyncEnabled(true);

	Settings::Width = width;
	Settings::Height = height;

	this->debug = nullptr;
	this->fps = nullptr;
	this->menu = nullptr;
}

GameFrame::~GameFrame() {
	delete debug;
	delete window;
	delete fps;
	delete Utility::Spawn::spawner;

	if(menu != nullptr)
		delete menu;
}

bool GameFrame::onInit() {
	if(!font.loadFromFile("src/resources/fonts/LTYPE.ttf"))
		return false;

	Background::onInit();
	this->setMenu(MainMenu::generate());

	this->debug = new sf::Text();
	this->debug->setPosition(50, Settings::Height-300);
	this->debug->setFont(font);
	this->debug->setCharacterSize(15);
	this->debug->setColor(sf::Color::Green);

	this->fps = new sf::Text();
	this->fps->setPosition(Settings::Width-55, 10);
	this->fps->setFont(font);
	this->fps->setCharacterSize(30);
	this->fps->setColor(sf::Color::Yellow);
	this->fps->setStyle(sf::Text::Bold);

	return true;
}

int GameFrame::onExecute() {
	if(!this->onInit())
		return -1;

	this->thread->restart();
	while(window->isOpen()) {
		this->onEvent();
		this->onUpdate(scale*this->thread->restart());
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
		}

		if(menu != nullptr)
			menu->onEvent(event);

		if(event.type == sf::Event::MouseMoved)
			Settings::mouse_position.set(
					event.mouseMove.x, event.mouseMove.y);

		if(!Settings::in_game) continue;

		if(event.type == sf::Event::MouseButtonReleased) {
			if(event.mouseButton.button == sf::Mouse::Middle)
				Entity::add(new Enemy((double)event.mouseButton.x,
						(double)event.mouseButton.y));
		} else if(event.type == sf::Event::KeyReleased) {
			if(event.key.code == sf::Keyboard::F1)
				this->debug_mode = !this->debug_mode;
			else if(event.key.code == sf::Keyboard::P) {
				this->paused = !this->paused;
				if(paused) MenuUtils::setMenu(PauseMenu::generate());
			} else if(event.key.code == sf::Keyboard::E)
				Settings::restart();
			else if(event.key.code == sf::Keyboard::Q)
				Player::getPlayer()->addExp(150);
			else if(event.key.code == sf::Keyboard::Delete)
				if(Player::getPlayer()!=nullptr)
					Player::getPlayer()->damage(150);
		}

		if(Player::getPlayer()!=nullptr)
			Player::getPlayer()->onEvent(event);
	}
}

void GameFrame::onRender() {
	window->clear();

	Background::onRender(window);

	Entity::paint(window);

	if(debug_mode && Settings::in_game) {
		window->draw(*debug);
		window->draw(*fps);
	}

	UserInterface::onRender(window);

	if(menu != nullptr)
		window->draw(*menu);

	window->display();
}

void GameFrame::onUpdate(const sf::Time& dt) {
	//TODO: Updating
	if(menu != nullptr)
		menu->update(dt);

	Timer::onUpdate(dt);

	if(paused || !Settings::in_game) return;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) scale+=0.01;
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) scale-=0.01;

	Background::onUpdate(dt);
	Entity::onUpdate(dt);
	UserInterface::onUpdate(dt);

	if(debug_mode && Settings::in_game) {
		debug->setString("Player's position: [" + toString(Player::getPlayer()->getPosition().x)
				+ ", " + toString(Player::getPlayer()->getPosition().y) + "]"
				+ "\nPlayer's speed: " + toString(Player::getPlayer()->getSpeed())
				+ "\nMouse position: " + toString(Settings::mouse_position)
				+ "\nEntities: " + toString(Entity::getEntities()->size())
				+ "\nPaintables: " + toString(Entity::getPaintables()->size())
				+ "\nPlayer's Health: " + toString(Player::getPlayer()->getHealth())
				+ "\nPlayer's Experience: " + toString(Player::getPlayer()->getExp())
				+ "\nPlayer's Level: " + toString(Player::getPlayer()->getLevel())
				+ "\nPlayer's Shield: " + toString(Player::getPlayer()->getShield().getShield())
				+ "\nTime Scale: " + toString(scale));
		fps->setString(toString((1/dt.asSeconds())*scale));
	}
}

int GameFrame::onCleanup() {
	try {
		Entity::clear();
		Timer::clear();
		UserInterface::onCleanup();
		Background::onCleanup();
	} catch(...) {
		std::cerr << "Cleanup gone wrong. That's bad yo." << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

namespace Settings {
	void terminate(void) {GameFrame::getInstance()->terminate();}
	void pause(void) {GameFrame::getInstance()->pause();}
	bool isPaused(void) {return GameFrame::getInstance()->isPaused();}
	void restart(void) {GameFrame::getInstance()->restart();}
	void clean(void) {
		UserInterface::onCleanup();
		Timer::clear();
		Timer::onCleanup();

		if(Player::getPlayer() != nullptr)
			Player::getPlayer()->destroy();

		Entity::clear();

		MenuUtils::setMenu(nullptr);

		Settings::in_game = false;
	}
}

void GameFrame::terminate(void) {this->window->close();}
void GameFrame::pause(void) {this->paused = !this->paused;}
bool GameFrame::isPaused(void) {return paused;}

void GameFrame::restart(void) {
	Background::onCleanup();
	UserInterface::onCleanup();
	Timer::clear();
	Timer::onCleanup();

	if(Player::getPlayer() != nullptr)
		Player::getPlayer()->destroy();

	Entity::clear();

	Player::setPlayer(new Player("Your mom",
			Settings::Width/2-30, Settings::Height/2-30, 30));
	Background::onInit();

	Utility::Spawn::spawner = new ActionTimer<void(void)>(sf::seconds,
			10.0f, true, 0.0f, Utility::Spawn::enemy, true, false);

	MenuUtils::setMenu(nullptr);

	if(this->paused)
		this->pause();

	Settings::in_game = true;
}

namespace MenuUtils {
	void setMenu(Menu* menu=nullptr) {
		GameFrame::getInstance()->setMenu(menu);
	}

	void mainMenu(void) {
		GameFrame::getInstance()->setMenu(MainMenu::generate());
	}
}

void GameFrame::setMenu(Menu* m) {
	delete menu;
	menu = m;
}

template <typename T> std::string GameFrame::toString(T e) {
	convert_stream << e;
	std::string cpy (convert_stream.str());
	convert_stream.str("");
	convert_stream.clear();
	return cpy;
}

static GameFrame* instance = nullptr;

GameFrame* GameFrame::getInstance(void) {return instance;}
GameFrame* GameFrame::getInstance(std::string title,
		unsigned short int width=800, unsigned short int height=600) {
	if(instance == nullptr)
		instance = new GameFrame(title, width, height);

	return instance;
}

#endif
