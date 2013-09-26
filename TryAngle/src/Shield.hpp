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
	private:
		Entity* owner;

		double max_shield;
		double shield;
		double rate;
		double mitigation;
		double fixed;

		sf::Color shield_color;
		sf::Color outline_color;

		sf::CircleShape* shape;
	public:
		Shield(Entity*, double, double, double);
		virtual ~Shield(void);
	public:
		double& getShield(void);
		double& getMitigation(void);
		double& getMaxShield(void);
		const sf::Color& getColor(void) const;
		const sf::Color& getOutlineColor(void) const;
	public:
		void setShield(double);
		void setMitigation(double);
		void setMaxShield(double);
		void setColor(sf::Uint8, sf::Uint8, sf::Uint8);
		void setOutlineColor(sf::Uint8, sf::Uint8, sf::Uint8);
	public:
		virtual void damage(double);
		virtual void recharge(double);
	public:
		void update(const sf::Time& dt);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
};

Shield::Shield(Entity* owner_ = nullptr, double shield_ = 25,
		double mitigation_ = 1.10, double rate_ = .01) :
		owner(owner_), max_shield(shield_), shield(shield_), rate(rate_),
		mitigation(mitigation_), fixed(shield_),
		shield_color(0, 60, 255, 125),
		outline_color(0, 10, 255, 100),
		shape(new sf::CircleShape(owner->getSize()->x/2 + 10)) {

	this->shape->setPointCount(30);
	this->shape->setFillColor(shield_color);
	this->shape->setOutlineThickness(1.5);
	this->shape->setOutlineColor(outline_color);

	this->shape->setOrigin(0, 0);
	this->shape->setPosition(-10, -10);
};

Shield::~Shield(void) {
	delete shape;
}

double& Shield::getShield(void) {return fixed;}
double& Shield::getMitigation(void) {return mitigation;}
double& Shield::getMaxShield(void) {return max_shield;}
const sf::Color& Shield::getColor(void) const {return shield_color;}
const sf::Color& Shield::getOutlineColor(void) const {return outline_color;}

void Shield::setShield(double shield) {this->shield = this->fixed = shield;}
void Shield::setMitigation(double mitigation) {
	this->mitigation = mitigation;
}
void Shield::setMaxShield(double m_shield) {max_shield = m_shield;}
void Shield::setColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b) {
	shield_color.r = r;
	shield_color.g = g;
	shield_color.b = b;
	this->shape->setFillColor(shield_color);
}
void Shield::setOutlineColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b) {
	outline_color.r = r;
	outline_color.g = g;
	outline_color.b = b;
	this->shape->setOutlineColor(outline_color);
}

void Shield::damage(double dam) {
	if(shield < 0) {
		owner->setHealth(owner->getHealth()-dam);
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
