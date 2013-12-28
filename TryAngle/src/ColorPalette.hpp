/*
 * ColorPalette.hpp
 *
 *  Created on: Dec 23, 2013
 *      Author: Renato Lui Geh
 */

#ifndef COLORPALETTE_HPP_
#define COLORPALETTE_HPP_

#include <cmath>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "Component.hpp"

class ColorPalette : public Component {
	private:
		std::string str;

		sf::VertexArray rgb;
		sf::VertexArray bw;

		sf::CircleShape rgb_c;
		sf::CircleShape bw_c;

		sf::CircleShape selected;
		sf::Text label;

		sf::Color color;

		double linear[6];
		double angular[6];
	public:
		ColorPalette(double, double, sf::Color);
		~ColorPalette(void);
	public:
		virtual bool onEvent(const sf::Event&);
		virtual void update(const sf::Time&);
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	private:
		void refresh(void);
	private:
		void loadCoefficients(const sf::VertexArray&);
		void identify(const sf::VertexArray&, double, double);
	private:
		static const sf::Color& parse(sf::Color&, std::string&);
		static const std::string& interpret(const sf::Color&, std::string&);
	private:
		static std::ostringstream conv;
		inline static double dist(double, double, double, double);
		inline static double distSq(double, double, double, double);
};

std::ostringstream ColorPalette::conv;

ColorPalette::ColorPalette(double x=0, double y=0,
		sf::Color def_color=sf::Color::Blue) : Component(x, y, 150, 500),
				rgb(sf::Triangles, 3), bw(sf::Triangles, 3),
				rgb_c(3, 4), bw_c(3, 4),
				selected(10, 4), label("", Settings::DEF_TEXT_FONT),
				color(def_color), linear(), angular() {

	rgb_c.setFillColor(sf::Color(0, 0, 0, 0));
	rgb_c.setOutlineColor(Utility::Color::getInverseColor(color));

	rgb_c.setOutlineThickness(2);
	rgb_c.setOrigin(rgb_c.getRadius(), rgb_c.getRadius());
	rgb_c.setRotation(45);

	bw_c.setFillColor(sf::Color(0, 0, 0, 0));
	bw_c.setOutlineColor(Utility::Color::getInverseColor(color));

	bw_c.setOutlineThickness(2);
	bw_c.setOrigin(bw_c.getRadius(), bw_c.getRadius());
	bw_c.setRotation(45);

	float width = getSize().x;
	float height = width*sqrt(3)/2;

	rgb[0].position = sf::Vector2<float>(x + width/2, y);
	rgb[1].position = sf::Vector2<float>(x + width, y + height);
	rgb[2].position = sf::Vector2<float>(x, y + height);

	bw[0].position = sf::Vector2<float>(x, y + height + 10);
	bw[1].position = sf::Vector2<float>(x + width, y + height + 10);
	bw[2].position = sf::Vector2<float>(x + width/2, y + 2*height + 10);

	rgb[0].color = sf::Color::Red;
	rgb[1].color = sf::Color::Green;
	rgb[2].color = sf::Color::Blue;

	bw[0].color = sf::Color::Black;
	bw[1].color = sf::Color::White;
	bw[2].color = color;

	loadCoefficients(rgb);
	loadCoefficients(bw);

	selected.setPosition(x, bw[2].position.y + 25);

	selected.setFillColor(color);
	selected.setOutlineColor(sf::Color::Black);

	selected.setOutlineThickness(2);
	selected.setOrigin(selected.getRadius(), selected.getRadius());
	selected.setRotation(45);

	label.setString(ColorPalette::interpret(color, str));
	label.setPosition(x + 2*selected.getRadius() + 5,
			selected.getPosition().y-label.getGlobalBounds().height/2);

	label.setColor(color);
}

ColorPalette::~ColorPalette(void) {}

bool ColorPalette::onEvent(const sf::Event& event) {
	return false;
}

