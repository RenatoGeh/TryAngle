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

class CustomMenu : public Menu {
	private:
		sf::Text name;
		sf::Text gender;
		sf::Text age;

		TextField* name_f;
		TextField* gender_f;
		TextField* age_f;

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
		gender("Gender: ", Settings::DEF_TEXT_FONT, 40),
		age("Age: ", Settings::DEF_TEXT_FONT, 40),
		name_f(new TextField(
				SentenceGenerator::getWord(SentenceGenerator::Noun))),
		gender_f(new TextField(
				SentenceGenerator::getWord(SentenceGenerator::Noun))),
		age_f(new TextField(
				static_cast<std::ostringstream*>(&(conv <<
				Utility::Random::getUnsignedRandom()%1001))->str())),
		save(new Button(
				Settings::Width/8,	7*Settings::Height/10, "Save")),
		load(new Button(
				7*Settings::Width/8, 7*Settings::Height/10, "Load")),
		random(new Button(
				Settings::Width/2, 7*Settings::Height/10, "Randomize!")),
		back(new Button(
				Settings::Width/2, 8*Settings::Height/10, "Back")) {

	conv.str("");

	name.setPosition(Settings::Width/2, Settings::Height/5);
	gender.setPosition(Settings::Width/2,
			name.getPosition().y+name.getGlobalBounds().height+50);
	age.setPosition(Settings::Width/2,
			gender.getPosition().y+gender.getGlobalBounds().height+50);

	name.setColor(name_f->getUnfocusColor());
	gender.setColor(gender_f->getUnfocusColor());
	age.setColor(age_f->getUnfocusColor());

	name_f->setPosition(double(name.getPosition().x +
			name.getGlobalBounds().width), name.getPosition().y);
	gender_f->setPosition(double(name.getPosition().x +
			gender.getGlobalBounds().width), gender.getPosition().y);
	age_f->setPosition(double(age.getPosition().x +
			age.getGlobalBounds().width), age.getPosition().y);

	add(save);
	add(load);
	add(random);
	add(back);

	add(name_f);
	add(gender_f);
	add(age_f);
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
	target.draw(gender, state);
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
			gender_f->setText(SentenceGenerator::getWord(SentenceGenerator::Noun));
			age_f->setText(conv.str());

			conv.str("");
		} else if(id == back->getID())
			MenuUtils::mainMenu();
		else if(id == name_f->getID())
			name.setColor(name_f->getColor());
		else if(id == age_f->getID())
			age.setColor(age_f->getColor());
		else if(id == gender_f->getID())
			gender.setColor(gender_f->getColor());
		else
			std::cerr << "Auto-destruction sequence activated. Nooooo!!!" << std::endl;
	}
}

CustomMenu* CustomMenu::generate(void) {return new CustomMenu();}

#endif
