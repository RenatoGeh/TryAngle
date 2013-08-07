#include <iostream>
#include "Vector2D.hpp"

namespace Settings {
	unsigned short int Width;
	unsigned short int Height;

	Vector2D* mouse_position = new Vector2D();
}

#include "GameFrame.hpp"

int main() {
	GameFrame app("I M43D 4 G4ME W1T XAPEZ 1N IT!!11ONE", 800, 600);

	return app.onExecute();
}
