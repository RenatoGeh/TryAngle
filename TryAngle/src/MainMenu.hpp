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
		virtual void update(const sf::Time& dt);
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

SentenceGenerator MainMenu::gen(3);
const std::string MainMenu::titles[] = {"Jilly Bean", "Blame on the Boogey!",
	"I Want My MTV", "On The Border of Hell", "Walk of Lifeguards",
	"Jenni and the Cats", "I Walked", "101 Helium Balloons", "Green Moon",
	"I Am The Waldo", "Mars Angel", "Da Ya Think I'm Fat", "Enjoy The Noise",
	"American Cake", "A Horse With Two Names", "Why M See A", "Porkeater",
	"Painted Love", "Karma is a Chameleon", "Another One Bites the Crust",
	"Livin' off a Player", "No Woman No Pie"};

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
	Component* triggered = getTriggered(event);
	if(triggered == nullptr) return;
	math::u_id id = triggered->getID();

	if(id == play->getID())
		Settings::restart();
	else if(id == options->getID())
		std::cout << "OPTIONS!" << std::endl;
	else if(id == customize->getID())
		std::cout << "CUSTOMIZATION!" << std::endl;
	else if(id == exit->getID())
		Settings::terminate();
	else
		std::cerr << "Something went terribly wrong: You!" << std::endl;

	if(event.type == sf::Event::KeyReleased)
		if(event.key.code == sf::Keyboard::P)
			Settings::pause();
}

std::string MainMenu::getTitle(void) {
	return gen.get();
}

MainMenu* MainMenu::generate(void) {
	Settings::clean();
	Settings::pause();
	return new MainMenu();
}

#endif
