/*
 * DeathMenu.hpp
 *
 *  Created on: Aug 31, 2013
 *      Author: Renato Lui Geh
 */

#ifndef DEATHMENU_HPP_
#define DEATHMENU_HPP_

class DeathMenu : public Menu {
	private:
		static DeathMenu* instance;
	private:
		sf::Text respawn;
		sf::Text message;
	private:
		DeathMenu(void);
		~DeathMenu(void);
	public:
		void onEvent(const sf::Event&);
		void update(const sf::Time&);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
};

DeathMenu::DeathMenu(void) : Menu("THE END") {
	respawn.setCharacterSize(20);
	respawn.setFont(Settings::DEF_FONT);

	message.setCharacterSize(20);
}

#endif
