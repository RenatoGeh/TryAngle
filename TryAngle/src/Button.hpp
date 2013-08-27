/*
 * Button.hpp
 *
 *  Created on: Aug 25, 2013
 *      Author: Renato Lui Geh
 */

#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include "Component.hpp"

class Button : public Component {
	private:
		sf::Color color;
		sf::VertexArray shape;

		/** State: button state
		 *	true: button has been pressed but not released
		 *	false: button has not been pressed nor released
		 *	tralse: your mom has been released. From jail. At my home.
		 */
		bool state;
	public:
		Button(double, double);
	private:
		void refresh(void);
	public:
		bool isInside(double, double);
	public:
		bool onEvent(const sf::Event&);
		void update(const sf::Time&);
};

Button::Button(double x, double y) : Component(x, y, 150, 150) {
	color = Utility::Random::getRandomColor();
	shape = VertexArray(sf::Quads, 4);
	state = 0;

	shape[0].position = sf::Vector2<float>(x, y);
	shape[1].position = sf::Vector2<float>(x+150, y);
	shape[2].position = sf::Vector2<float>(x+150, y+150);
	shape[3].position = sf::Vector2<float>(x, y+150);

	this->refresh();
}

void Button::refresh(void) {
	for(int i=0;i<4;i++)
		shape[i].color = color;
}

bool Button::isInside(double x, double y) {
	return (x>this->pos.x && x<(this->pos.x+this->size.x)) &&
			(y>this->pos.y && y<(this->pos.y+this->size.y));
}

bool Button::onEvent(const sf::Event& event) {
	if(event.type == sf::Event::MouseButtonPressed) {
		if(this->isInside(event.mouseButton.x, event.mouseButton.y)) {
			this->color = Utility::Random::getInverseColor(this->color);
			this->refresh();
			this->state = true;
		}
	} else if(event.type == sf::Event::MouseButtonReleased) {
		if(state && isInside(event.mouseButton.x, event.mouseButton.y)) {
			this->color = Utility::Random::getInverseColor(this->color);
			this->refresh();
			this->state = false;
			return true;
		}
	}

	return false;
}

void Button::update(const sf::Time& dt) {

}

#endif
