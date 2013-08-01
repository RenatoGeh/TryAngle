/*
 * Timer.hpp
 *
 *  Created on: Jul 31, 2013
 *      Author: Renato Lui Geh
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <vector>

class Timer {
	private:
		static std::vector<Timer*> timers;
	private:
		sf::Time time;
		sf::Time dt;

		bool repeats;
		bool active;

		void (*action)(void);
	public:
		template <typename T>
			Timer(sf::Time (*)(T), double, bool, double,
					void (*)(void), bool);
	public:
		void update(sf::Time);
		bool isActive(void);
		void setActive(bool);
	public:
		static void add(Timer*);
		static void remove(Timer*);
		static void clear(void);
		static void onUpdate(sf::Time);
};

std::vector<Timer*> Timer::timers;

template <typename T>
	Timer::Timer(sf::Time (*format)(T e), double dt,
			bool repeats=true, double time,	void (*f)(void), bool active) {

	this->dt = format(dt);
	this->repeats = repeats;
	this->time = format(time);
	this->active = active;
	this->action = f;

	if(active)
		Timer::add(this);
}

void Timer::update(sf::Time dt) {
	if(!this->active)
		return;

	this->time += dt;

	if(this->time < this->dt)
		return;

	if(!this->repeats)
		this->active = false;

	this->action();
}

bool Timer::isActive() {return this->active;}
void Timer::setActive(bool active) {this->active = active;}

void Timer::add(Timer* timer) {Timer::timers.push_back(timer);}
void Timer::remove(Timer* timer) {
	for(auto it = Timer::timers.begin();it!=Timer::timers.end();++it)
		if(*it == timer) {
			Timer::timers.erase(it);
			break;
		}
}

namespace TimerUtility {
	bool notActive(Timer* e) {
		bool flag = !e->isActive();
		if(flag) delete e;
		return flag;
	}
}

void Timer::onUpdate(sf::Time dt) {
	for(auto it = Timer::timers.begin();it!=Timer::timers.end();++it)
		(*it)->update(dt);

	Timer::timers.erase(std::remove_if(
			Timer::timers.begin(), Timer::timers.end(),
			TimerUtility::notActive), Timer::timers.end());
}

#endif
