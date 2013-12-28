/*
 * Button.hpp
 *
 *  Created on: Aug 25, 2013
 *      Author: Renato Lui Geh
 */

#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Component.hpp"

class Button : public Component {
	private:
		static const double MAX_ANGLE;
	private:
		sf::Color* color;
		sf::Color* invColor;
		sf::Text label;

		/** State: button state
		 *	true: button has been pressed but not released
		 *	false: button has not been pressed nor released
		 *	tralse: your mom has been released. From jail. To my home.
		 */
		bool state;
		bool hover;

		double angle;
		bool clockwise;
	public:
		Button(double, double, std::string);
		~Button(void);
	private:
		inline void swap(void) {
			sf::Color* temp = color;
			color = invColor;
			invColor = temp;
		}
		void refresh(void);
	public:
		bool isInside(double, double);
	public:
		bool onEvent(const sf::Event&);
		void update(const sf::Time&);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
};

const double Button::MAX_ANGLE = 10;

Button::Button(double x, double y, std::string title) :
		Component(x, y, 0, 0), state(false), hover(false),
		angle(0), clockwise(Utility::Random::getRandomBool()) {

	color = new sf::Color;
	invColor = new sf::Color;

	*color = Utility::Random::getRandomColor();
	*invColor = Utility::Color::getInverseColor(*color);

	label.setColor(*invColor);
	label.setFont(Settings::DEF_FONT);
	label.setCharacterSize(30);
	label.setString(title);

	sf::FloatRect bounds = label.getLocalBounds();

	label.setOrigin(bounds.width/2, bounds.height/2);
	label.setPosition(pos.x, pos.y);

	this->size.set(bounds.width, bounds.height);

	this->refresh();
}

Button::~Button(void) {
	delete color;
	delete invColor;
}

void Button::refresh(void) {
	label.setColor(*invColor);
}

bool Button::isInside(double x, double y) {
	return (x>(this->pos.x-this->size.x/2) && x<(this->pos.x+this->size.x/2)) &&
			(y>(this->pos.y-this->size.y/2) && y<(this->pos.y+this->size.y/2));
}

bool Button::onEvent(const sf::Event& event) {
	if(event.type == sf::Event::MouseButtonPressed) {
		if(this->isInside(event.mouseButton.x, event.mouseButton.y)) {
			this->swap();
			this->refresh();
			this->state = true;
		}
	} else if(event.type == sf::Event::MouseButtonReleased) {
		if(state && isInside(event.mouseButton.x, event.mouseButton.y)) {
			this->swap();
			this->refresh();
			this->state = false;
			return true;
		}
	} else if(event.type == sf::Event::MouseMoved) {
		bool in = isInside(event.mouseMove.x, event.mouseMove.y);
		if(in != hover) {
			this->swap();
			this->refresh();
			this->hover = in;

			if(!hover) {
				angle = 0;
				label.setRotation(0);
				clockwise = Utility::Random::getRandomBool();
			}
		}
	}

	return false;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(label, states);
}

void Button::update(const sf::Time& dt) {
	if(!hover) return;

	if(math::abs(angle)>Button::MAX_ANGLE) {
		angle = math::signum(angle)*Button::MAX_ANGLE;
		clockwise = !clockwise;
	}
	double theta = clockwise?10*dt.asSeconds():-10*dt.asSeconds();

	label.rotate(theta);
	this->angle += theta;
}

#endif
