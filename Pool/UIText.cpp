#include "UIText.h"

UIText::UIText(int size, string font, Vector2 position, sf::Color colour)
{
	m_text = new sf::Text();

	SetPosition(position);
	m_text->setCharacterSize(size);
	SetColour(colour);
	m_text->setOutlineThickness(1);
	m_text->setOutlineColor(sf::Color::Black);

	m_font = new sf::Font();
	if (!m_font->loadFromFile(font))
		std::cout << "Font not found " + font;

	m_text->setFont(*m_font);
}

UIText::~UIText()
{
	delete m_text;
	delete m_font;
}

void UIText::SetPosition(Vector2 position)
{
	m_text->setPosition(sf::Vector2f(position.x, position.y));
}

void UIText::SetColour(sf::Color colour)
{
	m_text->setFillColor(colour);
}

void UIText::SetString(string text)
{
	m_text->setString(text);
}

void UIText::Render(sf::RenderWindow& window)
{
	window.draw(*m_text);
}
