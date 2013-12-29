/*
 * RadioSet.hpp
 *
 *  Created on: Dec 26, 2013
 *      Author: Renato Lui Geh
 */

#ifndef RADIOSET_HPP_
#define RADIOSET_HPP_

#include <string>
#include <initializer_list>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "Component.hpp"

class RadioButton : public Component {
	private:
		sf::Text label;

		sf::CircleShape outline;
		sf::CircleShape fill;

		bool selected;
		bool focus;
	public:
		RadioButton(const std::string&, double, double);
		~RadioButton(void);
	public:
		virtual void setPosition(double, double);
		virtual void select(void);
	public:
		bool onEvent(const sf::Event&);
		void update(const sf::Time&);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
	private:
		inline static bool contains(const sf::CircleShape&, Vector2D&);

	friend class RadioSet;
};

RadioButton::RadioButton(const std::string& name, double x=0, double y=0) :
	Component(x, y, 0, 0), label(name, Settings::DEF_TEXT_FONT),
	outline(5, 10), fill(3, 10), selected(false), focus(false) {

	outline.setFillColor(Utility::Color::TRANSPARENT);
	outline.setOutlineColor(Component::DEF_UNFOCUS_COLOR);

	fill.setFillColor(Component::DEF_UNFOCUS_COLOR);
	fill.setOutlineColor(Utility::Color::TRANSPARENT);

	outline.setOutlineThickness(3);

	outline.setOrigin(outline.getRadius(), outline.getRadius());
	outline.setRotation(90);

	fill.setOrigin(fill.getRadius(), fill.getRadius());
	fill.setRotation(90);

	outline.setPosition(x, y);
	fill.setPosition(x, y);

	sf::Rect<float> bounds(outline.getGlobalBounds());

	label.setPosition(x+2*outline.getRadius()+5, y-bounds.width/2);
	label.setColor(Component::DEF_UNFOCUS_COLOR);

	setSize(bounds.width+2*outline.getRadius()+5,
			std::max(bounds.height, 2*outline.getRadius()));
}

RadioButton::~RadioButton(void) {}

void RadioButton::setPosition(double x, double y) {
	Component::setPosition(x, y);

	outline.setPosition(x, y);
	fill.setPosition(x, y);
	label.setPosition(x+2*outline.getRadius()+5,
			y-outline.getGlobalBounds().width/2);
}

void RadioButton::select(void) {selected = true;}

bool RadioButton::onEvent(const sf::Event& event) {
	if(event.type == sf::Event::MouseButtonReleased)
		selected = contains(outline, Settings::mouse_position);
	else if(event.type == sf::Event::MouseMoved) {
		bool inside = contains(outline, Settings::mouse_position);

		if(inside != focus) {
			sf::Color c(((focus=inside))?DEF_FOCUS_COLOR:DEF_UNFOCUS_COLOR);

			outline.setOutlineColor(c);
			fill.setFillColor(c);
			label.setColor(c);
		}
	}

	return false;
}

void RadioButton::update(const sf::Time& dt) {}

void RadioButton::draw(sf::RenderTarget& tar, sf::RenderStates st) const {
	tar.draw(label, st);
	tar.draw(outline, st);

	if(selected)
		tar.draw(fill, st);
}

inline bool RadioButton::contains(
		const sf::CircleShape& shape, Vector2D& point) {
	double r = shape.getRadius();
	r*=r;
	return point.distSq(shape.getPosition())<=r;
}

class RadioSet : public Component {
	public:
		enum Layout {Horizontal, Vertical};
	private:
		sf::Text title;
		std::vector<RadioButton> buttons;
		Layout orientation;
	public:
		RadioSet(std::initializer_list<std::string>,
				double, double, const std::string&, Layout);
		~RadioSet(void);
	public:
		virtual void setPosition(double x, double y);
		void add(const std::string&);
		std::string getSelected(void) const;
		const std::vector<RadioButton>& getOptions(void) const;
		void randomize(void);
	public:
		bool onEvent(const sf::Event&);
		void update(const sf::Time&);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
};

