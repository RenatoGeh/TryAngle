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
}

#include "GameFrame.hpp"

int main() {
	GameFrame app("F*CK THEM BALLS!!1", 800, 600);

	return app.onExecute();
}
