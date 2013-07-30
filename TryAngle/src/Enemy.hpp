/*
 * Enemy.hpp
 *
 *  Created on: Jul 29, 2013
 *      Author: Renato Lui Geh
 */

#ifndef ENEMY_HPP_
#define ENEMY_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Entity.hpp"
#include "Player.hpp"
#include "Vector2D.hpp"

namespace Utility {
	#include <random>

	std::random_device gen;

	sf::Color getRandomColor(void);

	sf::Color getRandomColor() {
		return sf::Color(gen()%256, gen()%256, gen()%256);
	}
}

class Enemy : public Entity {
	private:
		bool wasInside;
	protected:
		sf::CircleShape* shape;
	public:
		Enemy(double, double, double);
		~Enemy(void);
	public:
		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void update(void);
	public:
		void destroy(void);
};

Enemy::Enemy(double x, double y, double r) : Entity("Enemy", x, y, 2*r, 2*r) {
	this->shape = new sf::CircleShape(r);
	this->color = Utility::getRandomColor();
	this->wasInside = false;

	this->shape->setPointCount(3);
	this->shape->setFillColor(*color);

	this->setOrigin(0, 0);
	this->setPosition(position->x, position->y);
	this->setOrigin(r, r);
}

Enemy::~Enemy() {
	delete shape;
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform = getTransform();
	target.draw(*shape, states);
}

void Enemy::update() {
	Entity::update();

	if(!wasInside)
		if(position->x > 0 && position->x < Settings::Width &&
				position->y > 0 && position->y < Settings::Height)
			this->wasInside = true;

	if(wasInside)
		if(position->x < 0 || position->x > Settings::Width ||
				position->y < 0 || position->y > Settings::Height) {
			this->destroy();
			return;
		}
}

#endif
