/*
 * Stats.hpp
 *
 *  Created on: Sep 25, 2013
 *      Author: Renato Lui Geh
 */

#ifndef STATS_HPP_
#define STATS_HPP_

#include <string>
#include <sstream>
#include <fstream>

#include "Player.hpp"
#include "Gambs.hpp"

class Stats {
	private:
		std::string path;
		Player* val_ptr;
	public:
		Stats(Player*, std::string);
		Stats(const Stats&);
		~Stats(void);
	public:
		const std::string& getPath(void) const;
		Player* getPlayer(void) const;
	public:
		Stats& operator = (const Stats&);
		friend std::ostream& operator << (std::ostream&, const Stats&);
		friend std::istream& operator >> (std::istream&, const Stats&);
	private:
		template <typename T>
			inline static T getType(std::istream&, std::string&);
		inline static void pullColor(std::istream&, std::string&,
				int&, int&, int&);
		inline static void streamColor(std::ostream&, const sf::Color&);
	public:
		static bool save(const Stats&);
		static Stats load(const std::string& path, Player*);
	public:
		inline static bool exists(const std::string&);
		static void setDefault(const Stats&);
		static Stats getDefault(Player*);
	public:
		static const std::string Extension;
		static const std::string Default;
		static bool def_exists;
};

const std::string Stats::Extension = ".stats";
const std::string Stats::Default = "def_player.bak";
bool Stats::def_exists = Stats::exists(Stats::Default);

namespace {
	std::string trim(const std::string& str) {
		std::string res;

		for(auto it = str.begin();it!=str.end();++it)
			if(*it != ' ')
				res += *it;

		return res;
	}
}

Stats::Stats(Player* object, std::string r_path = "") :
		path(r_path), val_ptr(object) {
	if(object == nullptr) return;
	if(path.empty()) path = object->getName() + Stats::Extension;
	path = trim(path);
}

Stats::Stats(const Stats& copy) : path(copy.path), val_ptr(copy.val_ptr) {}

Stats::~Stats(void) {}

Stats& Stats::operator = (const Stats& copy) {
	path = copy.path;
	val_ptr = copy.val_ptr;
	return *this;
}

const std::string& Stats::getPath(void) const {return path;}
Player* Stats::getPlayer(void) const {return val_ptr;}

inline void Stats::streamColor(std::ostream& stream, const sf::Color& col) {
	stream << int(col.r) << ' ' <<
		int(col.g) << ' ' <<
		int(col.b) << " \n";
}

std::ostream& operator << (std::ostream& stream, const Stats& stats) {
	Player* object = stats.getPlayer();

	stream << object->getName() << '\n';
	stream << object->getMaxHealth() << '\n';
	stream << object->getMaxExp() << '\n';
	stream << object->getLevel() << '\n';
	stream << object->getShield().getMaxShield() << '\n';
	stream << object->getShield().getMitigation() << '\n';

	Stats::streamColor(stream, object->getShield().getColor());
	Stats::streamColor(stream, object->getShield().getOutlineColor());

	Stats::streamColor(stream, *(object->getColor()));
	Stats::streamColor(stream, *(object->getOutlineColor()));

	return stream;
}

template <typename T>
	inline T Stats::getType(std::istream& stream, std::string& line) {
	std::getline(stream, line);
	std::stringstream converter(line);
	T val;
	converter >> val;
	return val;
}

inline void Stats::pullColor(std::istream& stream, std::string& line,
		int& r, int& g, int& b) {
	std::getline(stream, line);
	std::stringstream converter(line);
	converter >> r >> g >> b;
}

std::istream& operator >> (std::istream& stream, const Stats& stats) {
	std::string line;
	std::stringstream converter;

	Player* object = stats.getPlayer();

	std::getline(stream, line);
	converter.str(line);
	object->setName(line);

	object->setMaxHealth(Stats::getType<double>(stream, line));
	object->setMaxExp(Stats::getType<double>(stream, line));
	object->setLevel(Stats::getType<unsigned short int>(
			stream, line));

	object->getShield().setMaxShield(Stats::getType<double>(
			stream, line));
	object->getShield().setMitigation(Stats::getType<double>(
			stream, line));

	int r, g, b;

	Stats::pullColor(stream, line, r, g, b);
	object->getShield().setColor(r, g, b);

	Stats::pullColor(stream, line, r, g, b);
	object->getShield().setOutlineColor(r, g, b);

	Stats::pullColor(stream, line, r, g, b);
	object->setColor(r, g, b);

	Stats::pullColor(stream, line, r, g, b);
	object->setOutlineColor(r, g, b);

	return stream;
}

bool Stats::save(const Stats& stats) {
	std::ofstream output(stats.path,
			std::ios_base::trunc | std::ofstream::out);

	if(!output.is_open()) {
		std::cerr << "D'Oh! File Input: something went wrong. "
				"Besides you." << std::endl;
		return false;
	}

	output << stats;

	output.close();

	return true;
}

Stats Stats::load(const std::string& path, Player* object) {
	std::ifstream input(path, std::ifstream::in);

	if(!input.is_open()) {
		std::cerr << "AH! LOADING HAS FAILED MISERABLY! Like you."
				<< std::endl;
		return nullptr;
	}

	Stats stats(object, path);
	input >> stats;

	input.close();

	return stats;
}

inline bool Stats::exists(const std::string& path) {
	return std::ifstream(path);
}

void Stats::setDefault(const Stats& stats) {
	std::ofstream output(Stats::Default,
			std::ios_base::trunc | std::ofstream::out);

	if(!output.is_open()) {
		std::cerr << "Problem with your output default, yo. "
				"That's a bad thing. Worse than your dad." << std::endl;
		return;
	}

	output << stats.path;

	output.close();
	Stats::def_exists = true;
}

Stats Stats::getDefault(Player* target) {
	if(!Stats::def_exists) return Stats(nullptr);

	std::ifstream input(Stats::Default, std::ifstream::in);
	std::string line;

	if(!input.is_open()) {
		std::cerr << "Ah! Default's input problem! No good." << std::endl;
		return nullptr;
	}

	std::getline(input, line);

	return Stats::load(line, target);
}

#endif
