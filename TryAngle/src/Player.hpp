/*
 * Player.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: Renato Lui Geh
 */

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "Entity.hpp"
#include "Vector2D.hpp"
#include "DeathMenu.hpp"
#include "Shield.hpp"
#include "SkillSet.hpp"

class Player : public Entity {
	private:
		static Player* def_player;
	public:
		static Player* getPlayer(void);
		static void setPlayer(Player*);
	private:
		Shield shield;
	public:
		SkillSet set;
	protected:
		sf::CircleShape* shape;
	public:
		Player(std::string, double, double, double);
		~Player(void);
	public:
		void setLevel(unsigned short int);
		void addLevel(unsigned short int);
		void subLevel(unsigned short int);
	public:
		Shield& getShield(void);
	public:
		virtual void setColor(sf::Uint8, sf::Uint8, sf::Uint8);
		virtual void setOutlineColor(sf::Uint8, sf::Uint8, sf::Uint8);
		virtual void setColor(const sf::Color&);
		virtual void setOutlineColor(const sf::Color&);
	public:
		virtual void damage(double);
	public:
		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void update(const sf::Time&);
		bool onEvent(const sf::Event&);
	public:
		bool handleDeath(void);
	public:
		virtual Entity::Type getID(void);
};

Player* Player::def_player = nullptr;

Player::Player(std::string name, double x, double y, double r) :
		Entity(name, x, y, 2*r, 2*r), shield(this), set() {
	this->shape = new sf::CircleShape(r);
	this->team = true;

	this->setOutlineColor(0, 0, 0);
	this->shape->setOutlineThickness(1.5);
	this->shape->setPointCount(set.getSides());
	this->setColor(0, 0, 255);

	this->setOrigin(0, 0);
	this->setPosition(position->x, position->y);
	this->setOrigin(r, r);

	this->level = 0;

	set.link(set.getSkills().add(&Skills::Instant), sf::Keyboard::F1);
}

Player::~Player() {
	delete shape;
	Player::def_player = nullptr;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform = getTransform();
	target.draw(*shape, states);
	target.draw(shield, states);
}

void Player::update(const sf::Time& dt) {
	Entity::update(dt);

	shield.update(dt);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->angle += (dt.asMilliseconds()/10.)*math::PI/60;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->angle -= (dt.asMilliseconds()/10.)*math::PI/60;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->speed->y = -2.5;
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->speed->y = 2.5;
	else this->speed->y = 0;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->speed->x = -2.5;
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->speed->x = 2.5;
	else this->speed->x = 0;

	if(position->x < 0 && speed->x < 0)
		speed->x = 0;
	else if(position->x > Settings::Width && speed->x > 0)
		speed->x = 0;

	if(position->y < 0 && speed->y < 0)
		speed->y = 0;
	else if(position->y > Settings::Height && speed->y > 0)
		speed->y = 0;
}

bool Player::onEvent(const sf::Event& event) {
	set.onEvent(event);

	switch(event.type) {
		case sf::Event::KeyPressed:
		break;
		case sf::Event::KeyReleased:
			if(event.key.code == sf::Keyboard::Space)
				this->shoot();
			if(event.key.code == sf::Keyboard::L) {
				this->addLevel(1);
				std::cout << this->getLevel() << std::endl;
			}
		break;
		case sf::Event::MouseMoved:
			angle = atan2(position->x - Settings::mouse_position.x,
					position->y - Settings::mouse_position.y);
		break;
		case sf::Event::MouseButtonPressed:
			if(event.mouseButton.button == sf::Mouse::Left)
				this->shoot();
		break;
		default:
			return false;
	}

	return true;
}

Entity::Type Player::getID(void) {return Entity::Type::Player;}

void Player::setLevel(unsigned short int level) {
	if(this->level != level) {
		this->level = level;
	}
}

void Player::addLevel(unsigned short int increment = 1) {
	this->level += increment;
}
void Player::subLevel(unsigned short int decrement = 1) {
	if(this->level > 0) {
		this->level -= decrement;
	}
}

void Player::setColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b) {
	Entity::setColor(r, g, b);
	shape->setFillColor(*color);
}

void Player::setOutlineColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b) {
	Entity::setOutlineColor(r, g, b);
	shape->setOutlineColor(*outline_color);
}

void Player::setColor(const sf::Color& color) {
	setColor(color.r, color.g, color.b);
}

void Player::setOutlineColor(const sf::Color& color) {
	setOutlineColor(color.r, color.g, color.b);
}

Shield& Player::getShield(void) {return shield;}

#include "Stats.hpp"

bool Player::handleDeath(void) {
	if(this->isDead()) {
		Stats state(this);
		Stats::save(state);
		Stats::setDefault(state);

		Settings::pause();
		MenuUtils::setMenu(DeathMenu::generate());
		return true;
	}
	return false;
}

void Player::damage(double dam) {shield.damage(dam);}

#include "UserInterface.hpp"

Player* Player::getPlayer(void) {return Player::def_player;}
void Player::setPlayer(Player* player) {
	Player::def_player = player;
	Entity::add(Player::def_player);
	UserInterface::bind(Player::def_player);
	Stats::getDefault(Player::def_player);
}

namespace ProjectileUtility {
	void transferExp(double n) {
		Player::getPlayer()->addExp(n);
	}
}

#endif
