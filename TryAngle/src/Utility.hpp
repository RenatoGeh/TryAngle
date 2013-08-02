/*
 * Utility.hpp
 *
 *  Created on: Jul 31, 2013
 *      Author: Renato Lui Geh
 */

#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <random>

namespace math {
	typedef char byte;

	const long double PI = 3.14159265358979323846;
	template <typename T> short int signum(T e) {return e>0?1:e<0?-1:0;}
}

namespace Utility {
	namespace Random {
		std::default_random_engine gen;

		sf::Color getRandomColor(void);
		unsigned long int getRandom(unsigned long int,
				unsigned long int);

		sf::Color getRandomColor() {
			return sf::Color(gen()%256, gen()%256, gen()%256);
		}

		unsigned long int getRandom(unsigned long int min=0,
				unsigned long int max = gen.max()) {
			return min + (gen() % max);
		}

		short int getRandomSign(bool zero=true) {
			if(zero) return (-1 + getRandom(0, 3));
			else return (getRandom(0, 11)%2==0)?-1:1;
		}
	}

	namespace Color {
		sf::Color palette = sf::Color();

		sf::Color nextColor(void) {
			palette.r = (palette.r + Random::getRandomSign())%256;
			palette.g = (palette.g + Random::getRandomSign())%256;
			palette.b = (palette.b + Random::getRandomSign())%256;

			return palette;
		}
	}
}

#endif
