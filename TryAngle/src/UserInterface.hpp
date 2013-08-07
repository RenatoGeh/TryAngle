/*
 * UserInterface.hpp
 *
 *  Created on: Aug 4, 2013
 *      Author: Renato Lui Geh
 */

#ifndef USERINTERFACE_HPP_
#define USERINTERFACE_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Player.hpp"
#include "Utility.hpp"

class UserInterface : public sf::Drawable {
	private:
		static UserInterface ui;
	private:
		Player* player;

		sf::RectangleShape* life_shape;
		sf::RectangleShape* exp_shape;

		short int level;
	public:
		UserInterface(Player*);
		~UserInterface(void);
	public:
		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void update(void);
	public:
		void bind(Player*);
};

UserInterface UserInterface::ui = nullptr;

UserInterface::UserInterface(Player* parent) {
	this->player = parent;
	this->life_shape = new sf::RectangleShape(sf::Vector2<float>(
			Settings::Width-80, 50));
	this->exp_shape = new sf::RectangleShape(sf::Vector2<float>(
			50, Settings::Height-80));

	this->life_shape->setPosition(50, 40);
	this->exp_shape->setPosition(40, Settings::Height-75);

	this->level = 0;
}

void UserInterface::update(void) {

}

void UserInterface::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->update();

	target.draw(*life_shape, states);
	target.draw(*exp_shape, states);
}

void UserInterface::bind(Player* p) {
	if(UserInterface == nullptr)
		ui = UserInterface(p);
}

#endif
