/*
 * SkillSet.hpp
 *
 *  Created on: Nov 3, 2013
 *      Author: Renato Lui Geh
 */

#ifndef SKILLSET_HPP_
#define SKILLSET_HPP_

#include <stdexcept>
#include <vector>

#include "Skills.hpp"

class SkillNode {
	private:
		std::vector<SkillNode*> children;
		std::vector<SkillNode*> parents;

		Skill* core;
		bool active;

		bool visited;
	public:
		SkillNode(Skill*);
		~SkillNode(void);
	public:
		SkillNode& add(Skill*);
	public:
		bool isActive(void) const;
		bool activate(void);
	public:
		Skill* getCore(void) const;
	private:
		static bool check_req(SkillNode*, bool);
};

SkillNode::SkillNode(Skill* core_) : core(core_),
		active(false), visited(false) {}

SkillNode::~SkillNode(void) {
	while(!children.empty()) {
		SkillNode* trash = children.back();
		children.pop_back();
		delete trash;
	}
}

SkillNode& SkillNode::add(Skill* child) {
	SkillNode* node = new SkillNode(child);
	children.push_back(node);
	node->parents.push_back(this);
	return *node;
}

/**
 * Checks recursively requirements to the Skill.
 *
 * +===============================================+
 * |WARNING WARNING WARNING WARNING WARNING WARNING|
 * +===============================================+
 *
 * Highly unoptimized.
 * Why?
 * Because I'm lazy.
 *
 * P.S.: What's an infinite loop called?
 *       FOR-EVER
 *       for(;;) cout << "HA";
 */
bool SkillNode::check_req(SkillNode* node, bool prim = true) {
	for(auto it=node->parents.begin();it!=node->parents.end();++it)
		(*it)->visited = false;

	for(auto it=node->parents.begin();it!=node->parents.end();++it)
		if(!(*it)->visited && !check_req(*it, false))
			return false;

	if(!prim) return node->active;
	return true;
}

bool SkillNode::isActive(void) const {return active;}
bool SkillNode::activate(void) {
	if(check_req(this)) return active = true;
	return false;
}

Skill* SkillNode::getCore(void) const {return core;}

class SkillTree {
	private:
		SkillNode head;
	public:
		SkillTree(void);
		~SkillTree(void);
	public:
		inline SkillNode* add(Skill*);
};

SkillTree::SkillTree(void) : head(&Skills::Default) {}
SkillTree::~SkillTree(void) {}

inline SkillNode* SkillTree::add(Skill* core) {return &(head.add(core));}

class SkillSet {
	private:
		unsigned short int sides;
		SkillTree tree;
		SkillNode* hotkeys[15];
	public:
		SkillSet(unsigned short int);
		~SkillSet(void);
	public:
		inline void addSide(unsigned short int);
		inline void subSide(unsigned short int);
		inline void setSide(unsigned short int);
	public:
		inline SkillTree& getSkills(void);
		inline unsigned short int getSides(void) const;
	public:
		void onEvent(const sf::Event&);
		void link(SkillNode*, sf::Keyboard::Key);
};

SkillSet::SkillSet(unsigned short int _sides=0) : sides(_sides) {
	for(int i=0;i<15;++i)
		hotkeys[i] = nullptr;
}
SkillSet::~SkillSet(void) {}

void SkillSet::addSide(unsigned short int inc) {sides += inc;}
void SkillSet::subSide(unsigned short int dec) {sides -= dec;}
void SkillSet::setSide(unsigned short int side_) {sides = side_;}

inline SkillTree& SkillSet::getSkills(void) {return tree;}
inline unsigned short int SkillSet::getSides(void) const {return 3+sides;}

void SkillSet::onEvent(const sf::Event& event) {
	if(event.type == sf::Event::KeyPressed) {
		if(event.key.code >= sf::Keyboard::F1 &&
				event.key.code <= sf::Keyboard::F15) {
			int code = event.key.code - sf::Keyboard::F1;

			if(hotkeys[code] != nullptr)
				hotkeys[code]->getCore()->onTrigger();
		}
	}
}

void SkillSet::link(SkillNode* node, sf::Keyboard::Key key) {
	for(int i=0;i<15;++i)
		if(hotkeys[i] == node) {
			hotkeys[i] = nullptr;
			break;
		}

	hotkeys[key-sf::Keyboard::F1] = node;
}

#endif
