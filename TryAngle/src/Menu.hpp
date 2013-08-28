/*
 * Menu.hpp
 *
 *  Created on: Aug 25, 2013
 *      Author: Renato Lui Geh
 */

#ifndef MENU_HPP_
#define MENU_HPP_

class Menu : public sf::Drawable {
	private:
		std::vector<Component*> components;
		sf::Text title;
	public:
		Menu(std::string);
		~Menu(void);
	public:
		std::string getTitle(void);
};

Menu::Menu(std::string title) {
	this->title.setString(title);
}

Menu::~Menu(void) {
	while(!components.empty()) {
		delete components.back();
		components.pop_back();
	}
}

std::string Menu::getTitle(void) {return this->title.getString();}

#endif
