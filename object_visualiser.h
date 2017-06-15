#include<SFML\Graphics.hpp>
#pragma once
class object_visualiser
{
public:
	virtual void visualise(int x, int y,sf::RenderWindow* window);
	object_visualiser();
	~object_visualiser();
};

class object_visualiser_texture:public object_visualiser
{

public:
	sf::Texture texture;
	void visualise(int x, int y,sf::RenderWindow* window);
	object_visualiser_texture() {};
	~object_visualiser_texture() {};
};

