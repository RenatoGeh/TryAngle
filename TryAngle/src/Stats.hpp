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

class Stats {
	private:
		std::string path;
		Player* val_ptr;
	public:
		Stats(const std::string&, Player*);
		~Stats(void);
	public:
		const std::string& getPath(void) const;
		Player* getPlayer(void) const;
	public:
		friend std::ostream& operator << (std::ostream&, const Stats&);
		friend std::istream& operator >> (std::istream&, const Stats&);
	private:
		template <typename T>
			inline static T getType(std::istream&, std::stringstream&,
					std::string&);
		inline static void pullColor(std::istream&, std::stringstream&,
				std::string&, int&, int&, int&);
		inline static void streamColor(
				std::ostream&, const sf::Color&);
	public:
		static bool save(const Stats&);
		static Stats* load(const std::string& path, Player*);
};

Stats::Stats(const std::string& r_path, Player* object) :
		path(r_path), val_ptr(object) {}

Stats::~Stats(void) {}

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
	inline T Stats::getType(std::istream& stream,
			std::stringstream& converter, std::string& line) {
	std::getline(stream, line);
	converter.str(line);
	T val;
	converter >> val;
	return val;
}

inline void Stats::pullColor(std::istream& stream,
		std::stringstream& converter, std::string& line,
		int& r, int& g, int& b) {
	std::getline(stream, line);
	converter.str(line);
	converter >> r >> g >> b;
}

std::istream& operator >> (std::istream& stream, const Stats& stats) {
	std::string line;
	std::stringstream converter;

	Player* object = stats.getPlayer();

	std::getline(stream, line);
	converter.str(line);
	object->setName(line);

	object->setMaxHealth(Stats::getType<double>(stream, converter, line));
	object->setMaxExp(Stats::getType<double>(stream, converter, line));
	object->setLevel(Stats::getType<unsigned short int>(
			stream, converter, line));

	object->getShield().setMaxShield(Stats::getType<double>(
			stream, converter, line));
	object->getShield().setMitigation(Stats::getType<double>(
			stream, converter, line));

	int r, g, b;

	Stats::pullColor(stream, converter, line, r, g, b);
	object->getShield().setColor(r, g, b);

	Stats::pullColor(stream, converter, line, r, g, b);
	object->getShield().setOutlineColor(r, g, b);

	Stats::pullColor(stream, converter, line, r, g, b);
	object->setColor(r, g, b);

	Stats::pullColor(stream, converter, line, r, g, b);
	object->setOutlineColor(r, g, b);

	return stream;
}

bool Stats::save(const Stats& stats) {
	std::ofstream output(stats.path);

	if(!output.is_open()) {
		std::cerr << "D'Oh! File Input: something went wrong. "
				"Besides you." << std::endl;
		return false;
	}

	output << stats;

	output.close();

	return true;
}

Stats* Stats::load(const std::string& path, Player* object) {
	std::ifstream input(path);

	if(!input.is_open()) {
		std::cerr << "AH! LOADING HAS FAILED MISERABLY! Like you."
				<< std::endl;
		return nullptr;
	}

	Stats* stats = new Stats(path, object);
	input >> stats;

	input.close();

	return stats;
}

#endif
