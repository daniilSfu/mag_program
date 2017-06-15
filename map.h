#pragma once
#include "base_object.h"
#include <list>
class world_map :
	public RefC
{
public:
	int map_size_x;
	int map_size_y;

	int cell_size;
	int incell_max_object_count;
	vector<vector<vector<base_object*>>> grid;
	bool push_in_cell(int x, int y, base_object* obj);
	int find_in_cell(int x, int y, base_object* obj);
	bool del_in_cell(int x, int y, int pos);
	bool del_in_cell(int x, int y, base_object* obj);

	bool**visible_player1;
	bool**visible_player2;

	bool**possible_to_move;
	bool is_possible_to_move(int x, int y);
	void set_possible_to_move(int x, int y, bool possible);

	list<base_object> objects_player1;
	list<base_object> objects_player2;
	list<base_object*> one_turn_object_without_grid_linking;

	world_map(void);
	~world_map(void);
};

world_map* world_mapFactoryE();
