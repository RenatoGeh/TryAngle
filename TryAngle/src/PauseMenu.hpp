/*
 * PauseMenu.hpp
 *
 *  Created on: Sep 23, 2013
 *      Author: Renato Lui Geh
 */

#ifndef PAUSEMENU_HPP_
#define PAUSEMENU_HPP_

#include "Menu.hpp"
#include "SentenceGenerator.hpp"

class PauseMenu : public Menu {
	private:
		sf::RectangleShape board;
		sf::Text message;
		sf::Text instructions;
	private:
		PauseMenu(void);
		~PauseMenu(void);
	public:
		void onEvent(const sf::Event&);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
	private:
		static const std::string messages[];
		static std::string getMessage(void);
	public:
		static PauseMenu* generate(void);
	private:
		static SentenceGenerator gen;
};

SentenceGenerator PauseMenu::gen(3);

PauseMenu::PauseMenu(void) : Menu("Paused Game"),
		board(sf::Vector2<float>(Settings::Width, Settings::Height)) {
	instructions.setCharacterSize(15);
	instructions.setFont(Settings::DEF_FONT);
	instructions.setColor(sf::Color::Red);
	instructions.setString("To unpause press 'P'.\n"
			"To restart press 'R'.\n"
			"To quit press 'Q'.\n"
			"To go to the main screen press 'M'.");

	sf::FloatRect instBounds(instructions.getLocalBounds());
	instructions.setPosition(Settings::Width - instBounds.width - 50,
			Settings::Height - instBounds.height - 100);

	message.setCharacterSize(20);
	message.setFont(Settings::DEF_FONT);
	message.setColor(sf::Color::Red);
	message.setString(PauseMenu::getMessage());

	message.setPosition(50, 250);

	board.setOrigin(0, 0);
	board.setPosition(0, 0);

	board.setFillColor(sf::Color(0, 0, 0, 200));
}

PauseMenu::~PauseMenu(void) {}

void PauseMenu::onEvent(const sf::Event& event) {
	if(event.type == sf::Event::KeyReleased) {
		if(event.key.code == sf::Keyboard::P)
			MenuUtils::setMenu(nullptr);
		else if(event.key.code == sf::Keyboard::Q)
			Settings::terminate();
		else if(event.key.code == sf::Keyboard::M)
			MenuUtils::setMenu(MainMenu::generate());
		else if(event.key.code == sf::Keyboard::R)
			Settings::restart();
	}
}

void PauseMenu::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	target.draw(board, state);
	target.draw(message, state);
	target.draw(instructions, state);

	Menu::draw(target, state);
}

std::string PauseMenu::getMessage(void) {
	return gen.get();
}

PauseMenu* PauseMenu::generate(void) {return new PauseMenu();}

#endif
