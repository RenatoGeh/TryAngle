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
		Vector2D shieldSize;

		sf::RectangleShape* life_shape;
		sf::RectangleShape* shield_shape;
		sf::RectangleShape* exp_shape;

		double* exp;
		double* life;
		double& shield;
	public:
		UserInterface(Player*);
		~UserInterface(void);
	public:
		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void update(const sf::Time&);
	public:
		static void bind(Player*);
		static void onUpdate(const sf::Time&);
		static void onRender(sf::RenderTarget*);
		static void onCleanup(void);
};

UserInterface* UserInterface::ui = nullptr;

UserInterface::UserInterface(Player* parent) :
		shield(parent->getShield().getShield()) {
	this->player = parent;

	this->expSize = Vector2D(Settings::Width-80, 5);
	this->exp_shape = new sf::RectangleShape(expSize);
	this->exp_shape->setOutlineColor(sf::Color::Black);
	this->exp_shape->setOutlineThickness(1.5);
	this->exp_shape->setFillColor(sf::Color(0, 170, 170));

	this->lifeSize = Vector2D(5, Settings::Height-80);
	this->life_shape = new sf::RectangleShape(lifeSize);
	this->life_shape->setOutlineColor(sf::Color::Black);
	this->life_shape->setOutlineThickness(1.5);
	this->life_shape->setFillColor(sf::Color(0, 180, 0));

	this->shieldSize = Vector2D(5, Settings::Height-80);
	this->shield_shape = new sf::RectangleShape(shieldSize);
	this->shield_shape->setOutlineColor(sf::Color::Black);
	this->shield_shape->setOutlineThickness(1.5);
	this->shield_shape->setFillColor(sf::Color(0, 0, 250));

	this->exp_shape->setPosition(40, Settings::Height-40);
	this->life_shape->setPosition(20, 40);
	this->shield_shape->setPosition(35, 40);

	this->life = &(parent->health);
	this->exp = &(parent->exp);
}

UserInterface::~UserInterface(void) {
	delete life_shape;
	delete exp_shape;
	delete shield_shape;
}

void UserInterface::update(const sf::Time& dt) {
	this->expSize.set(
			(Settings::Width-75)*(*exp)/Mortal::MAX_EXP, 7.5);
	this->lifeSize.set(
			7.5, (Settings::Height-80)*((*life<0?0:*life))/Mortal::MAX_HEALTH);
	this->shieldSize.set(
			7.5, (Settings::Height-80)*shield/Shield::MAX_SHIELD);

	this->exp_shape->setSize(expSize);
	this->life_shape->setSize(lifeSize);
	this->shield_shape->setSize(shieldSize);

	sf::Vector2<float> pos = this->life_shape->getPosition();

	this->life_shape->setPosition(pos.x,
			(Settings::Height-80) - lifeSize.y - 50);
	this->shield_shape->setPosition(shield_shape->getPosition().x,
			(Settings::Height-80) - shieldSize.y - 50);
}

void UserInterface::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*life_shape, states);
	target.draw(*exp_shape, states);
	target.draw(*shield_shape, states);
}

void UserInterface::bind(Player* p) {
	if(UserInterface::ui == nullptr)
		ui = new UserInterface(p);
}

void UserInterface::onUpdate(const sf::Time& dt) {
	if(UserInterface::ui != nullptr)
		UserInterface::ui->update(dt);
}

void UserInterface::onRender(sf::RenderTarget *window) {
	if(UserInterface::ui != nullptr)
		window->draw(*UserInterface::ui);
}

void UserInterface::onCleanup(void) {
	delete UserInterface::ui;
	UserInterface::ui = nullptr;
}

#endif
