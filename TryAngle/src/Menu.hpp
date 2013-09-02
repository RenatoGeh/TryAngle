/*
 * Menu.hpp
 *
 *  Created on: Aug 25, 2013
 *      Author: Renato Lui Geh
 */

#ifndef MENU_HPP_
#define MENU_HPP_

#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.hpp"

class Menu : public sf::Drawable {
	private:
		std::vector<Component*> components;
		sf::Text title;
		bool active;
	public:
		Menu(std::string);
		~Menu(void);
	public:
		std::string getTitle(void) const;
		void setTitle(std::string);
	public:
		void add(Component*);
		void remove(Component*);
		void clear(void);
	public:
		virtual void onEvent(const sf::Event&) = 0;
		virtual void update(const sf::Time&);
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	protected:
		Component* getTriggered(const sf::Event&);
};

Menu::Menu(std::string title) {
	this->title.setString(title);
	this->title.setColor(Utility::Random::getRandomColor());
	this->title.setCharacterSize(40);

	this->active = true;
}

Menu::~Menu(void) {
	while(!components.empty()) {
		delete components.back();
		components.pop_back();
	}
}

std::string Menu::getTitle(void) const {return this->title.getString();}
void Menu::setTitle(std::string title) {this->title.setString(title);}

Component* Menu::getTriggered(const sf::Event& event) {
	for(auto it = components.begin();it!=components.end();++it)
		if((*it)->onEvent(event))
			return *it;
	return nullptr;
}

void Menu::update(const sf::Time& dt) {
	for(auto it = components.begin();it!=components.end();++it)
		(*it)->update(dt);
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(title, states);

	for(auto it = components.begin();it!=components.end();++it)
		target.draw(*(*it), states);
}

void Menu::add(Component* e) {
	components.push_back(e);
}

void Menu::remove(Component* e) {
	components.erase(std::remove_if(components.begin(), components.end(),
			[&](Component* comp){return e==comp;}), components.end());
}

void Menu::clear(void) {
	while(!components.empty()) {
		delete components.back();
		components.pop_back();
	}
}

#endif
