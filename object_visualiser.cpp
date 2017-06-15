#include "stdafx.h"
#include "object_visualiser.h"


void object_visualiser::visualise(int x, int y, sf::RenderWindow* window)
{
}

object_visualiser::object_visualiser()
{
}


object_visualiser::~object_visualiser()
{
}

void object_visualiser_texture::visualise(int x, int y,sf::RenderWindow* window)
{
	sf::Sprite sprite;
	sprite.setPosition(x, y);
	sprite.setTexture(texture);
	sprite.setScale(0.2f, 0.2f);
	window->draw(sprite);
}
