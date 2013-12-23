/*
 * Menu.hpp
 *
 *  Created on: Aug 25, 2013
 *      Author: Renato Lui Geh
 */

#ifndef MENU_HPP_
#define MENU_HPP_

#include <vector>
#include <stack>

#include <SFML/Graphics.hpp>
#include "Component.hpp"

class Menu : public sf::Drawable {
	private:
		std::vector<Component*> components;
		std::stack<Component*> stack;
		sf::Text title;
		bool active;
	public:
		Menu(std::string);
		~Menu(void);
	public:
		std::string getTitle(void) const;
		void setTitle(std::string);
		void setTitleSize(int);
	public:
		void add(Component*);
		void remove(Component*);
		void clear(void);
	public:
		virtual void onEvent(const sf::Event&) = 0;
		virtual void update(const sf::Time&);
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
		virtual void destroy(void);
	protected:
		void pushEvent(const sf::Event&);
		Component* popEvent(const sf::Event&);
		inline void clearEvent(void);
};

Menu::Menu(std::string title) {
	this->title.setFont(Settings::DEF_FONT);
	this->title.setString(title);
	this->title.setColor(sf::Color::Red);
	this->title.setCharacterSize(50*title.size()>Settings::Width?40:50);

	sf::FloatRect titleBounds(this->title.getLocalBounds());

	this->title.setOrigin(0, 0);
	this->title.setPosition((Settings::Width-titleBounds.width)/2, 50);

	this->active = true;
}

Menu::~Menu(void) {
	clear();
}

std::string Menu::getTitle(void) const {return this->title.getString();}
void Menu::setTitle(std::string title) {this->title.setString(title);}

void Menu::setTitleSize(int size) {
	title.setCharacterSize(size);

	sf::FloatRect titleBounds(this->title.getLocalBounds());

	title.setOrigin(0, 0);
	title.setPosition((Settings::Width-titleBounds.width)/2, 50);
}

void Menu::pushEvent(const sf::Event& event) {
	for(auto it = components.begin();it!=components.end();++it)
		if((*it)->onEvent(event))
			stack.push(*it);
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
	clearEvent();

	while(!components.empty()) {
		delete components.back();
		components.pop_back();
	}
}

void Menu::destroy(void) {this->active = false;}

namespace MenuUtils {
	void setMenu(Menu*);
	void mainMenu(void);
}

Component* Menu::popEvent(const sf::Event& event) {
	if(!active) return nullptr;

	if(stack.empty())
		pushEvent(event);

	if(stack.empty()) return nullptr;

	Component* e = stack.top();
	stack.pop();

	return e;
}

void Menu::clearEvent(void) {while(!stack.empty()) stack.pop();}

#endif
