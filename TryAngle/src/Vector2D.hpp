/*
 * Vector2D.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: Renato Lui Geh
 */

#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_

#include <cmath>
#include <string>
#include <iostream>
#include <SFML/System/Vector2.hpp>

class Vector2D : public sf::Vector2<float> {
	public:
		Vector2D(float, float);
		Vector2D(Vector2D*);
	public:
		Vector2D* set(float, float);
		Vector2D* set(Vector2D*);
		Vector2D* reset(void);
	public:
		Vector2D* add(float, float);
		Vector2D* sub(float, float);
		Vector2D* mult(float, float);
		Vector2D* div(float, float);
	public:
		Vector2D* add(Vector2D*);
		Vector2D* sub(Vector2D*);
		Vector2D* mult(Vector2D*);
		Vector2D* div(Vector2D*);
	public:
		float distSq(Vector2D*);
		float distSq(float, float);
		float dist(Vector2D*);
		float dist(float, float);
	public:
		std::string toString(void);
		bool null(void);
	public:
		friend std::ostream& operator << (std::ostream&, const Vector2D*);
};

Vector2D::Vector2D(float x=0, float y=0) {
	this->x = x;
	this->y = y;
}

Vector2D::Vector2D(Vector2D* v) {
	this->x = v->x;
	this->y = v->y;
}

Vector2D* Vector2D::set(float x=0, float y=0) {
	this->x = x;
	this->y = y;
	return this;
}

Vector2D* Vector2D::set(Vector2D* v) {
	this->x = v->x;
	this->y = v->y;
	return this;
}

Vector2D* Vector2D::reset() {
	this->x = this->y = 0;
	return this;
}

Vector2D* Vector2D::add(float x, float y=0) {
	this->x += x;
	this->y += y;
	return this;
}

Vector2D* Vector2D::sub(float x, float y=0) {
	this->x -= x;
	this->y -= y;
	return this;
}

Vector2D* Vector2D::mult(float x, float y=1) {
	this->x *= x;
	this->y *= y;
	return this;
}

Vector2D* Vector2D::div(float x, float y=1) {
	this->x /= x;
	this->y /= y;
	return this;
}

Vector2D* Vector2D::add(Vector2D* v) {
	this->x += v->x;
	this->y += v->y;
	return this;
}

Vector2D* Vector2D::sub(Vector2D* v) {
	this->x -= v->x;
	this->y -= v->y;
	return this;
}

Vector2D* Vector2D::mult(Vector2D* v) {
	this->x *= v->x;
	this->y *= v->y;
	return this;
}

Vector2D* Vector2D::div(Vector2D* v) {
	this->x /= v->x;
	this->y /= v->y;
	return this;
}

float Vector2D::distSq(Vector2D* v) {
	float dx = v->x - this->x;
	float dy = v->y - this->y;
	return dx*dx + dy*dy;
}

float Vector2D::distSq(float x, float y) {
	float dx = x - this->x;
	float dy = y - this->y;
	return dx*dx + dy*dy;
}

float Vector2D::dist(Vector2D* v) {return sqrt(this->distSq(v));}
float Vector2D::dist(float x, float y) {return sqrt(this->distSq(x, y));}

bool Vector2D::null() {return x==0 && y==0;}

std::ostream& operator << (std::ostream& stream, const Vector2D* v) {
	stream << "[" << v->x << ", " << v->y << "]";
	return stream;
}

#endif
