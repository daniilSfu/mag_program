#include "stdafx.h"
#include "visualise.h"
#include <SFML/Graphics.hpp>
sf::RenderWindow* visualise::window = new sf::RenderWindow(sf::VideoMode(700, 600), "window");

void visualise::prepare_to_draw()
{
	window->clear();
}
int visualise::right_screen_border()
{
	return min(int(camera_x+window->getSize().x/current_map->cell_size), current_map->map_size_x);
}
int visualise::down_screen_border()
{
	return min(int(camera_y+window->getSize().y/current_map->cell_size), current_map->map_size_y);
}
void visualise::draw()
{
	int bord_size = 3;
	for (auto &cur : current_map->one_turn_object_without_grid_linking)
	{
		if (cur->x>camera_x&&cur->y<right_screen_border() && cur->y > camera_y&&cur->y<down_screen_border())
		{
			sf::Sprite sprite;
			cur->draw((cur->x - camera_x)*current_map->cell_size + bord_size + 1,
				(cur->y - camera_y)*current_map->cell_size + bord_size + 1, window);
		}
	}
	for(int i= camera_x ; i<right_screen_border() ; i++)
		for(int j= camera_y ; j<down_screen_border() ; j++)
			for (int k = 0; k< current_map->grid[i][j].size(); k++)
			{
				current_map->grid[i][j][k]->draw((current_map->grid[i][j][k]->x - camera_x)*current_map->cell_size+ bord_size+1,
					(current_map->grid[i][j][k]->y - camera_y)*current_map->cell_size+bord_size+1, window);
			}
	sf::Color cl = sf::Color(200, 200, 215, 255);
	sf::RectangleShape line(sf::Vector2f(right_screen_border()*current_map->cell_size+6+ bord_size*2, bord_size));
	line.setPosition(sf::Vector2f(0, 0));
	line.setFillColor(cl);
	window->draw(line);

	sf::RectangleShape line2(sf::Vector2f(down_screen_border()*current_map->cell_size+6+ bord_size*2, bord_size));
	line2.setPosition(sf::Vector2f(bord_size, 0));
	line2.setFillColor(cl);
	line2.rotate(90);
	window->draw(line2);

	sf::RectangleShape line3(sf::Vector2f(right_screen_border()*current_map->cell_size + 6+ bord_size*2, bord_size));
	line3.setPosition(sf::Vector2f(0, down_screen_border()*current_map->cell_size + 6+ bord_size));
	line3.setFillColor(cl);
	window->draw(line3);

	sf::RectangleShape line4(sf::Vector2f(down_screen_border()*current_map->cell_size + 6+ bord_size*2, bord_size));
	line4.setPosition(sf::Vector2f(right_screen_border()*current_map->cell_size + 6+ bord_size*2, 0));
	line4.setFillColor(cl);
	line4.rotate(90);
	window->draw(line4);
	for (int i = 0;i < data.size();i++)
	{
		sf::Text text;
		text.setFont(f);
		text.setString(data[i]);
		text.setCharacterSize(18);
		text.setFillColor(cl);
		text.setPosition(sf::Vector2f(30, down_screen_border()*current_map->cell_size + 30*(i+1)));
		window->draw(text);
	}
}
void visualise::after_draw_and_hold_window()
{
	if (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}

		window->display();
	}
}
visualise::visualise(void)
{
	camera_x = 0;
	camera_y = 0;
	f.loadFromFile("bankgothic_russ_medium.ttf");
}


visualise::~visualise(void)
{
}
