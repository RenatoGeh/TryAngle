/*
 * Skill.hpp
 *
 *  Created on: Nov 3, 2013
 *      Author: Renato Lui Geh
 */

#ifndef SKILL_HPP_
#define SKILL_HPP_

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Utility.hpp"
#include "Timer.hpp"

class Skill {
	public:
		enum class Type : math::u_byte {Pass, Inst, Time};
	public:
		std::string name;
		Skill::Type id;
		bool isAvailable;
		ActionTimer<void(void)> cooldown;
	public:
		Skill(const std::string&, Skill::Type, float);
		virtual ~Skill(void);
	public:
		void setCooldown(float);
		float getCooldown(void) const;
	public:
		const std::string& getName(void) const;
		const Skill::Type& getType(void) const;
	public:
		virtual void onTrigger(void) = 0;
	public:
		virtual void triggerCooldown(void);
		virtual bool available(void) const;

	/* Declares "static" inner classes */
	public:
		class Passive;
		class Instant;
		class Timed;
};

/* Passive Skills */
class Skill::Passive : public Skill {
	public:
		Passive(const std::string&);
		virtual ~Passive(void);
	public:
		virtual void onTrigger(void) = 0;
};
/* Instant Skills */
class Skill::Instant : public Skill {
	public:
		Instant(const std::string&);
		virtual ~Instant(void);
	public:
		virtual void onTrigger(void) = 0;
};
/* Timed Skills */
class Skill::Timed : public Skill {
	protected:
		ActionTimer<void(void)> delay;
	public:
		Timed(const std::string&, float, std::function<void(void)>);
		virtual ~Timed(void);
	public:
		virtual void onTrigger(void);
};

Skill::Skill(const std::string& name_, Type id_, float tempo=3) :
		name(name_), id(id_), isAvailable(true),
		cooldown(sf::seconds, tempo, false, float(0), [&](){
	std::cout << "Cooled!" << std::endl;
	isAvailable = true;
	cooldown.reset();
}, false, false) {}
Skill::~Skill(void) {}

void Skill::setCooldown(float tempo) {cooldown.setTempo(sf::seconds, tempo);}
float Skill::getCooldown(void) const {return cooldown.getTempo();}

const std::string& Skill::getName(void) const {return name;}
const Skill::Type& Skill::getType(void) const {return id;}

void Skill::triggerCooldown(void) {
	isAvailable = false;
	cooldown.setActive(true);
	Timer::add(&cooldown);
}

bool Skill::available(void) const {return isAvailable;}

Skill::Passive::Passive(const std::string& name_) :
		Skill(name_, Type::Pass) {}
Skill::Passive::~Passive(void) {}

Skill::Instant::Instant(const std::string& name_) :
		Skill(name_, Type::Inst) {}
Skill::Instant::~Instant(void) {}

Skill::Timed::Timed(const std::string& name_, float dt, std::function<void(void)> func) :
		Skill(name_, Type::Time), delay(sf::seconds, dt, false, 0.f, func, false, false) {}
Skill::Timed::~Timed(void) {}

void Skill::Timed::onTrigger(void) {Timer::add(&delay);}

#endif
