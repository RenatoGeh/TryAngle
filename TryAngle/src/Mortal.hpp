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
		double health = 100;
	public:
		virtual ~Mortal(void) {};
	public:
		double getHealth(void);
		void setHealth(double);
		bool isDead(void);
	public:
		void damage(double);
		void heal(double);
		virtual bool handleDeath(void) = 0;
};

double Mortal::getHealth(void) {return this->health;}
void Mortal::setHealth(double health) {this->health = health;}

void Mortal::damage(double dam) {this->health -= dam;}
void Mortal::heal(double pts) {this->health += pts;}

bool Mortal::isDead(void) {return health<0;}

#endif
