/*
 * Utility.hpp
 *
 *  Created on: Jul 31, 2013
 *      Author: Renato Lui Geh
 */

#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <random>
#include <iostream>
#include <ctime>
#include "Gambs.hpp"

namespace math {
	typedef char byte;
	typedef unsigned char u_byte;
	typedef unsigned long int u_id;

	const long double PI = 3.14159265358979323846;
	template <typename T> short int signum(T e) {return e>0?1:e<0?-1:0;}

	template <typename T> inline T max(T a, T b) {return b<a?a:b;}
	template <typename T> inline T min(T a, T b) {return b>a?a:b;}
	template <typename T> inline T abs(T n) {return n<-1?-n:n;}

	template <long int N, long int I> struct Power {
		enum {value = N * Power<N, I-1>::value};
	};

	template <long int N> struct Power<N, 0> {
		enum {value	= 1};
	};

	template <typename T> inline T pow(T base, T exp) {
		if(exp==0) return 1;
		T p;
		for(p=base;exp>1;p*=base,--exp);
		return p;
	}

	template <typename T> inline void printBinary(T n) {
		T p;
		for(p=1;p<n;p*=2);
		for(;p>0;p/=2)
			std::cout << (n/p)%2;
	}
}

namespace Utility {
	namespace Random {
		std::default_random_engine gen(time(0));

		sf::Color getRandomColor() {
			return sf::Color(gen()%256, gen()%256, gen()%256);
		}

		sf::Color getInverseColor(const sf::Color& orig) {
			return sf::Color(
					(255-orig.r)%256,
					(255-orig.b)%256,
					(255-orig.g)%256);
		}

		unsigned long int getUnsignedRandom(unsigned long int min=0,
				unsigned long int max=gen.max()) {
			return min + (gen() % (max-min));
		}

		short int getRandomSign(bool zero=true) {
			if(zero) return (-1 + getUnsignedRandom(0, 3));
			else return (getUnsignedRandom(0, 11)%2==0)?-1:1;
		}

		long int getRandom(long int min=0, long int max=gen.max()/2-1) {
			return getUnsignedRandom(0, math::abs(max-min))+min;
		}

		long int getBoundRandom(long int x1, long int x2, long int x3, long int x4) {
			return getRandomSign(false)<0?getRandom(x1, x2):getRandom(x3, x4);
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
				inline static void modulo(sf::Uint8, short int&);
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

		inline void Pattern::modulo(sf::Uint8 comp, short int& sign) {
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

	namespace Spawn {
		//This is a forward declaration of namespace Spawn.
		//It is implemented accordingly throughout their respective
		//representatives (i.e. YOUR MOM).
	}
}

#endif
