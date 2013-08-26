/*
 * Component.hpp
 *
 *  Created on: Aug 25, 2013
 *      Author: Renato Lui Geh
 */

#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Utility.hpp"
#include "Vector2D.hpp"

class Component : public sf::Drawable {
	public:
		static unsigned long int global_ids;
		static unsigned long int recursive_id(unsigned long int);
		static unsigned long int nextID(void);
	private:
		unsigned long int id;
		Vector2D pos;
		Vector2D size;
	public:
		Component(double, double, double, double);

	public:
		Vector2D getPosition(void) const;
		void setPosition(double, double);
		Vector2D getSize(void) const;
		void setSize(double, double);
	public:
		virtual bool onEvent(const sf::Event&) = 0;
		virtual void update(const sf::Time& dt) {};
};

unsigned long int Component::global_ids = 0;

Component::Component(double x, double y, double w, double h) {
	this->pos.set(x, y);
	this->size.set(x, y);

	this->id = Component::nextID();
}

Vector2D Component::getPosition(void) const {return pos;}
Vector2D Component::getSize(void) const {return size;}

void Component::setPosition(double x, double y) {this->pos.set(x, y);}
void Component::setSize(double x, double y) {this->size.set(x, y);}

unsigned long int Component::recursive_id(unsigned long int global) {
	if(global%2==0) return 0;
	else return Component::recursive_id(global/2)+1;
}

unsigned long int Component::nextID(void) {
	unsigned long int id = Component::recursive_id(Component::global_ids);
	Component::global_ids += math::pow(2ul, id);
	return id;
}

#endif
