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
#include "Timer.hpp"

class Enemy : public Entity {
	private:
		bool wasInside;
		math::byte clockwise;
	protected:
		sf::CircleShape* shape;
	private:
		ActionTimer<void(void)>* shooter;
	public:
		Enemy(double, double, double, double, double);
		~Enemy(void);
	public:
		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void update(sf::Time dt);
};

Enemy::Enemy(double x, double y, double r=30, double vx=0, double vy=0) :
		Entity("Enemy", x, y, 2*r, 2*r, vx, vy){
	this->shape = new sf::CircleShape(r);
	this->color = new sf::Color(Utility::Random::getRandomColor());
	this->wasInside = false;

	this->shape->setPointCount(3);
	this->shape->setFillColor(*color);

	this->setOrigin(0, 0);
	this->setPosition(position->x, position->y);
	this->setOrigin(r, r);

	this->clockwise = Utility::Random::getRandomSign(false);

	this->shooter = new ActionTimer<void(void)>(sf::seconds, 1.5f, true, 0.0f,
			[&]() {this->shoot();}, true);
}

Enemy::~Enemy() {
	delete shape;
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform = getTransform();
	target.draw(*shape, states);
}

void Enemy::update(sf::Time dt) {
	Entity::update(dt);

	this->angle += double(this->clockwise)/25;

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
