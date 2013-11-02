/*
 * Item.hpp
 *
 *  Created on: Aug 12, 2013
 *      Author: Renato Lui Geh
 */

#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <functional>
#include "Entity.hpp"

template <typename Fn> class Item : public Entity {
	public:
		enum class Predefined : math::u_byte {Health=0, Exp=1, Level=2};
	private:
		std::function<Fn> action;
		sf::CircleShape* shape;
	public:
		Item(double, double, Item::Predefined, double);
		Item(double, double, std::function<Fn>, double);
		~Item(void);
	public:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
		virtual void update(const sf::Time&);
		virtual void activate(void);
	private:
		static void healthAction(void);
		static void expAction(void);
		static void levelAction(void);
	public:
		virtual Entity::Type getID(void);
};

template <typename Fn>
	Item<Fn>::Item(double x, double y, Item::Predefined type, double r=10) :
	Entity("Item", x, y, 2*r, 2*r) {

	this->action = type==Item::Predefined::Health?healthAction:
			type==Item::Predefined::Exp?expAction:
					levelAction;
	this->shape = new sf::CircleShape();

	this->shape->setFillColor(
			type==Item::Predefined::Health?sf::Color::Red:
					type==Item::Predefined::Exp?sf::Color::Yellow:
							sf::Color::Cyan);

	this->shape->setPointCount(type==Item::Predefined::Health?5:
			type==Item::Predefined::Exp?6:7);
	this->shape->setRadius(r);
	this->shape->setOutlineColor(sf::Color::Black);
	this->shape->setOutlineThickness(2);
}

template <typename Fn>
	Item<Fn>::Item(double x, double y, std::function<Fn> action, double r=10) :
	Entity("Item", x, y, 2*r, 2*r) {

	this->action = action;
	this->shape = new sf::CircleShape(5);

	this->shape->setFillColor(sf::Color::Red);
}

template <typename Fn> Item<Fn>::~Item(void) {
	delete shape;
}

template <typename Fn>
	void Item<Fn>::draw(
			sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform = this->getTransform();
	target.draw(*shape, states);
}

template <typename Fn> void Item<Fn>::update(const sf::Time& dt) {
	if(!active) return;

	Entity::update(dt);

	if(this->intersects(Player::getPlayer())) {
		this->activate();
		this->destroy();
	}
}

template <typename Fn> void Item<Fn>::activate(void) {this->action();}

template <typename Fn> Entity::Type Item<Fn>::getID(void) {
	return Entity::Type::Item;
}

template <typename Fn> void Item<Fn>::expAction(void) {
	Player::getPlayer()->addExp(50);
}

template <typename Fn> void Item<Fn>::levelAction(void) {
	Player::getPlayer()->addExp(Player::getPlayer()->getMaxExp());
}

template <typename Fn> void Item<Fn>::healthAction(void) {
	Player::getPlayer()->heal(100);
}

#endif
