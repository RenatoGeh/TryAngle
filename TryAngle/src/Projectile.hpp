/*
 * Projectile.hpp
 *
 *  Created on: Jul 29, 2013
 *      Author: Renato Lui Geh
 */

#ifndef PROJECTILE_HPP_
#define PROJECTILE_HPP_

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Vector2D.hpp"
#include "Entity.hpp"
#include "Player.hpp"

class Projectile : public Entity {
	protected:
		sf::CircleShape* shape;
	public:
		Projectile(Entity*, double, double, double, double, double);
		~Projectile(void);
	public:
		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void update(const sf::Time&);
	public:
		virtual Entity::Type getID(void);
};

Projectile::Projectile(Entity* parent, double x, double y, double vx, double vy, double r=2) :
		Entity("Projectile", x+1, y+1, 2*r, 2*r, vx, vy) {
	this->shape = new sf::CircleShape(r);
	this->color = new sf::Color(Utility::Random::getRandomColor());
	this->team = parent->getTeam();

	this->shape->setOutlineColor(sf::Color::Black);
	this->shape->setOutlineThickness(1.5);
	this->shape->setPointCount(4);
	this->shape->setFillColor(*color);

	this->setOrigin(0, 0);
	this->setPosition(position->x, position->y);
	this->setOrigin(r/2, r/2);
}

Projectile::~Projectile() {
	delete shape;
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform = getTransform();
	target.draw(*shape, states);
}

namespace ProjectileUtility {
	void transferExp(double);
}

void Projectile::update(const sf::Time& dt) {
	Entity::update(dt);

	if(!this->active) return;

	if(position->x < 0 || position->x > Settings::Width ||
			position->y < 0 || position->y > Settings::Height) {
		this->destroy();
		return;
	}

	std::vector<Entity*>* entities = Entity::getEntities();
	for(auto it = entities->begin(); it!=entities->end(); ++it) {
		Entity* e = *it;

		if(e->getID() == Entity::Type::Item)
			continue;

		if(e->getID() != Entity::Type::Projectile) {
			if(e->getTeam() != this->team)
				if(e->intersects(this)) {
					e->damage(30);
					if(e->isDead())
						ProjectileUtility::transferExp(10);
					this->destroy();
				}
		} else {
			//TODO: Projectile x Projectile collision. HIPPOPOTAMUS.
		}
	}
}

Entity::Type Projectile::getID(void) {return Entity::Type::Projectile;}

#endif
