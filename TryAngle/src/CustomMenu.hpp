/*
 * CustomMenu.hpp
 *
 *  Created on: Dec 16, 2013
 *      Author: Renato Lui Geh
 */

#ifndef CUSTOMMENU_HPP_
#define CUSTOMMENU_HPP_

#include <sstream>

#include <SFML/Graphics.hpp>

#include "Menu.hpp"
#include "Button.hpp"
#include "Background.hpp"
#include "Timer.hpp"
#include "SentenceGenerator.hpp"
#include "TextField.hpp"
#include "ColorPalette.hpp"
#include "RadioSet.hpp"

class CustomMenu : public Menu {
	private:
		sf::Text name;
		sf::Text age;

		TextField* name_f;
		TextField* age_f;

		RadioSet* gender;
		ColorPalette* palette;

		Button* save;
		Button* load;
		Button* random;
		Button* back;
	private:
		CustomMenu(void);
		~CustomMenu(void);
	public:
		virtual void update(const sf::Time&);
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
		virtual void onEvent(const sf::Event&);
	public:
		static CustomMenu* generate(void);
	private:
		static std::ostringstream conv;
};

std::ostringstream CustomMenu::conv;

CustomMenu::CustomMenu(void) : Menu("Kust O' Mee Zay Shun"),
		name("Name: ", Settings::DEF_TEXT_FONT, 40),
		age("Age: ", Settings::DEF_TEXT_FONT, 40),
		name_f(new TextField(
				SentenceGenerator::getWord(SentenceGenerator::Noun))),
		age_f(new TextField(
				static_cast<std::ostringstream*>(&(conv <<
				Utility::Random::getUnsignedRandom()%1001))->str())),
		gender(new RadioSet({"Male", "Female", "Other"},
				Settings::Width/2, Settings::Height/5 +
				name.getGlobalBounds().height + 20, "Gender:")),
		palette(new ColorPalette(50, Settings::Height/5)),
		save(new Button(
				Settings::Width/8,	8*Settings::Height/10, "Save")),
		load(new Button(
				7*Settings::Width/8, 8*Settings::Height/10, "Load")),
		random(new Button(
				Settings::Width/2, 8*Settings::Height/10, "Randomize!")),
		back(new Button(
				Settings::Width/2, 9*Settings::Height/10, "Back")) {

	conv.str("");

	name.setPosition(Settings::Width/2, Settings::Height/5);
	age.setPosition(Settings::Width/2,
			gender->getPosition().y+gender->getSize().y+100);

	name.setColor(name_f->getUnfocusColor());
	age.setColor(age_f->getUnfocusColor());

	name_f->setPosition(double(name.getPosition().x +
			name.getGlobalBounds().width), name.getPosition().y);
	age_f->setPosition(double(age.getPosition().x +
			age.getGlobalBounds().width), age.getPosition().y);

	add(save);
	add(load);
	add(random);
	add(back);

	add(name_f);
	add(age_f);

	add(gender);
	add(palette);
}

CustomMenu::~CustomMenu(void) {}

void CustomMenu::update(const sf::Time& dt) {
	Menu::update(dt);
	Background::onUpdate(dt);
}

void CustomMenu::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	Background::onRender(&target);
	Menu::draw(target, state);

	target.draw(name, state);
	target.draw(age, state);
}

void CustomMenu::onEvent(const sf::Event& event) {
	Component* triggered = nullptr;

	while((triggered = popEvent(event))) {
		math::u_id id = triggered->getID();

		if(id == save->getID()) {

		} else if(id == load->getID()) {

		} else if(id == random->getID()) {
			conv << (Utility::Random::getUnsignedRandom()%1001);

			name_f->setText(SentenceGenerator::getWord(SentenceGenerator::Noun));
			age_f->setText(conv.str());

			conv.str("");
		} else if(id == back->getID())
			MenuUtils::mainMenu();
		else if(id == name_f->getID())
			name.setColor(name_f->getColor());
		else if(id == age_f->getID())
			age.setColor(age_f->getColor());
		else
			std::cerr << "Auto-destruction sequence activated. Nooooo!!!" << std::endl;
	}
}

CustomMenu* CustomMenu::generate(void) {return new CustomMenu();}

#endif
