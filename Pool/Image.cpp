#include "Image.h"

Image::Image(Vector2 position, int width, int height, string texturePath)
{
	m_sprite = new sf::RectangleShape();
	SetPosition(position);
	SetWidth(width);
	SetHeight(height);
	SetTexture(texturePath);
}

Image::~Image()
{
	delete m_texture;
	delete m_sprite;
}

void Image::SetWidth(int width)
{
	m_width = width;
	m_sprite->setSize(sf::Vector2f(m_width, m_height));
}

void Image::SetHeight(int height)
{
	m_height = height;
	m_sprite->setSize(sf::Vector2f(m_width, m_height));
}

void Image::SetPosition(Vector2 position)
{
	m_sprite->setPosition(sf::Vector2f(position.x, position.y));
}

void Image::SetTexture(string texturePath)
{
	m_texturePath = texturePath;

	delete m_texture;

	m_texture = new sf::Texture();
	
	if (!m_texture->loadFromFile(texturePath))
	{
		std::cout << "Texture error! Texture not found: " << texturePath << '\n';
	}

	m_sprite->setTexture(m_texture);
}

void Image::Render(sf::RenderWindow& window)
{
	window.draw(*m_sprite);
}