void ColorPalette::update(const sf::Time& dt) {
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		double x = Settings::mouse_position.x;
		double y = Settings::mouse_position.y;

		if((linear[0]*x+angular[0]-y)<=0 &&
			(linear[1]*x+angular[1]-y)>=0 &&
			(linear[2]*x+angular[2]-y)<=0) {
			rgb_c.setPosition(x, y);
			identify(rgb, x, y);
			refresh();
		} else if((linear[3]*x+angular[3]-y)<=0 &&
			(linear[4]*x+angular[4]-y)>=0 &&
			(linear[5]*x+angular[5]-y)>=0) {
			bw_c.setPosition(x, y);
		}
	}
}

inline double ColorPalette::distSq(double x1, double y1,
		double x2, double y2) {
	double dx = x2-x1, dy = y2-y1;
	return dx*dx + dy*dy;
}

inline double ColorPalette::dist(double x1, double y1,
		double x2, double y2) {
	return sqrt(distSq(x1, y1, x2, y2));
}

void ColorPalette::identify(const sf::VertexArray& canvas,
		double x, double y) {
	sf::Uint8* comps[3];
	int i;

	comps[0] = &color.r;
	comps[1] = &color.g;
	comps[2] = &color.b;

	for(i=0;i<3;++i) {
		double x1 = canvas[i].position.x, y1 = canvas[i].position.y;

		double a = (y1-y)/(x1-x);
		double b = y1-x1*((y1-y)/(x1-x));

		double c = linear[(i+1)%3];
		double d = angular[(i+1)%3];

		double x2 = (d-b)/(a-c);
		double y2 = a*x2+b;

		double AP = distSq(x1, y1, x, y);
		double PQ = distSq(x, y, x2, y2);

		*comps[i] = 255-(255*AP/(AP+PQ));
	}
}

void ColorPalette::refresh(void) {
	selected.setFillColor(color);
	label.setColor(color);

	label.setString(ColorPalette::interpret(color, str));

	rgb_c.setOutlineColor(Utility::Color::getInverseColor(color));
	bw_c.setOutlineColor(Utility::Color::getInverseColor(color));
}

void ColorPalette::draw(sf::RenderTarget& tar, sf::RenderStates st) const {
	tar.draw(rgb, st);
	tar.draw(bw, st);
	tar.draw(rgb_c, st);
	tar.draw(bw_c, st);
	tar.draw(selected, st);
	tar.draw(label, st);
}

#define isNumber(c) ((c)>='0'&&(c)<='9')

const sf::Color& ColorPalette::parse(sf::Color& color, std::string& str) {
	int n=0, k=1, i=0;
	sf::Uint8* comps[3];
	std::string::iterator it(str.begin());

	comps[0] = &(color.r);
	comps[1] = &(color.g);
	comps[2] = &(color.b);

	while(it!=str.end()) {
		while(it!=str.end() && !isNumber(*it)) ++it;

		if(it==str.end()) break;

		while(isNumber(*it)) {
			n+=((*it)-'0')*k;
			k*=10;
		}

		*(comps[i++]) = n;

		k=1, n=0;
	}

	return color;
}

#undef isNumber

const std::string& ColorPalette::interpret(
		const sf::Color& color, std::string& str) {

	conv.write("[ ", 2);
	conv << int(color.r);
	conv.write(" ; ", 3);
	conv << int(color.g);
	conv.write(" ; ", 3);
	conv << int(color.b);
	conv.write(" ]", 2);

	str.assign(conv.str());

	conv.str("");

	return str;
}

void ColorPalette::loadCoefficients(const sf::VertexArray& shape) {
	int n = (&shape==&rgb)?0:3;

	for(int i=1;i<4;++i) {
		double x1 = shape[(i-1)%3].position.x;
		double y1 = shape[(i-1)%3].position.y;

		double x2 = shape[i%3].position.x;
		double y2 = shape[i%3].position.y;

		linear[n+i-1] = (y1-y2)/(x1-x2);
		angular[n+i-1] = y1-x1*((y1-y2)/(x1-x2));
	}
}

#endif
