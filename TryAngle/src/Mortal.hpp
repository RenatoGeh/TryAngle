/*
 * Mortal.hpp
 *
 *  Created on: Aug 2, 2013
 *      Author: Renato Lui Geh
 */

#ifndef MORTAL_HPP_
#define MORTAL_HPP_

class Mortal {
	public:
		static constexpr double MAX_HEALTH = 800;
		static constexpr double MAX_EXP    = 500;
	protected:
		double health = 100;
		double exp = 0;
	public:
		virtual ~Mortal(void) {};
	public:
		double getHealth(void);
		void setHealth(double);
		bool isDead(void) const;
	public:
		double getExp(void);
		void setExp(double);
		void addExp(double);
		void subExp(double);
	public:
		void damage(double);
		void heal(double);
		virtual bool handleDeath(void) = 0;

	friend class UserInterface;
};

double Mortal::getHealth(void) {return this->health;}
void Mortal::setHealth(double health) {
	if(health > MAX_HEALTH || health < 0)
		return;
	this->health = health;
}

void Mortal::damage(double dam) {
	if(this->health - dam < 0)
		this->health = 0;
	else
		this->health -= dam;
}
void Mortal::heal(double pts) {
	if(this->health + pts > MAX_HEALTH)
		this->health = MAX_HEALTH;
	else
		this->health += pts;
}

double Mortal::getExp(void) {return this->exp;}
void Mortal::setExp(double exp) {
	if(exp > MAX_EXP || exp < 0)
		return;
	this->exp = exp;
}
void Mortal::addExp(double n) {
	if(this->exp + n > MAX_EXP)
		this->exp = MAX_EXP;
	else
		this->exp += n;
}
void Mortal::subExp(double n) {
	if(this->exp - n < 0)
		this->exp = 0;
	else
		this->exp -= n;
}

bool Mortal::isDead(void) const {return health<=0;}

#endif
