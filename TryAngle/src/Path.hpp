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
	private:
		Entity* parent;
		std::queue<Vector2D> navpts;
		Vector2D* current;
	public:
		Path(Entity*);
		~Path(void);
	public:
		Vector2D* getCurrentMarker(void);
		void setCurrentMarker(double, double);
		std::queue<Vector2D>* getNavPoints(void);
	public:
		void push(double, double);
		void pop(void);
	public:
		void update(sf::Time);
	public:
		bool isEmpty(void);
};

Path::Path(Entity* parent) {
	this->parent = parent;

	this->navpts = std::queue<Vector2D>();
	this->current = NULL;
}

Path::~Path(void) {
	delete current;
}

void Path::update(sf::Time dt) {
	if(this->current == NULL)
		return;

	float x = this->parent->getPosition().x;
	float y = this->parent->getPosition().y;

	if(this->current->distSq(x, y) < 25) {
		this->navpts.pop();
		this->current = this->navpts.empty()?NULL:&(this->navpts.front());

		if(current==NULL)
			this->parent->setSpeed(0, 0);
		else
			this->parent->moveTo(this->current->x, this->current->y);
	}
}

void Path::push(double x, double y) {
	if(this->navpts.empty()) {
		this->current = new Vector2D(x, y);
		this->navpts.push(*(this->current));
		std::cout << this->current << std::endl;
		this->parent->moveTo(x, y);
		return;
	}
	std::cout << x << '\t' << y << std::endl;
	this->navpts.push(Vector2D(x, y));
}
void Path::pop(void) {this->navpts.pop();}

Vector2D* Path::getCurrentMarker(void) {return this->current;}
void Path::setCurrentMarker(double x, double y) {this->current->set(x, y);}
std::queue<Vector2D>* Path::getNavPoints(void) {return &navpts;}

bool Path::isEmpty(void) {return this->navpts.empty();}

#endif