RadioSet::RadioSet(std::initializer_list<std::string> list,
		double x=0, double y=0, const std::string& name="",
		Layout _orientation=Vertical) :
			Component(x, y, 0, 0), title(name, Settings::DEF_TEXT_FONT, 40),
			buttons(), orientation(_orientation) {
	for(std::string label : list)
		buttons.push_back(RadioButton(label, x, y));

	std::vector<RadioButton>::iterator it, prev;

	title.setPosition(x, y);
	title.setColor(Component::DEF_UNFOCUS_COLOR);

	y+=title.getGlobalBounds().height/2+25;
	x+=20;

	Vector2D ref(0, 0);

	for(it=buttons.begin();it!=buttons.end();++it) {
		if(orientation == Vertical)
			(*it).setPosition(x, y+ref.y+20);
		else
			(*it).setPosition(x+ref.x+20, y);

		ref.add((*it).getSize()).add(10, 10);
	}

	if(orientation == Horizontal)
		setSize(std::max(title.getGlobalBounds().width,
				(buttons.back().getPosition().x -
						buttons.front().getPosition().x) +
						buttons.back().getSize().x),
				title.getGlobalBounds().height +
				buttons.front().getSize().y);
	else {
		float width=0;

		for(auto it=buttons.begin();it!=buttons.end();++it)
			width = std::max((*it).getSize().x, width);

		setSize(std::max(title.getGlobalBounds().width, width),
				(buttons.back().getPosition().x -
						buttons.front().getPosition().x) +
						buttons.back().getSize().x);
	}

	buttons.front().select();
}

RadioSet::~RadioSet(void) {}

void RadioSet::setPosition(double x, double y) {
	Component::setPosition(x, y);

	double dx = x-pos.x;
	double dy = y-pos.y;

	title.move(dx, dy);

	for(auto it=buttons.begin();it!=buttons.end();++it)
		(*it).setPosition((*it).getPosition().x + dx,
				(*it).getPosition().y + dy);
}

void RadioSet::add(const std::string& label) {
	Vector2D _pos(buttons.back().getSize());

	if(orientation==Vertical)
		_pos.set(0, 5+_pos.y);
	else
		_pos.set(5+_pos.x, 0);

	_pos.add(&pos);

	buttons.push_back(RadioButton(label, _pos.x, _pos.y));
}

const std::vector<RadioButton>& RadioSet::getOptions(void) const {
	return buttons;
}

void RadioSet::randomize(void) {
	int n = Utility::Random::getUnsignedRandom(0, buttons.size());

	for(auto it=buttons.begin();it!=buttons.end();++it,--n) {
		if(n==0)
			(*it).select();
		else
			(*it).selected = false;
	}
}

bool RadioSet::onEvent(const sf::Event& event) {
	if(event.type == sf::Event::MouseButtonReleased) {
		bool any = false;

		for(auto it=buttons.begin();it!=buttons.end();++it)
			if(RadioButton::contains(
					(*it).outline, Settings::mouse_position))
				any = true;

		if(!any) return false;
	}

	for(auto it=buttons.begin();it!=buttons.end();++it)
		if((*it).onEvent(event))
			return true;

	return false;
}

void RadioSet::update(const sf::Time& dt) {
	for(auto it=buttons.begin();it!=buttons.end();++it)
		(*it).update(dt);
}

void RadioSet::draw(sf::RenderTarget& target, sf::RenderStates st) const {
	target.draw(title, st);

	for(auto it=buttons.begin();it!=buttons.end();++it)
		target.draw(*it, st);
}

std::string RadioSet::getSelected(void) const {
	for(auto it=buttons.begin();it!=buttons.end();++it)
		if((*it).selected)
			return (*it).label.getString();

	return std::string();
}

#endif
