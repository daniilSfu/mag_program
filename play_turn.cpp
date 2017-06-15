#include "stdafx.h"
#include "play_turn.h"
#include "iostream"
#include "all_const.h"
#define PI 3.14159265

typedef void(*pmove0)();
typedef void(*pmove1)(int);
typedef void(*pmove2)(int, int);
typedef void(*pmove3)(int, int, int);
typedef void(*pmove4)(int, int, int, int);
typedef void(*pmove5)(int, int, int, int, int);
typedef void(*pmove6)(int, int, int, int, int, int);
typedef void(*pmove7)(int, int, int, int, int, int,int);
typedef void(*pmove8)(int, int, int, int, int, int, int,int);
typedef void(*pmove9)(int, int, int, int, int, int, int, int,int);

play_turn* play_turnFactoryE()
{
	return new play_turn();
}


void clean_command_array(std::vector<command>&commands)
{
	for (int i = commands.size()-1;i >= 0;i--)
	{
		if (commands[i].delay<=0)
		{
			commands.erase(commands.begin() + i);
//			i++;
		}
	}
	for (int i = commands.size()-1;i >= 0;i--)
		commands[i].delay--;
}
void play_turn::del_objects(list<base_object>&obj)
{
	for (auto &cur : obj)
		cur.step_count = STEP_COUNT;
	bool flag = true;
	while (flag == true)
	{
		for (auto &cur : obj)
		{
			if (cur.hp < 1)
			{
				if (current_map->del_in_cell(cur.x, cur.y, &cur) == 0)
					cout << "err" << endl;
				obj.remove(cur);
				break;
			}
		}
		flag = false;
	}
}

void play_turn::end_turn()
{
	clean_command_array(move_turn_player);
	clean_command_array(turn_player);


	del_objects(current_map->objects_player1);

	del_objects(current_map->objects_player2);

}
CScriptArray* play_turn::get_player_object()
{
	// If called from the script, there will always be an active 
	// context, which can be used to obtain a pointer to the engine.
	asIScriptContext *ctx = asGetActiveContext();
	if (ctx)
	{
		asIScriptEngine* engine = ctx->GetEngine();
		// The script array needs to know its type to properly handle the elements.
		// Note that the object type should be cached to avoid performance issues
		// if the function is called frequently.
		asITypeInfo* t = engine->GetTypeInfoByDecl("array<base_object_ref>");
		// Create an array with the initial size of 3 elements
		CScriptArray* arr_=CScriptArray::Create(t, this->current_map->objects_player1.size());
		int i = 0;
		for (auto &cur : this->current_map->objects_player1)
		{
			arr_->InsertAt(i, &base_object_ref(&cur));
			i++;
		}
		// The ref count for the returned handle was already set in the array's constructor
		return arr_;
	}
	return 0;
}
void play_turn::fire__(int this_,int object,int x2,int y2,int step)
{
	if(step==0)
	{
		base_object *obj=(base_object*)object;
		play_turn*turn=(play_turn*)this_;

		base_object*temp=obj->fire(x2,y2);

 		turn->current_map->one_turn_object_without_grid_linking.push_back(temp);
		turn->write_shell_move_to_function_calling_list(turn->current_map->one_turn_object_without_grid_linking.back(),x2,y2);

		//проверки возможности совершить действие 
	}
}
void play_turn::camera_move(int this_,int visualise_, int x, int y,int step)
{
  	if (step < MOVE_STEP_COUNT+1)
	{
		play_turn*turn = (play_turn*)this_;
		visualise*vis = (visualise*)visualise_;
		vis->camera_x = double(vis->camera_x) + ((double(x - vis->camera_x)) / (MOVE_STEP_COUNT - step));
		vis->camera_y = double(vis->camera_y) + ((double(y - vis->camera_y)) / (MOVE_STEP_COUNT - step));
		vis->camera_x = max(vis->camera_x, 0);
		vis->camera_y = max(vis->camera_y, 0);
		//		obj->move(false, dx, dy);

	}

}
//спустить вниз обработку размера шага?
void play_turn::move_(int this_, int object, int x2, int y2, int step)
{
	base_object *obj = (base_object*)object;
	play_turn*turn = (play_turn*)this_;
	if (step == 0)
	{
		if (turn->current_map->is_possible_to_move(x2, y2))
		{
			if ((obj->x != x2) || (obj->y != y2))
			{
				if (turn->current_map->find_in_cell(obj->x, obj->y, obj) != -1)
				{
					turn->current_map->push_in_cell(x2, y2, obj);
					turn->current_map->del_in_cell(obj->x, obj->y, obj);
				}
				else
					throw exception();
			}
		}
		else
			throw exception();
		//проверки возможности совершить действие 
		obj->move(true, x2, y2);
	}
	else
		if (step<MOVE_STEP_COUNT + 1)
		{

//			obj->x += rand() % 3 - 1;
//			obj->y += rand() % 3 - 1;
			double dx = double(obj->x) + ((double(x2 - obj->x)) / (MOVE_STEP_COUNT - step));
			double dy = double(obj->y) + ((double(y2 - obj->y)) / (MOVE_STEP_COUNT - step));

			builder b;

			base_object* shell = b.build(b.base_shell.object);
			shell->step_count = 1000;

			shell->x = obj->x;
			shell->y = obj->y;

			shell->visible = true;

			turn->current_map->one_turn_object_without_grid_linking.push_back(shell);

			obj->move(false, dx, dy);

			//			turn->visualise_class->draw(obj,obj->x+dx,obj->y+dy,atan(dx/dy)* 180.0 / PI);
		}
}

