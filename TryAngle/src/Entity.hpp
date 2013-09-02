/*
 * Entity.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: Renato Lui Geh
 */

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>
#include "Utility.hpp"
#include "Vector2D.hpp"
#include "Mortal.hpp"

class Entity : public sf::Drawable, public sf::Transformable, public Mortal {
	public:
		enum class Type : math::u_byte {Enemy, Player, Projectile, Item};
	protected:
		static std::vector<Entity*> entities;
		static std::vector<sf::Drawable*> paintables;
	protected:
		Vector2D* position;
		Vector2D* size;
		Vector2D* speed;

		bool team;
		bool active;

		double angle;

		unsigned short int level;

		std::string name;
		sf::Color* color;
	public:
		Entity(std::string, double, double, double, double, double, double);
		virtual ~Entity(void);
	public:
		static void add(Entity*);
		static void remove(Entity*);
		static void add(sf::Drawable*, bool);
		static void remove(sf::Drawable*, bool);
		static void clear(void);
	public:
		static std::vector<sf::Drawable*>* getPaintables(void);
		static std::vector<Entity*>* getEntities(void);
		static void paint(sf::RenderWindow*);
		static void onUpdate(const sf::Time&);
	public:
		virtual void destroy(void);
	public:
		bool isActive(void);
		Vector2D* getSize(void);
		Vector2D* getSpeed(void);
		sf::Color* getColor(void);
		bool getTeam(void);
		virtual Entity::Type getID(void) = 0;
	public:
		void setActive(bool);
		void setSize(double, double);
		void setSpeed(double, double);
		void setColor(sf::Uint8, sf::Uint8, sf::Uint8);
		void setTeam(bool);
	public:
		virtual void setLevel(unsigned short int) {};
		virtual void addLevel(unsigned short int) {};
		virtual void subLevel(unsigned short int) {};
		virtual unsigned short int getLevel(void) {return 0;};
	public:
		virtual void update(const sf::Time&);
		virtual bool intersects(Entity*);
		virtual void shoot(void);
		virtual void lookAt(double, double);
		virtual void moveTo(double, double);
	public:
		virtual bool handleDeath(void);
};

std::vector<Entity*> Entity::entities;
std::vector<sf::Drawable*> Entity::paintables;

Entity::Entity(std::string name, double x, double y, double w=0, double h=0,
		double vx=0, double vy=0) {
	this->name = name;
	this->color = NULL;
	this->angle = 0;

	this->position = new Vector2D(x, y);
	this->size = new Vector2D(w, h);
	this->speed = new Vector2D(vx, vy);
	this->team = false;

	this->active = false;
	this->level = 0;
}

Entity::~Entity() {
	delete color;
	delete position;
	delete size;
	delete speed;
}

void Entity::destroy() {this->active = false;}

bool Entity::handleDeath(void) {
	if(this->isDead()) {
		this->destroy();
		return true;
	}
	return false;
}

void Entity::add(Entity* e) {
	e->active = true;

	Entity::entities.push_back(e);
	Entity::paintables.push_back(e);
}

void Entity::remove(Entity* e) {
	e->active = false;
	for(std::vector<Entity*>::iterator it = Entity::entities.begin();
			it!=Entity::entities.end();++it)
		if(*it==e) {
			Entity::remove(*it, false);
			break;
		}
	delete e;
}

void Entity::add(sf::Drawable* e, bool diff=true) {
	Entity::paintables.push_back(e);
}

void Entity::remove(sf::Drawable* e, bool deletion=true) {
	for(std::vector<sf::Drawable*>::iterator it = Entity::paintables.begin();
			it!=Entity::paintables.end();++it)
		if(*it==e) {
			Entity::paintables.erase(it);
			break;
		}
	if(deletion)
		delete e;
}

std::vector<sf::Drawable*>* Entity::getPaintables() {
	return &Entity::paintables;
}

std::vector<Entity*>* Entity::getEntities() {
	return &Entity::entities;
}

void Entity::paint(sf::RenderWindow* window) {
	for(std::vector<sf::Drawable*>::iterator it = Entity::paintables.begin();
			it!=Entity::paintables.end();++it)
		window->draw(*(*it));
}

namespace EntityUtility {
	bool notActive(Entity* e) {
		bool flag = !e->isActive();
		if(flag) {
			Entity::remove(e, false);
			delete e;
		}
		return flag;
	}
}

void Entity::clear() {
	for(auto it = Entity::entities.begin();it!=Entity::entities.end();++it)
		(*it)->destroy();

	Entity::entities.erase(std::remove_if(
			Entity::entities.begin(), Entity::entities.end(),
			EntityUtility::notActive), Entity::entities.end());

	while(!Entity::paintables.empty()) {
		delete Entity::paintables.back();
		Entity::paintables.pop_back();
	}
}

void Entity::onUpdate(const sf::Time& dt) {
	for(std::vector<Entity*>::iterator it = Entity::entities.begin();
			it!=Entity::entities.end();++it)
		if((*it)->active)
			(*it)->update(dt);

	Entity::entities.erase(std::remove_if(
		Entity::entities.begin(), Entity::entities.end(),
		EntityUtility::notActive), Entity::entities.end());
}

bool Entity::isActive() {return active;}
Vector2D* Entity::getSize() {return size;}
Vector2D* Entity::getSpeed() {return speed;}
sf::Color* Entity::getColor() {return color;}
bool Entity::getTeam() {return team;}

void Entity::setActive(bool active) {this->active = active;}
void Entity::setSize(double x, double y) {size->set(x, y);}
void Entity::setSpeed(double x, double y) {speed->set(x, y);}
void Entity::setTeam(bool team) {this->team = team;}
void Entity::setColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b) {
	delete color;
	color = new sf::Color(r, g, b);
}

void Entity::update(const sf::Time& dt) {
	if(this->handleDeath())
		return;

	position->add(speed);
	this->setPosition(position->x, position->y);
	this->setRotation(-180*angle/math::PI);
}

bool Entity::intersects(Entity* e) {
	/* Old rectangle boundaries checking. *BURP*. Excuse me. */
	/*return (position->x < e->position->x + e->size->x &&
			position->x + size->x > e->position->x &&
			position->y < e->position->y + e->size->y &&
			position->y + size->y > e->position->y);*/

	double dx = e->position->x - position->x;
	double dy = e->position->y - position->y;


	double rA = size->x/2;
	double rB = e->size->x/2;

	double dist = rA*rA + rB*rB;

	return (dx*dx + dy*dy) <= dist;
}

void Entity::lookAt(double x, double y) {
	this->angle = atan2(position->x-x, position->y-y);
}

void Entity::moveTo(double x, double y) {
	double theta = atan2(x-position->x, y-position->y);
	this->speed->set(sin(theta)*5, cos(theta)*5);
}

#include "Projectile.hpp"

void Entity::shoot(void) {
	double alpha, theta = math::PI-(this->getRotation()*math::PI/180);
	double r = this->size->x/2;
	unsigned short int k = 3 + this->level;

	for(short int i=0;i<k;i++) {
		alpha = theta + i*(2*math::PI/k);
		Entity::add(new Projectile(this,
				position->x + r*sin(alpha),
				position->y + r*cos(alpha),
				sin(alpha)*7.5, cos(alpha)*7.5));
	}
}

#endif
