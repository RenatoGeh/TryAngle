/*
 * Background.hpp
 *
 *  Created on: Aug 9, 2013
 *      Author: Renato Lui Geh
 */

#ifndef BACKGROUND_HPP_
#define BACKGROUND_HPP_

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Vector2D.hpp"
#include "Utility.hpp"
#include "Player.hpp"

class BTile : public sf::VertexArray {
	private:
		std::vector<Utility::Color::Pattern*> palette;
	public:
		BTile(sf::PrimitiveType, unsigned int);
		~BTile(void);
	public:
		void update(const sf::Time&);

	friend class Background;
};

BTile::BTile(sf::PrimitiveType type=sf::Quads, unsigned int vertices=0) :
	sf::VertexArray(type, vertices) {
	this->palette = std::vector<Utility::Color::Pattern*>();

	for(unsigned int i=0;i<vertices;++i)
		this->palette.push_back(new Utility::Color::Pattern());
}

BTile::~BTile(void) {
	while(!palette.empty()) {
		delete palette.back();
		palette.pop_back();
	}
}

void BTile::update(const sf::Time& dt) {
	for(unsigned int i=0;i<this->getVertexCount();++i)
		this->operator [](i).color = this->palette.at(i)->nextColor();
}

class Background : public sf::Drawable {
	private:
		static Background* back;
	private:
		std::vector<BTile*> tiles;
		Player* parent;
		unsigned short int level;
	public:
		Background(Player*);
		~Background(void);
	public:
		void update(const sf::Time&);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
	public:
		static void onUpdate(const sf::Time&);
		static void onRender(sf::RenderWindow*);
		static void onInit(Player*);
		static void onCleanup(void);
};

Background* Background::back = nullptr;

Background::Background(Player* p = Player::getPlayer()) {
	this->tiles = std::vector<BTile*>();
	this->parent = p;
	this->level = parent->getLevel();

	BTile* tile = new BTile(sf::Quads, 4);
	(*tile)[0].position = sf::Vector2<float>(0, 0);
	(*tile)[1].position = sf::Vector2<float>(Settings::Width, 0);
	(*tile)[2].position = sf::Vector2<float>(Settings::Width, Settings::Height);
	(*tile)[3].position = sf::Vector2<float>(0, Settings::Height);

	this->tiles.push_back(tile);
}

Background::~Background(void) {
	while(!tiles.empty()) {
		delete tiles.back();
		tiles.pop_back();
	}
}

void Background::update(const sf::Time& dt) {
	if(this->level != this->parent->getLevel()) {
		this->level = this->parent->getLevel();
		//TODO: Add BTiles.
	}

	for(auto it = tiles.begin();it!=tiles.end();++it)
		(*it)->update(dt);
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	for(auto it = tiles.begin();it!=tiles.end();++it)
		target.draw(*(*it), state);
}

void Background::onUpdate(const sf::Time& dt) {Background::back->update(dt);}
void Background::onRender(sf::RenderWindow* window) {
	window->draw(*Background::back);
}
void Background::onInit(Player *p = Player::getPlayer()) {
	if(Background::back == nullptr)
		Background::back = new Background(p);
}
void Background::onCleanup(void) {delete Background::back;}

#endif
