/*
 * Path.hpp
 *
 *  Created on: Aug 3, 2013
 *      Author: Renato Lui Geh
 */

#ifndef PATH_HPP_
#define PATH_HPP_

#include <queue>
#include "Vector2D.hpp"

class Path {
	public:
		static const math::u_byte DEFAULT_NAVPOINTS;
	private:
		Entity* parent;
		std::queue<Vector2D> navpts;
		Vector2D* current;
	public:
		Path(Entity*, bool);
		~Path(void);
	public:
		Vector2D* getCurrentMarker(void);
		void setCurrentMarker(double, double);
		std::queue<Vector2D>* getNavPoints(void);
	public:
		void push(double, double);
		void pop(void);
		void clear(void);
	public:
		void update(const sf::Time&);
	public:
		bool isEmpty(void) const;
};

const math::u_byte Path::DEFAULT_NAVPOINTS = 10;

Path::Path(Entity* parent, bool random=true) {
	this->parent = parent;
	this->current = nullptr;

	if(random) {
		for(int i=1;i<Path::DEFAULT_NAVPOINTS;++i)
			this->push(
					Utility::Random::getUnsignedRandom(0, Settings::Width),
					Utility::Random::getUnsignedRandom(0, Settings::Height));

		this->push(
				Utility::Random::getBoundRandom(-100, 0, Settings::Width, Settings::Width+100),
				Utility::Random::getBoundRandom(-100, 0, Settings::Height, Settings::Height+100));
	}
}

Path::~Path(void) {
	while(!navpts.empty())
		navpts.pop();

	current = nullptr;
}

void Path::update(const sf::Time& dt) {
	if(this->current == nullptr)
		return;

	float x = this->parent->getPosition().x;
	float y = this->parent->getPosition().y;

	if(this->current->distSq(x, y) < 25) {
		this->navpts.pop();

		this->current = this->navpts.empty()?nullptr:&(this->navpts.front());

		if(current==nullptr)
			this->parent->setSpeed(0, 0);
		else
			this->parent->moveTo(this->current->x, this->current->y);
	}
}

void Path::push(double x, double y) {
	if(this->navpts.empty()) {
		this->navpts.push(Vector2D(x, y));
		this->current = &(navpts.back());
		this->parent->moveTo(x, y);
		return;
	}
	this->navpts.push(Vector2D(x, y));
}
void Path::pop(void) {this->navpts.pop();}

void Path::clear(void) {
	while(!this->navpts.empty())
		this->navpts.pop();
}

Vector2D* Path::getCurrentMarker(void) {return this->current;}
void Path::setCurrentMarker(double x, double y) {this->current->set(x, y);}
std::queue<Vector2D>* Path::getNavPoints(void) {return &navpts;}

bool Path::isEmpty(void) const {return this->navpts.empty();}

#endif
