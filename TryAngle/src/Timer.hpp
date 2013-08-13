/*
 * Timer.hpp
 *
 *  Created on: Jul 31, 2013
 *      Author: Renato Lui Geh
 */

#ifndef Timer_HPP_
#define Timer_HPP_

#include <vector>
#include <functional>

class Timer {
	private:
		static std::vector<Timer*> timers;
	protected:
		sf::Time time;
		sf::Time dt;

		bool repeats;
		bool active;
		bool auto_deletion;
	public:
		template <typename T>
			Timer(sf::Time(*)(T), T, bool, T, bool, bool);
		virtual ~Timer(void);
	public:
		virtual void update(const sf::Time&);
		bool isActive(void) const;
		void setActive(bool);
		bool autoDeletes(void) const;
		void setAutoDeletion(bool);
	public:
		static void add(Timer*);
		static void remove(Timer*);
		static void clear(void);
		static void onUpdate(const sf::Time&);
		static void refresh(void);
};

std::vector<Timer*> Timer::timers;

template <typename T> Timer::Timer(sf::Time (*format)(T e), T dt,
		bool repeats = false, T time = 0, bool active = false,
		bool auto_deletion = true) {

	this->dt = format(dt);
	this->repeats = repeats;
	this->time = format(time);
	this->active = active;
	this->auto_deletion = auto_deletion;

	if(active)
		Timer::add(this);
}

Timer::~Timer() {}

void Timer::update(const sf::Time& dt) {
	if(!this->active)
		return;

	this->time += dt;

	if(this->time < this->dt)
		return;

	if(!this->repeats)
		this->active = false;
}

bool Timer::isActive() const {return this->active;}
void Timer::setActive(bool active) {this->active = active;}

bool Timer::autoDeletes(void) const {return this->auto_deletion;}
void Timer::setAutoDeletion(bool deletes) {auto_deletion = deletes;}

void Timer::add(Timer* timer) {Timer::timers.push_back(timer);}
void Timer::remove(Timer* timer) {
	for(auto it = Timer::timers.begin();it!=Timer::timers.end();++it)
		if(*it == timer) {
			Timer::timers.erase(it);
			break;
		}
}

void Timer::clear() {
	while(!Timer::timers.empty()) {
		delete Timer::timers.back();
		Timer::timers.pop_back();
	}
}

namespace TimerUtility {
	bool notActive(Timer* e) {
		bool flag = !e->isActive();
		if(flag && e->autoDeletes()) delete e;
		return flag;
	}
}

void Timer::onUpdate(const sf::Time& dt) {
	for(auto it = Timer::timers.begin();it!=Timer::timers.end();++it)
		(*it)->update(dt);

	Timer::timers.erase(std::remove_if(
			Timer::timers.begin(), Timer::timers.end(),
			TimerUtility::notActive), Timer::timers.end());
}

void Timer::refresh(void) {
	Timer::timers.erase(std::remove_if(
			Timer::timers.begin(), Timer::timers.end(),
			TimerUtility::notActive), Timer::timers.end());
}

/**********************ACTION_TIMER****************************/

template <typename Fn> class ActionTimer : public Timer {
	private:
		std::function<Fn> action;
	public:
		template <typename T>
			ActionTimer(sf::Time (*format)(T e), T dt, bool repeats=true,
					T time = 0, std::function<Fn> f = NULL, bool active=false,
					bool auto_deletion = true) :
					Timer(format, dt, repeats, time, active, auto_deletion) {

				this->action = f;

				if(active)
					Timer::add(this);
		}
		~ActionTimer(void);
	public:
		void update(const sf::Time&);
};

template<typename Fn> ActionTimer<Fn>::~ActionTimer() {}

template<typename Fn> void ActionTimer<Fn>::update(const sf::Time& dt) {
	if(!this->active)
		return;

	this->time += dt;

	if(this->time < this->dt)
		return;

	if(!this->repeats) {
		this->active = false;
		return;
	} else
		this->time = sf::Time::Zero;

	if(this->action != NULL)
		this->action();
}

#endif
