/*
 * TextField.hpp
 *
 *  Created on: Dec 16, 2013
 *      Author: Renato Lui Geh
 */

#ifndef TEXTFIELD_HPP_
#define TEXTFIELD_HPP_

#include <string>

#include <SFML/Graphics.hpp>

#include "Component.hpp"

class TextField : public Component {
	private:
		sf::Text str;

		int width;
		bool focus;

		sf::Color focus_c;
		sf::Color unfocus_c;
	public:
		TextField(const std::string&, double, double, int, int, const sf::Font&);
		~TextField(void);
	public:
		inline void setText(const std::string&);
		inline void addText(const std::string&);
		inline void addChar(sf::Uint32);
	public:
		virtual void update(const sf::Time&);
		virtual bool onEvent(const sf::Event&);
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	public:
		virtual void setPosition(double x, double y);
	public:
		virtual bool hasFocus(void) const;
		const sf::Color& getFocusColor(void) const;
		const sf::Color& getUnfocusColor(void) const;
		const sf::Color& getColor(void) const;
	private:
		std::string format(const std::string&);
};

TextField::TextField(const std::string& text = "",
		double x = 0, double y = 0, int _width = 0, int size = 40,
		const sf::Font& font = Settings::DEF_TEXT_FONT) :
		Component(x, y, 0, 0),
		str(text, font, size),
		width(_width), focus(false),
		focus_c(0, 0, 0), unfocus_c(50, 50, 50) {
	str.setString(format(text));

	setPosition(x, y);

	sf::Rect<float> bounds = str.getGlobalBounds();

	setSize(bounds.width, bounds.height);

	str.setColor(unfocus_c);
}

TextField::~TextField(void) {}

std::string TextField::format(const std::string& block) {
	if(width<=0) return block;

	std::string line;
	std::string text(block);
	std::string::iterator st, end, aux = end = st = text.begin();

	while(end!=text.end()) {
		while(end!=text.end() && *end!=' ') ++end;

		if(end==text.end()) break;

		line.assign(st, end);
		str.setString(line);

		if(str.getLocalBounds().width > width) {
			*aux = '\n';
			st = aux+1;
		}

		aux = end;
	}

	return text;
}

inline void TextField::setText(const std::string& block) {
	str.setString(format(block));
}
inline void TextField::addText(const std::string& block) {
	str.setString(str.getString() + format(block));
}

inline void TextField::addChar(sf::Uint32 c) {
	str.setString(str.getString() + c);
}

void TextField::update(const sf::Time& dt) {}

bool TextField::onEvent(const sf::Event& event) {
	if(event.type == sf::Event::MouseButtonPressed) {
		bool state = str.getGlobalBounds().contains(Settings::mouse_position);

		if(state!=focus) {
			focus = state;
			str.setColor(focus?focus_c:unfocus_c);

			return true;
		}
	} else if(focus && event.type == sf::Event::TextEntered) {
		if(event.text.unicode == '\b') {
			if(!str.getString().isEmpty())
				setText(str.getString().toAnsiString().
						erase(str.getString().getSize()-1, 1));
		} else
			addChar(event.text.unicode);
	}

	return false;
}

void TextField::draw(sf::RenderTarget& target, sf::RenderStates st) const {
	target.draw(str, st);
}

void TextField::setPosition(double x, double y) {
	Component::setPosition(x, y);
	str.setPosition(x, y);
}

bool TextField::hasFocus(void) const {return focus;}

const sf::Color& TextField::getFocusColor(void) const {return focus_c;}
const sf::Color& TextField::getUnfocusColor(void) const {return unfocus_c;}

const sf::Color& TextField::getColor(void) const {return str.getColor();}

#endif
