#pragma once
#include "play_turn.h"
#include "visualise.h"
#include <SFML/Graphics.hpp>
class main_constants_class
{
public:
	static play_turn current_play_turn_player1;
	static play_turn current_play_turn_player2;

	static visualise current_visualise_player1;
	static visualise current_visualise_player2;

	static int move_turn_count;

	main_constants_class(void);
	~main_constants_class(void);
};
