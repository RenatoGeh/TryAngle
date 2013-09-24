#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Vector2D.hpp"

#include "Gambs.hpp"

namespace Settings {
	unsigned short int Width;
	unsigned short int Height;

	Vector2D mouse_position = new Vector2D();
	sf::Font DEF_FONT;

	static_block(defFont) {
		DEF_FONT.loadFromFile("src/resources/fonts/abduction2002.ttf");
	}

	bool in_game = false;

	/* Have to be overriden. THAT IS SO UGLY CODING. */
	void terminate(void);
	void clean(void);
	void pause(void);
	bool isPaused(void);
	void restart(void);
}

#include "GameFrame.hpp"

int main() {
	GameFrame* app = GameFrame::getInstance("I HAVE THE POWER");

	int state = app->onExecute();

	delete app;

	return state;
}
