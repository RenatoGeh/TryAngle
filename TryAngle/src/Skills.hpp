/*
 * Skills.hpp
 *
 *  Created on: Nov 4, 2013
 *      Author: Renato Lui Geh
 */

#ifndef SKILLS_HPP_
#define SKILLS_HPP_

#include "Skill.hpp"

/*
 * Note to self:
 *  - Do some random joke with MissileToe.
 */

namespace Skills {
	class : public Skill::Passive {
		public:
			void onTrigger(void) {}
	} Default("Default");

	class : public Skill::Instant {
		public:
			void onTrigger(void) {

			}
	};
}

#endif
