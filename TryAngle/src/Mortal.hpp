/*
 * Mortal.hpp
 *
 *  Created on: Aug 2, 2013
 *      Author: Renato Lui Geh
 */

#ifndef MORTAL_HPP_
#define MORTAL_HPP_

class Mortal {
	protected:
		double max_health;
		double max_exp;

		double health;
		double exp;
		bool dead;

		unsigned short int level;
	public:
		Mortal(void);
		virtual ~Mortal(void) {};
	public:
		double getHealth(void) const;
		void setHealth(double);
		bool isDead(void) const;
	public:
		double getExp(void) const;
		void setExp(double);
		void addExp(double);
		void subExp(double);
	public:
		virtual void damage(double);
		virtual void heal(double);
		virtual bool handleDeath(void) = 0;
	public:
		void setMaxHealth(double);
		void setMaxExp(double);
		double getMaxHealth(void);
		double getMaxExp(void);
	public:
		virtual void setLevel(unsigned short int);
		virtual void addLevel(unsigned short int);
		virtual void subLevel(unsigned short int);
		virtual unsigned short int getLevel(void);

	friend class UserInterface;
};

Mortal::Mortal(void) : max_health(400), max_exp(500),
		health(max_health), exp(0), dead(false), level(0) {};

double Mortal::getHealth(void) const {return this->health;}
void Mortal::setHealth(double health) {
	if(health > max_health)
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
	if(this->health + pts > max_health)
		this->health = max_health;
	else
		this->health += pts;
}

double Mortal::getExp(void) const {return this->exp;}
void Mortal::setExp(double exp) {
	if(exp > max_exp || exp < 0)
		return;
	this->exp = exp;
}
void Mortal::addExp(double n) {
	if(this->exp + n > max_exp) {
		max_exp += max_exp*(level/10.);
		max_health += max_health*(level/50.);
		addLevel(1);

		this->exp = 0;
		this->health = max_health;
	} else
		this->exp += n;
}
void Mortal::subExp(double n) {
	if(this->exp - n < 0)
		this->exp = 0;
	else
		this->exp -= n;
}

bool Mortal::isDead(void) const {return health<=0;}

void Mortal::setMaxHealth(double m_health) {max_health = m_health;}
void Mortal::setMaxExp(double m_exp) {max_exp = m_exp;}

double Mortal::getMaxHealth(void) {return max_health;}
double Mortal::getMaxExp(void) {return max_exp;}

void Mortal::setLevel(unsigned short int n_level) {level=n_level;}
void Mortal::addLevel(unsigned short int i_level) {level+=i_level;}
void Mortal::subLevel(unsigned short int d_level) {level-=d_level;}
unsigned short int Mortal::getLevel(void) {return level;}

#endif
