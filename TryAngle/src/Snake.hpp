/*
 * Snake.hpp
 *
 *  Created on: Aug 17, 2013
 *      Author: Renato Lui Geh
 */

#ifndef SNAKE_HPP_
#define SNAKE_HPP_

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Vector2D.hpp"
#include "Enemy.hpp"

class Snake : public sf::Drawable {
	private:
		std::vector<Enemy*> pieces;
	public:
		Snake(double, double, int);
		~Snake(void);
	public:
		void push(double, double);
		void pop(void);
	public:
		std::vector<Enemy*>* getPieces(void);
};

Snake::Snake(double x, double y, int size) {

}

Snake::~Snake(void) {
	while(!pieces.empty()) {
		delete pieces.back();
		pieces.pop_back();
	}
}

void Snake::pop(void) {
	for(auto it = pieces.begin();it!=pieces.end();++it)
		(*it)->getPath()->pop();
}

void Snake::push(double x, double y) {
	for(auto it = pieces.begin();it!=pieces.end();++it)
		(*it)->getPath()->push(x, y);
}

std::vector<Enemy*>* Snake::getPieces(void) {return &pieces;}

#endif
