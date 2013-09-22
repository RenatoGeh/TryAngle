/*
 * Shield.hpp
 *
 *  Created on: Sep 19, 2013
 *      Author: Renato Lui Geh
 */

#ifndef SHIELD_HPP_
#define SHIELD_HPP_

#include "Mortal.hpp"

class Shield : public sf::Drawable {
	public:
		static const double MAX_SHIELD;
	private:
		Entity* owner;

		double max_shield;
		double shield;
		double rate;
		double mitigation;
		double fixed;

		sf::CircleShape* shape;
	public:
		Shield(Entity*, double, double, double);
		virtual ~Shield(void);
	public:
		double& getShield(void);
		double& getMitigation(void);
	public:
		void setShield(double);
		void setMitigation(double);
	public:
		virtual void damage(double);
		virtual void recharge(double);
	public:
		void update(const sf::Time& dt);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
};

const double Shield::MAX_SHIELD = 75;

Shield::Shield(Entity* owner_ = nullptr, double shield_ = 25,
		double mitigation_ = 1.10, double rate_ = .01) :
		owner(owner_), max_shield(shield_), shield(shield_), rate(rate_),
		mitigation(mitigation_), fixed(shield_) {

	this->shape = new sf::CircleShape(owner->getSize()->x/2 + 10);
	this->shape->setPointCount(30);
	this->shape->setFillColor(sf::Color(0, 60, 255, 125));
	this->shape->setOutlineThickness(1.5);
	this->shape->setOutlineColor(sf::Color(0, 10, 255, 100));

	this->shape->setOrigin(0, 0);
	this->shape->setPosition(-10, -10);
};

Shield::~Shield(void) {
	delete shape;
}

double& Shield::getShield(void) {return fixed;}
double& Shield::getMitigation(void) {return mitigation;}

void Shield::setShield(double shield) {this->shield = this->fixed = shield;}
void Shield::setMitigation(double mitigation) {
	this->mitigation = mitigation;
}

void Shield::damage(double dam) {
	if(shield < 0) {
		owner->setHealth(-dam);
		return;
	}

	dam *= mitigation-1;
	shield -= dam;

	if(shield < 0) {
		if(owner->getHealth() + shield < 0) owner->setHealth(0);
		else owner->setHealth(owner->getHealth() + shield);
	}
}

void Shield::recharge(double pts) {this->shield += pts;}

void Shield::update(const sf::Time& dt) {
	if(shield < max_shield) shield += rate;
	fixed = shield<0?0:shield;
}

void Shield::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	if(shield > 0)
		target.draw(*shape, state);
}

#endif
