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
	typedef unsigned char u_byte;

	const long double PI = 3.14159265358979323846;
	template <typename T> short int signum(T e) {return e>0?1:e<0?-1:0;}
}

namespace Utility {
	namespace Random {
		std::default_random_engine gen;

		sf::Color getRandomColor() {
			return sf::Color(gen()%256, gen()%256, gen()%256);
		}

		unsigned long int getUnsignedRandom(unsigned long int min=0,
				unsigned long int max = gen.max()) {
			return min + (gen() % max);
		}

		short int getRandomSign(bool zero=true) {
			if(zero) return (-1 + getUnsignedRandom(0, 3));
			else return (getUnsignedRandom(0, 11)%2==0)?-1:1;
		}

		long int getRandom(long int min=0, long int max = gen.max()/2-1) {
			return min + getRandomSign(false)*getUnsignedRandom(min, max);
		}
	}

	namespace Color {
		class Pattern {
			private:
				sf::Color* palette;

				short int sign_r;
				short int sign_g;
				short int sign_b;
			public:
				Pattern(sf::Uint8, sf::Uint8, sf::Uint8);
				~Pattern(void);
			private:
				static void modulo(sf::Uint8, short int&);
			public:
				sf::Color& nextColor(void);
		};

		Pattern::Pattern(sf::Uint8 r = Random::getUnsignedRandom(0, 256),
				sf::Uint8 g = Random::getUnsignedRandom(0, 256),
				sf::Uint8 b = Random::getUnsignedRandom(0, 256)) {

			this->palette = new sf::Color(r, g, b);

			this->sign_r = Random::getRandom(1, 3);
			this->sign_g = Random::getRandom(1, 3);
			this->sign_b = Random::getRandom(1, 3);
		}

		Pattern::~Pattern(void) {
			delete palette;
		}

		void Pattern::modulo(sf::Uint8 comp, short int& sign) {
			if(comp + sign > 255 || comp + sign < 0) sign = -sign;
		}

		sf::Color& Pattern::nextColor(void) {
			Pattern::modulo(palette->r, sign_r);
			Pattern::modulo(palette->g, sign_g);
			Pattern::modulo(palette->b, sign_b);

			palette->r += sign_r;
			palette->g += sign_g;
			palette->b += sign_b;

			return *palette;
		}
	}
}

#endif
