/*
 * DeathMenu.hpp
 *
 *  Created on: Aug 31, 2013
 *      Author: Renato Lui Geh
 */

#ifndef DEATHMENU_HPP_
#define DEATHMENU_HPP_

#include <vector>
#include <SFML/Graphics.hpp>
#include "Utility.hpp"
#include "Menu.hpp"

class DeathMenu : public Menu {
	private:
		sf::RectangleShape board;
		sf::Text respawn;
		sf::Text message;
	private:
		DeathMenu(void);
		~DeathMenu(void);
	public:
		void onEvent(const sf::Event&);
		void update(const sf::Time&);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
	private:
		static const std::string messages[];
		static std::string getMessage(void);
	public:
		static DeathMenu* generate(void);
};

const std::string DeathMenu::messages[] = {"Hi. Po. Po. Ta. Mus.",
		"42", "It is known.", "Hey there sexy.", "You're weak!",
		"Resistance is futile.", "All your base are belong to us.",
		"I have no creativity whatsoever."};

DeathMenu::DeathMenu(void) : Menu("THE END"),
		board(sf::Vector2<float>(Settings::Width, Settings::Height)) {
	respawn.setCharacterSize(20);
	respawn.setFont(Settings::DEF_FONT);
	respawn.setColor(sf::Color::Red);
	respawn.setString("To respawn press 'R'.\nTo quit press 'Q'.");

	sf::FloatRect respawnBounds(respawn.getLocalBounds());
	respawn.setPosition(Settings::Width - respawnBounds.width - 50,
			Settings::Height - respawnBounds.height - 100);

	message.setCharacterSize(20);
	message.setFont(Settings::DEF_FONT);
	message.setColor(sf::Color::Red);
	message.setString(DeathMenu::getMessage());

	message.setPosition(50, 250);

	board.setOrigin(0, 0);
	board.setPosition(0, 0);

	board.setFillColor(sf::Color(0, 0, 0, 200));
}

DeathMenu::~DeathMenu(void) {}

void DeathMenu::onEvent(const sf::Event& event) {
	if(event.type == sf::Event::KeyReleased) {
		if(event.key.code == sf::Keyboard::R) {
			Settings::restart();
		} else if(event.key.code == sf::Keyboard::Q) {
			Settings::terminate();
		}
	}
}

void DeathMenu::update(const sf::Time& dt) {
	Menu::update(dt);
}

void DeathMenu::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	target.draw(board, state);
	target.draw(message, state);
	target.draw(respawn, state);

	Menu::draw(target, state);
}

std::string DeathMenu::getMessage(void) {
	return messages[Utility::Random::getUnsignedRandom(0,
			sizeof(messages)/sizeof(*messages))];
}

DeathMenu* DeathMenu::generate(void) {return new DeathMenu();}

#endif
