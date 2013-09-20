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
#include "Path.hpp"
#include "Item.hpp"

class Enemy : public Entity {
	private:
		Path* nav;
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
		void setLevel(unsigned short int);
		void addLevel(unsigned short int);
		void subLevel(unsigned short int);
		unsigned short int getLevel(void);
	public:
		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void update(const sf::Time& dt);
	public:
		virtual Entity::Type getID(void);
		virtual bool handleDeath(void);
	public:
		Path* getPath(void);
};

Enemy::Enemy(double x, double y, double r=30, double vx=0, double vy=0) :
		Entity("Enemy", x, y, 2*r, 2*r, vx, vy) {
	this->shape = new sf::CircleShape(r);
	this->color = new sf::Color(Utility::Random::getRandomColor());
	this->wasInside = false;

	this->shape->setOutlineColor(sf::Color::Black);
	this->shape->setOutlineThickness(1.5);
	this->shape->setPointCount(3+Player::getPlayer()->getLevel());
	this->shape->setFillColor(*color);

	this->setOrigin(0, 0);
	this->setPosition(position->x, position->y);
	this->setOrigin(r, r);

	this->clockwise = Utility::Random::getRandomSign(false);

	this->shooter = new ActionTimer<void(void)>(sf::seconds, 1.0f, true,
			0.0f, [&](void) {this->shoot();}, false, false);
	this->shooter->setActive(false);

	this->nav = new Path(this);

	this->setHealth(30);
}

Enemy::~Enemy() {
	delete shape;

	shooter->setActive(false);
	Timer::refresh();

	delete shooter;
	delete nav;
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform = getTransform();
	target.draw(*shape, states);
}

void Enemy::update(const sf::Time& dt) {
	if(!active) return;

	Entity::update(dt);

	if(!this->shooter->isActive()) {
		this->shooter->setActive(true);
		Timer::add(this->shooter);
	}

	this->angle += (dt.asMilliseconds()/10.)*double(this->clockwise)/25;

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

	this->nav->update(dt);
}

void Enemy::setLevel(unsigned short int level) {
	if(this->level != level) {
		this->level = level;
		this->shape->setPointCount(3 + this->level);
	}
}

void Enemy::addLevel(unsigned short int increment = 1) {
	this->level += increment;
	this->shape->setPointCount(3 + this->level);
}
void Enemy::subLevel(unsigned short int decrement = 1) {
	if(this->level > 0) {
		this->level -= decrement;
		this->shape->setPointCount(3 + this->level);
	}
}

unsigned short int Enemy::getLevel(void) {return this->level;}

Entity::Type Enemy::getID(void) {return Entity::Type::Enemy;}

bool Enemy::handleDeath(void) {
	if(Entity::handleDeath()) {
		unsigned long int k = Utility::Random::getUnsignedRandom(0, 5);
		if(k==2) {
			math::u_byte choice = Utility::Random::getUnsignedRandom(0, 3);
			Entity::add(new Item<void(void)>(this->position->x, this->position->y,
					choice==0?Item<void(void)>::Predefined::Health:
							choice==1?Item<void(void)>::Predefined::Exp:
									Item<void(void)>::Predefined::Level));
		}
		return true;
	} else return false;
}

Path* Enemy::getPath(void) {return this->nav;}

namespace Utility {
	namespace Spawn {
		inline void enemy(void) {
			Entity::add(new Enemy(
					Random::getBoundRandom(-100, 0, Settings::Width, Settings::Width+100),
					Random::getBoundRandom(-100, 0, Settings::Height, Settings::Height+100)));
		}
	}
}

#endif
