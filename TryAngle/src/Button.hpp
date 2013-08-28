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
		static constexpr sf::Font* FONT;
		static constexpr double MAX_ANGLE;
	private:
		sf::Color* color;
		sf::Color* invColor;
		sf::VertexArray shape;
		sf::Text label;

		/** State: button state
		 *	true: button has been pressed but not released
		 *	false: button has not been pressed nor released
		 *	tralse: your mom has been released. From jail. To my home.
		 */
		bool state;
		bool hover;

		double angle;
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
	public:
		static void onCleanup(void) {delete Button::FONT;}
};

sf::Font* Button::FONT = nullptr;
double Button::MAX_ANGLE = math::PI/6;

Button::Button(double x, double y, std::string title) :
		Component(x, y, 150, 150), shape(sf::Quads, 4) {

	color = new sf::Color;
	invColor = new sf::Color;

	*color = Utility::Random::getRandomColor();
	*invColor = Utility::Random::getInverseColor(*color);

	angle = 0;
	state = false;
	hover = false;

	shape[0].position = sf::Vector2<float>(x, y);
	shape[1].position = sf::Vector2<float>(x+150, y);
	shape[2].position = sf::Vector2<float>(x+150, y+150);
	shape[3].position = sf::Vector2<float>(x, y+150);

	if(Button::FONT == nullptr) {
		Button::FONT = new sf::Font();
		Button::FONT->loadFromFile("src/resources/fonts/abduction2002.ttf");
	}

	label.setColor(*invColor);
	label.setFont(*Button::FONT);
	label.setCharacterSize(30);
	label.setString(title);

	sf::FloatRect bounds = label.getLocalBounds();

	label.setPosition(
			(float)pos.x + (bounds.width*title.size()-size.x)/2,
			(float)pos.y + bounds.height/2);

	label.setOrigin(bounds.width/2, bounds.height/2);

	this->refresh();
}

Button::~Button(void) {
	delete color;
	delete invColor;
}

void Button::refresh(void) {
	for(int i=0;i<4;i++)
		shape[i].color = *color;
	label.setColor(*invColor);
}

bool Button::isInside(double x, double y) {
	return (x>this->pos.x && x<(this->pos.x+this->size.x)) &&
			(y>this->pos.y && y<(this->pos.y+this->size.y));
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
		}
	}

	return false;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(shape, states);
	target.draw(label, states);
}

void Button::update(const sf::Time& dt) {
	if(!hover) {
		angle = 0;
		label.setRotation(0);
		return;
	}

	double theta =
			angle>Button::MAX_ANGLE?5*dt.asSeconds():-5*dt.asSeconds();
	label.rotate(theta);
	this->angle += theta;
	if(angle > 2*Button::MAX_ANGLE)
		angle = -Button::MAX_ANGLE;
}

#endif
