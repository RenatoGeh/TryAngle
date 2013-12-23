/*
 * MainMenu.hpp
 *
 *  Created on: Sep 21, 2013
 *      Author: Renato Lui Geh
 */

#ifndef MAINMENU_HPP_
#define MAINMENU_HPP_

#include "Menu.hpp"
#include "Button.hpp"
#include "Background.hpp"
#include "Timer.hpp"
#include "SentenceGenerator.hpp"
#include "CustomMenu.hpp"

class MainMenu : public Menu {
	private:
		Button* play;
		Button* options;
		Button* customize;
		Button* exit;
	private:
		MainMenu(void);
		virtual ~MainMenu(void);
	public:
		virtual void update(const sf::Time&);
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
		virtual void onEvent(const sf::Event&);
	private:
		static const std::string titles[];
		static std::string getTitle(void);
	public:
		static MainMenu* generate(void);
	private:
		static SentenceGenerator gen;
};

SentenceGenerator MainMenu::gen(2);

MainMenu::MainMenu(void) : Menu(MainMenu::getTitle()),
	play(new Button(Settings::Width/2, 150, "Play!")),
	options(new Button(Settings::Width/2, 250, "Awpshuns")),
	customize(new Button(Settings::Width/2, 350, "Make me Unique!")),
	exit(new Button(Settings::Width/2, 450, "See you soon!")) {

	this->setTitleSize(20);

	this->add(play);
	this->add(options);
	this->add(customize);
	this->add(exit);
};

MainMenu::~MainMenu(void) {
	Settings::pause();
};

void MainMenu::update(const sf::Time& dt) {
	Menu::update(dt);
	Background::onUpdate(dt);
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	Background::onRender(&target);
	Menu::draw(target, state);
}

void MainMenu::onEvent(const sf::Event& event) {
	Component* triggered = nullptr;

	while((triggered = popEvent(event))) {
		math::u_id id = triggered->getID();

		if(id == play->getID())
			Settings::restart();
		else if(id == options->getID())
			std::cout << "OPTIONS!" << std::endl;
		else if(id == customize->getID())
			MenuUtils::setMenu(CustomMenu::generate());
		else if(id == exit->getID())
			Settings::terminate();
		else
			std::cerr << "Something went terribly wrong: You!" << std::endl;

		if(event.type == sf::Event::KeyReleased)
			if(event.key.code == sf::Keyboard::P)
				Settings::pause();
	}
}

std::string MainMenu::getTitle(void) {
	return gen.describe();
}

MainMenu* MainMenu::generate(void) {
	Settings::clean();
	Settings::pause();
	return new MainMenu();
}

#endif
