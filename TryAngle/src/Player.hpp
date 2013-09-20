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

class Player : public Entity {
	private:
		static Player* def_player;
	public:
		static Player* getPlayer(void);
		static void setPlayer(Player*);
	private:
		Shield shield;
	protected:
		sf::CircleShape* shape;
	public:
		Player(std::string, double, double, double);
		~Player(void);
	public:
		void setLevel(unsigned short int);
		void addLevel(unsigned short int);
		void subLevel(unsigned short int);
		unsigned short int getLevel(void);
	public:
		Shield& getShield(void);
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
		Entity(name, x, y, 2*r, 2*r), shield(this) {
	this->shape = new sf::CircleShape(r);
	this->color = new sf::Color(0, 0, 255);
	this->team = true;

	this->shape->setOutlineColor(sf::Color::Black);
	this->shape->setOutlineThickness(1.5);
	this->shape->setPointCount(3 + this->level);
	this->shape->setFillColor(*color);

	this->setOrigin(0, 0);
	this->setPosition(position->x, position->y);
	this->setOrigin(r, r);

	this->level = 0;
	this->setHealth(50);
	this->setExp(100);
}

Player::~Player() {
	delete shape;
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
		default:
			return false;
	}

	return true;
}

Entity::Type Player::getID(void) {return Entity::Type::Player;}

void Player::setLevel(unsigned short int level) {
	if(this->level != level) {
		this->level = level;
		this->shape->setPointCount(3 + this->level);
	}
}

void Player::addLevel(unsigned short int increment = 1) {
	this->level += increment;
	this->shape->setPointCount(3 + this->level);
}
void Player::subLevel(unsigned short int decrement = 1) {
	if(this->level > 0) {
		this->level -= decrement;
		this->shape->setPointCount(3 + this->level);
	}
}

Shield& Player::getShield(void) {return shield;}

bool Player::handleDeath(void) {
	if(this->isDead()) {
		Settings::pause();
		MenuUtils::setMenu(DeathMenu::generate());
		return true;
	}
	return false;
}

void Player::damage(double dam) {shield.damage(dam);}

unsigned short int Player::getLevel(void) {return this->level;}

#include "UserInterface.hpp"

Player* Player::getPlayer(void) {return Player::def_player;}
void Player::setPlayer(Player* player) {
	Player::def_player = player;
	Entity::add(Player::def_player);
	UserInterface::bind(Player::def_player);
}

namespace ProjectileUtility {
	void transferExp(double n) {
		Player::getPlayer()->addExp(n);
	}
}

#endif
