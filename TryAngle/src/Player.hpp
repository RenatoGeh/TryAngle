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
#include <cmath>
#include "Entity.hpp"
#include "Vector2D.hpp"
#include "Projectile.hpp"

class Player : public Entity {
	protected:
		sf::CircleShape* shape;
	public:
		Player(std::string, double, double, double);
		~Player(void);
	public:
		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void update(sf::Time);
		bool onEvent(sf::Event&);
	public:
		void lookAt(double, double);
		void shoot(void);
};

Player::Player(std::string name, double x, double y, double r) :
		Entity(name, x, y, 2*r, 2*r) {
	this->shape = new sf::CircleShape(r);
	this->color = new sf::Color(0, 0, 255);
	this->team = false;

	this->shape->setPointCount(3);
	this->shape->setFillColor(*color);

	this->setOrigin(0, 0);
	this->setPosition(position->x, position->y);
	this->setOrigin(r, r);
}

Player::~Player() {
	delete shape;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform = getTransform();
	target.draw(*shape, states);
}

void Player::update(sf::Time dt) {
	Entity::update(dt);

	if(position->x < 0 || position->x + size->x > Settings::Width)
		speed->x = 0;
	if(position->y < 0 || position->y + size->y > Settings::Height)
		speed->y = 0;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->angle += 2;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->angle -= 2;

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
}

void Player::lookAt(double x, double y) {
	this->angle = atan2(position->x-x, position->y-y);
}

void Player::shoot() {
	double alpha, theta = math::PI-(this->getRotation()*math::PI/180);
	double r = this->size->x/2;

	for(short int i=0;i<3;i++) {
		alpha = theta + i*(2*math::PI/3);
		Entity::add(new Projectile(this,
				position->x + r*sin(alpha),
				position->y + r*cos(alpha),
				sin(alpha)*5, cos(alpha)*5));
	}
}

bool Player::onEvent(sf::Event& event) {
	switch(event.type) {
		case sf::Event::KeyPressed:
		break;
		case sf::Event::KeyReleased:
			if(event.key.code == sf::Keyboard::Space)
				this->shoot();
		break;
		default:
			return false;
	}

	return true;
}

#endif
