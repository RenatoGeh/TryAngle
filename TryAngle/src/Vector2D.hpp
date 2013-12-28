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
		inline Vector2D* set(float, float);
		inline Vector2D* set(Vector2D*);
		inline Vector2D* reset(void);
	public:
		inline Vector2D* add(float, float);
		inline Vector2D* sub(float, float);
		inline Vector2D* mult(float, float);
		inline Vector2D* div(float, float);
	public:
		inline Vector2D* add(Vector2D*);
		inline Vector2D* sub(Vector2D*);
		inline Vector2D* mult(Vector2D*);
		inline Vector2D* div(Vector2D*);
	public:
		inline Vector2D& add(const Vector2D&);
		inline Vector2D& sub(const Vector2D&);
		inline Vector2D& mult(const Vector2D&);
		inline Vector2D& div(const Vector2D&);
	public:
		inline float distSq(Vector2D*);
		inline float distSq(float, float);
		inline float dist(Vector2D*);
		inline float dist(float, float);
		template <typename T> inline float distSq(const Vector2<T>&);
	public:
		inline bool null(void) const;
	public:
		friend std::ostream& operator << (std::ostream&, const Vector2D*);
		friend std::ostream& operator << (std::ostream&, const Vector2D&);
};

Vector2D::Vector2D(float x=0, float y=0) {
	this->x = x;
	this->y = y;
}

Vector2D::Vector2D(Vector2D* v) {
	this->x = v->x;
	this->y = v->y;
}

inline Vector2D* Vector2D::set(float x=0, float y=0) {
	this->x = x;
	this->y = y;
	return this;
}

inline Vector2D* Vector2D::set(Vector2D* v) {
	this->x = v->x;
	this->y = v->y;
	return this;
}

inline Vector2D* Vector2D::reset() {
	this->x = this->y = 0;
	return this;
}

inline Vector2D* Vector2D::add(float x, float y=0) {
	this->x += x;
	this->y += y;
	return this;
}

inline Vector2D* Vector2D::sub(float x, float y=0) {
	this->x -= x;
	this->y -= y;
	return this;
}

inline Vector2D* Vector2D::mult(float x, float y=1) {
	this->x *= x;
	this->y *= y;
	return this;
}

inline Vector2D* Vector2D::div(float x, float y=1) {
	this->x /= x;
	this->y /= y;
	return this;
}

inline Vector2D* Vector2D::add(Vector2D* v) {
	this->x += v->x;
	this->y += v->y;
	return this;
}

inline Vector2D* Vector2D::sub(Vector2D* v) {
	this->x -= v->x;
	this->y -= v->y;
	return this;
}

inline Vector2D* Vector2D::mult(Vector2D* v) {
	this->x *= v->x;
	this->y *= v->y;
	return this;
}

inline Vector2D* Vector2D::div(Vector2D* v) {
	this->x /= v->x;
	this->y /= v->y;
	return this;
}

inline Vector2D& Vector2D::add(const Vector2D& v) {
	x += v.x;
	y += v.y;
	return *this;
}

inline Vector2D& Vector2D::sub(const Vector2D& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

inline Vector2D& Vector2D::mult(const Vector2D& v) {
	x *= v.x;
	y *= v.y;
	return *this;
}

inline Vector2D& Vector2D::div(const Vector2D& v) {
	x /= v.x;
	y /= v.y;
	return *this;
}

inline float Vector2D::distSq(Vector2D* v) {
	float dx = v->x - this->x;
	float dy = v->y - this->y;
	return dx*dx + dy*dy;
}

inline float Vector2D::distSq(float x, float y) {
	float dx = x - this->x;
	float dy = y - this->y;
	return dx*dx + dy*dy;
}

template <typename T> inline float Vector2D::distSq(const sf::Vector2<T>& v) {
	float dx = v.x-x;
	float dy = v.y-y;
	return dx*dx + dy*dy;
}

inline float Vector2D::dist(Vector2D* v) {return sqrt(this->distSq(v));}
inline float Vector2D::dist(float x, float y) {return sqrt(this->distSq(x, y));}

inline bool Vector2D::null() const {return x==0 && y==0;}

std::ostream& operator << (std::ostream& stream, const Vector2D* v) {
	stream << '[' << v->x << ", " << v->y << ']';
	return stream;
}

std::ostream& operator << (std::ostream& stream, const Vector2D& v) {
	stream << '[' << v.x << ", " << v.y << ']';
	return stream;
}

#endif