void play_turn::move(int this_,int object,int x2,int y2,int step)
{
	base_object *obj = (base_object*)object;
	play_turn*turn = (play_turn*)this_;
	if(step==0)
	{
		if (turn->current_map->is_possible_to_move(x2, y2))
		{
			if ((obj->x != x2) || (obj->y != y2))
			{
				if (turn->current_map->find_in_cell(obj->x, obj->y, obj) != -1)
				{
					turn->current_map->push_in_cell(x2, y2, obj);
					turn->current_map->del_in_cell(obj->x, obj->y, obj);
				}
				else
					throw exception();
			}
		}
		else
			throw exception();
		//проверки возможности совершить действие 
		obj->move(true, x2, y2);
	}
	else
		if(step<MOVE_STEP_COUNT+1)
		{
//			obj->x += rand() % 3-1;
//			obj->y += rand() % 3-1;
			double dx = double(obj->x) + ((double(x2 - obj->x)) / (MOVE_STEP_COUNT - step));
			double dy = double(obj->y) + ((double(y2 - obj->y)) / (MOVE_STEP_COUNT - step));

			obj->move(false,dx,dy);

//			turn->visualise_class->draw(obj,obj->x+dx,obj->y+dy,atan(dx/dy)* 180.0 / PI);
		}
}

void play_turn::shell_move(int this_, int object, int x2, int y2, int step)
{
	base_object *obj = (base_object*)object;
	play_turn*turn = (play_turn*)this_;
	if (step == 0)
	{
		//проверки возможности совершить действие 
		obj->move(true, x2, y2);
	}
	else
		if (step<MOVE_STEP_COUNT)
		{
			double dx = double(obj->x) + ((double(x2 - obj->x)) / (MOVE_STEP_COUNT - step));
			double dy = double(obj->y) + ((double(y2 - obj->y)) / (MOVE_STEP_COUNT - step));

			obj->move(false, dx, dy);
			
			//			turn->visualise_class->draw(obj,obj->x+dx,obj->y+dy,atan(dx/dy)* 180.0 / PI);
		}
	if (step == 5)
	{
		if (turn->current_map->grid[x2][y2].size()>0)
		{
			for(int i=0;i<turn->current_map->grid[x2][y2].size();i++)
			turn->current_map->grid[x2][y2][i]->hp -= 1;
		}

	}
}

void play_turn::write_parameters_to_function_calling_list(vector<command>&command_vector,int delay,int a,...) //Так объявляется функция с заранее неизвестным числом параметров
{
  int *P=&a; //Взятие адреса у первого параметра

  command_vector.push_back(command(delay,vector<int>()));
  command_vector[command_vector.size()-1].push_back((int)this);

  while (*P!=-9999)
   {
	   command_vector[command_vector.size()-1].push_back(*P);//Вытаскиваем значение с адреса по которому живет параметр 
     P++; //Адресная арифметика. Смена текущего адреса на следующий
   }
 
}

void play_turn::write_move_to_function_calling_list(base_object_ref& object,int x,int y)
{
	auto current_function=move;
	write_parameters_to_function_calling_list(move_turn_player,0,(int)current_function, (int)(object.object),x,y,-9999);
}

void play_turn::write_move_to_function_calling_list_(base_object_ref& object, int x, int y)
{
	auto current_function = move_;
	write_parameters_to_function_calling_list(move_turn_player, 0, (int)current_function, (int)(object.object), x, y, -9999);
}

void play_turn::write_camera_move_to_function_calling_list(visualise*visualise_, int x, int y)
{
	auto current_function = camera_move;
	write_parameters_to_function_calling_list(move_turn_player, 0, (int)current_function,(int)visualise_, x, y, -9999);
}


void play_turn::write_shell_move_to_function_calling_list(base_object* object,int x,int y)
{
	auto current_function=shell_move;
	write_parameters_to_function_calling_list(turn_player,0,(int)current_function,(int)object,x,y,-9999);
}

void play_turn::write_fire_to_function_calling_list(base_object_ref& object,int x,int y)
{
	auto current_function=fire__;
	write_parameters_to_function_calling_list(turn_player,0,(int)current_function, (int)(object.object),x,y,-9999);
}


void play_turn::take_action(command&vec,int step)
{
	if(vec.delay==0)
	{
		if (vec.size() == 9)
			((pmove9)(vec[1]))(vec[0], vec[2], vec[3], vec[4], vec[5], vec[6], vec[7], vec[8], step);
		if (vec.size() == 8)
			((pmove8)(vec[1]))(vec[0], vec[2], vec[3], vec[4], vec[5], vec[6],vec[7], step);
		if (vec.size() == 7)
			((pmove7)(vec[1]))(vec[0], vec[2], vec[3], vec[4], vec[5],vec[6], step);
		if (vec.size() == 6)
			((pmove6)(vec[1]))(vec[0], vec[2], vec[3], vec[4], vec[5], step);
		if (vec.size() == 5)
			((pmove5)(vec[1]))(vec[0], vec[2], vec[3], vec[4], step);
		if (vec.size() == 4)
			((pmove4)(vec[1]))(vec[0], vec[2], vec[3], step);
		if (vec.size() == 3)
			((pmove3)(vec[1]))(vec[0], vec[2], step);
		if (vec.size() == 2)
			((pmove2)(vec[1]))(vec[0], step);
		if (vec.size() == 1)
			((pmove1)(vec[1]))(step);
	}
}


play_turn::play_turn(void)
{
	current_turn = 0;
}


play_turn::~play_turn(void)
{
}
