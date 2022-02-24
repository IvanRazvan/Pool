#pragma once

#include "Vector2.h"

/*
	Image class implements behaviour for a sprite
*/

class Image
{
protected:
	string m_texturePath;
	int m_width{ 0 }, m_height{ 0 };
	sf::Texture* m_texture{ nullptr };
	sf::RectangleShape* m_sprite{ nullptr };
public:
	Image() = default;
	Image(Vector2 position, int width, int height, string texturePath);
	~Image();

	void SetWidth(int width);
	void SetHeight(int height);

	void SetPosition(Vector2 position);
	void SetTexture(string texturePath);

	void Render(sf::RenderWindow& window);
};

