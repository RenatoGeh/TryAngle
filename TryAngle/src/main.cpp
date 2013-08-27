#include <iostream>
#include "Vector2D.hpp"

namespace Settings {
	unsigned short int Width;
	unsigned short int Height;

	Vector2D* mouse_position = new Vector2D();
}

//#include "GameFrame.hpp"
#include "Component.hpp"

int main() {
	GameFrame app("F*CK THEM BALLS!!1", 800, 600);

	return app.onExecute();
}
