#pragma once
class base_object;
#include <SFML/Graphics.hpp>
#include "base_object.h"
#include "map.h"
class visualise
{
int right_screen_border();
int down_screen_border();
public:
    static sf::RenderWindow* window;
	int camera_x;
	int camera_y;
	sf::Font f;
	world_map* current_map;
	vector<string> data;
	void prepare_to_draw();
	void draw();
	void after_draw_and_hold_window();

	visualise(void);
	~visualise(void);
};

