#pragma once
#include<vector>
#include "base_object.h"
#include "map.h"
#include "visualise.h"
#include "builder.h"
#include "scriptarray.h"
#include "scriptstdstring.h"
#include "scriptbuilder.h"
#include <list>
#include <iostream>
#include <windows.h>
#include "all_const.h"
#include <SFML/Window/Keyboard.hpp>

using namespace std;

typedef void(*tppoint)(int, int, int, int, int, int, ...);


class command
{
public:
	int delay;
	vector<int> command_parameters;
	command(int delay_,vector<int>command_parameters_):delay(delay_),command_parameters(command_parameters_)
	{
	}
	void push_back(int n)
	{
		command_parameters.push_back(n);
	}
	int size()
	{
		return command_parameters.size();
	}
	int operator[](int n)
	{
		return command_parameters[n];
	}
};
class play_turn:public RefC
{
public:
	world_map* current_map;
	int current_turn;
	visualise* current_visualise;
	list<base_object>* current_player_object;
	list<base_object>* opponent_player_object;
	vector<command>move_turn_player;
	vector<command>turn_player;
	void end_turn();
	int current_step;
	void push_back_output(string s, int n)
	{
		char*c = new char[100];
		_itoa_s(n, c,100, 10);
		current_visualise->data.push_back(s + string(c));
	}
	void push_back_output_(string s, string s1)
	{
		current_visualise->data.push_back(s + s1);
	}
	void push_back_output__(string s)
	{
		current_visualise->data.push_back(s);
	}
	void output_clean()
	{
		current_visualise->data.erase(current_visualise->data.begin(),current_visualise->data.end());
	}
	CScriptArray* play_turn::get_player_object();

	int get_turn_number()
	{
		return current_turn;
	}
	void change_map_size(int x,int y)
	{
		current_map->map_size_x = x;
		current_map->map_size_y = y;
	}
	void write_move_to_function_calling_list(base_object_ref& object,int x,int y);
	void write_move_to_function_calling_list_(base_object_ref& object, int x, int y);
	void write_camera_move_to_function_calling_list(visualise* visualise_, int x, int y);
	void write_shell_move_to_function_calling_list(base_object* object,int x,int y);
	void write_fire_to_function_calling_list(base_object_ref& object,int x,int y);

	void write_parameters_to_function_calling_list(vector<command>&command_vector,int delay,int a,...); //Так объявляется функция с заранее неизвестным числом параметров
	void take_action(command&vec,int step);

	void change_texture(string s,base_object_ref&base)
	{
		if (base.object->vis == 0)
		{
			for (int i = 0;i < base.object->modules.size();i++)
				if (base.object->modules[i] != 0)
					if (base.object->modules[i]->vis != 0)
					{
						object_visualiser_texture*o = (object_visualiser_texture*)(base.object->modules[i]->vis);
						o->texture.loadFromFile(s);
					}
		}
		else
		{
			object_visualiser_texture*o = (object_visualiser_texture*)(base.object->vis);
			o->texture.loadFromFile(s);
		}
//		delete base.object->vis;
//		base.object->vis = o;
	}


	static void camera_move(int this_, int visualise_, int x, int y,int step);
	static void move(int this_,int object,int x2,int y2,int step);
	static void move_(int this_, int object, int x2, int y2, int step);
	static void shell_move(int this_, int object, int x2, int y2, int step);
	static void fire__(int this_,int object,int x2,int y2,int step);

	void set_visible(int x, int y, bool vis)
	{
		for (int i = 0;i < current_map->grid[x][y].size();i++)
			current_map->grid[x][y][i]->visible = vis;
	}
	int rand_(int n)
	{
		if (n <= 0)
			return 0;
		return (rand() % n)+1;
	}

	void receive_message(int&message)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			cin >> message;
	}

	int one_turn()
	{
		int message=0;
		for (int i = 0;i < MOVE_STEP_COUNT;i++)
		{
			receive_message(message);
			//			system("pause");
			current_visualise->prepare_to_draw();
			Sleep(9);
			for (int j = 0;j < move_turn_player.size();j++)
			{
				receive_message(message);
				take_action(move_turn_player[j], i);
			}
			current_visualise->draw();

			current_visualise->after_draw_and_hold_window();
		}
		for (int i = 0;i < MOVE_STEP_COUNT;i++)
		{
			receive_message(message);
			//			system("pause");
			current_visualise->prepare_to_draw();

			for (int j = 0;j < turn_player.size();j++)
			{
				receive_message(message);
				take_action(turn_player[j], i);
			}
			current_visualise->draw();

			current_visualise->after_draw_and_hold_window();
		}
		end_turn();
		return message;
	}
	template<typename ... tail>
	static void create(int this_, int object, int x, int y,int target, tail...t,int step)
	{
		if (step == 0)
		{
			auto turn = (play_turn*)this_;
			auto obj = (base_object*)object;
			auto obj_vector = (list<base_object>*)target;
			builder b;
			base_object* new_object = b.build((t.object)...);
			new_object->x = x;
			new_object->y = y;
			new_object->refC = -1;

			obj_vector->push_back(*new_object);
			delete new_object;

			turn->current_map->push_in_cell(x, y, &obj_vector->back());
		}
	}
	void clean()
	{
		#define arr current_map->one_turn_object_without_grid_linking 
			for (auto &cur : arr)
				delete cur;
			arr.erase(arr.begin(), arr.end());
		#undef arr
	}
	template<typename... tail>
	void write_create_to_function_calling_list(base_object_ref object, int x, int y, tail...t)
	{
		auto current_function = create<tail...>;
		write_parameters_to_function_calling_list(turn_player, 0, (int)current_function, (int)object.object, x, y, (int)current_player_object, int(t.object)..., -9999);
	}

	void write_create_to_function_calling_list_1(base_object_ref& object, int x, int y, base_object_ref& object1)
	{
		write_create_to_function_calling_list(object, x,y, object1);
	}
	void write_create_to_function_calling_list_2(base_object_ref& object, int x, int y, base_object_ref& object1,base_object_ref& object2)
	{
		write_create_to_function_calling_list(object, x, y, object1,object2);
	}
	void write_create_to_function_calling_list_3(base_object_ref& object, int x, int y, base_object_ref& object1,base_object_ref& object2,base_object_ref& object3)
	{
		write_create_to_function_calling_list(object, x, y, object1, object2,object3);
	}
	void write_create_to_function_calling_list_4(base_object_ref& object, int x, int y, base_object_ref& object1,base_object_ref& object2,base_object_ref& object3,base_object_ref& object4)
	{
		write_create_to_function_calling_list(object, x, y, object1, object2, object3,object4);
	}
	void write_create_to_function_calling_list_5(base_object_ref& object, int x, int y, base_object_ref& object1,base_object_ref& object2,base_object_ref& object3,base_object_ref& object4,base_object_ref& object5)
	{
		write_create_to_function_calling_list(object, x, y, object1, object2, object3, object4, object5);
	}





	play_turn(void);
	~play_turn(void);
	void del_objects(list<base_object>&obj);
};

play_turn* play_turnFactoryE();

