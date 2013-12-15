/*
 * Skills.hpp
 *
 *  Created on: Nov 4, 2013
 *      Author: Renato Lui Geh
 */

#ifndef SKILLS_HPP_
#define SKILLS_HPP_

#include <iostream>

#include "Skill.hpp"

/*
 * Note to self:
 *  - Do some random joke with MissileToe.
 */

namespace Skills {
	class _Default : public Skill::Passive {
		public:
			_Default(void) : Skill::Passive("Default") {}
		public:
			void onTrigger(void) {}
	} Default;

	class _Instant : public Skill::Instant {
		public:
			_Instant(void) : Skill::Instant("Instant") {}
		public:
			void onTrigger(void) {
				std::cout << "Sexy Powers ACTIVATE!" << std::endl;
			}
	} Instant;

	class _Timed : public Skill::Timed {
		public:
			_Timed(void) : Skill::Timed("Timed", .5f, [](){}) {}
	} Timed;
}

#endif
