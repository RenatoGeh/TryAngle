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
		static UserInterface* ui;
	private:
		Player* player;

		Vector2D lifeSize;
		Vector2D expSize;

		sf::RectangleShape* life_shape;
		sf::RectangleShape* exp_shape;

		double* exp;
		double* life;
	public:
		UserInterface(Player*);
		~UserInterface(void);
	public:
		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void update(sf::Time);
	public:
		static void bind(Player*);
		static void onUpdate(sf::Time);
		static void onRender(sf::RenderWindow*);
		static void onCleanup(void);
};

UserInterface* UserInterface::ui = nullptr;

UserInterface::UserInterface(Player* parent) {
	this->player = parent;

	this->expSize = Vector2D(Settings::Width-80, 5);
	this->exp_shape = new sf::RectangleShape(lifeSize);
	this->exp_shape->setOutlineColor(sf::Color::Black);
	this->exp_shape->setOutlineThickness(1.5);
	this->exp_shape->setFillColor(sf::Color(0, 170, 170));

	this->lifeSize = Vector2D(5, Settings::Height-80);
	this->life_shape = new sf::RectangleShape(expSize);
	this->life_shape->setOutlineColor(sf::Color::Black);
	this->life_shape->setOutlineThickness(1.5);
	this->life_shape->setFillColor(sf::Color(0, 180, 0));

	this->exp_shape->setPosition(40, Settings::Height-40);
	this->life_shape->setPosition(20, 40);

	this->life = &(parent->health);
	this->exp = &(parent->exp);
}

UserInterface::~UserInterface(void) {
	delete life_shape;
	delete exp_shape;
}

void UserInterface::update(sf::Time dt) {
	this->expSize.set(
			(Settings::Width-75)*(*exp)/Mortal::MAX_EXP, 7.5);
	this->lifeSize.set(
			7.5, (Settings::Height-80)*(*life)/Mortal::MAX_HEALTH);

	this->exp_shape->setSize(expSize);
	this->life_shape->setSize(lifeSize);

	double delta = (Settings::Height-80) - lifeSize.y;
	sf::Vector2<float> pos = this->life_shape->getPosition();

	this->life_shape->setPosition(pos.x, delta - 50);
}

void UserInterface::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*life_shape, states);
	target.draw(*exp_shape, states);
}

void UserInterface::bind(Player* p) {
	if(UserInterface::ui == nullptr)
		ui = new UserInterface(p);
}

void UserInterface::onUpdate(sf::Time dt) {
	UserInterface::ui->update(dt);
}

void UserInterface::onRender(sf::RenderWindow *window) {
	window->draw(*UserInterface::ui);
}

void UserInterface::onCleanup(void) {delete UserInterface::ui;}

#endif
