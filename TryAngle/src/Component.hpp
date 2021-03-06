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
	private:
		static math::u_id global_ids;
		static math::u_id recursive_id(math::u_id);
		static math::u_id nextID(void);
	private:
		math::u_id id;
	protected:
		Vector2D pos;
		Vector2D size;
	public:
		Component(double, double, double, double);
		~Component(void);
	public:
		virtual Vector2D getPosition(void) const;
		virtual void setPosition(double, double);
		virtual Vector2D getSize(void) const;
		virtual void setSize(double, double);

		math::u_id getID(void);
	public:
		virtual bool onEvent(const sf::Event&) = 0;
		virtual void update(const sf::Time&) = 0;
	protected:
		static const sf::Color DEF_UNFOCUS_COLOR;
		static const sf::Color DEF_FOCUS_COLOR;
};

math::u_id Component::global_ids = 0;

const sf::Color Component::DEF_UNFOCUS_COLOR(50, 50, 50);
const sf::Color Component::DEF_FOCUS_COLOR(0, 0, 0);

Component::Component(double x, double y, double w, double h) :
		id(math::u_id(this)), pos(x, y), size(w, h) {};

Component::~Component(void) {
	Component::global_ids -= math::pow(2ul, (unsigned long int)id);
}

Vector2D Component::getPosition(void) const {return pos;}
Vector2D Component::getSize(void) const {return size;}

void Component::setPosition(double x, double y) {this->pos.set(x, y);}
void Component::setSize(double x, double y) {this->size.set(x, y);}

math::u_id Component::getID(void) {return id;}

math::u_id Component::recursive_id(math::u_id global) {
	if(global%2==0) return 0;
	else return Component::recursive_id(global/2)+1;
}

math::u_id Component::nextID(void) {
	math::u_id id = Component::recursive_id(Component::global_ids);
	Component::global_ids += math::pow(math::u_id(2), (math::u_id)id);
	return id;
}

#endif
