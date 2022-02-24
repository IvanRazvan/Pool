#pragma once

#include "Vector2.h"

/*
	UIText implements functionality to display a text on the screen
*/

class UIText
{
private:
	sf::Font* m_font;
	sf::Text* m_text;
public:
	UIText() = default;
	UIText(int size, string font, Vector2 position = Vector2(), sf::Color colour = sf::Color::White);
	~UIText();

	void SetPosition(Vector2 position);
	void SetColour(sf::Color colour);
	void SetString(string text);

	void Render(sf::RenderWindow& window);
};

