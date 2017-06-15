#include "stdafx.h"
#include "map.h"


world_map::world_map(void)
{
	map_size_x=80;
	map_size_y=60;
	cell_size=10;
	incell_max_object_count = 100;

	grid.assign(map_size_x,vector<vector<base_object*>>());
	for(int i=0;i<map_size_x;i++)
		grid[i].assign(map_size_y, vector<base_object*>());
	for (int i = 0;i<map_size_x;i++)
		for (int j = 0;j<map_size_y;j++)
			grid[i][j].assign(0, NULL);

	one_turn_object_without_grid_linking.assign(0,NULL);

	possible_to_move = new bool*[map_size_x];
	for (int i = 0;i < map_size_x;i++)
		possible_to_move[i] = new bool[map_size_y];
	for (int i = 0;i < map_size_x;i++)
		for (int j = 0;j < map_size_y;j++)
			possible_to_move[i][j] = true;

}
bool world_map::is_possible_to_move(int x, int y)
{
	if (possible_to_move[x][y] == true)
		return true;
	return false;
}
void world_map::set_possible_to_move(int x, int y, bool possible)
{
	possible_to_move[x][y] = possible;
}

bool world_map::push_in_cell(int x, int y, base_object* obj)
{
	if (grid[x][y].size() < incell_max_object_count)
	{
		grid[x][y].push_back(obj);
		return 1;
	}
	return 0;
}

int world_map::find_in_cell(int x, int y, base_object* obj)
{
	for (int i = 0;i < grid[x][y].size();i++)
		if (grid[x][y][i] == obj)
			return i;
	return -1;
}

bool world_map::del_in_cell(int x, int y, int pos)
{
	if (pos < grid[x][y].size()&&pos>=0)
	{
		grid[x][y].erase(grid[x][y].begin() + pos);
		return 1;
	}
	return 0;
}

bool world_map::del_in_cell(int x, int y, base_object* obj)
{
	return del_in_cell(x, y, find_in_cell(x, y, obj));
}



world_map::~world_map(void)
{
}

world_map* world_mapFactoryE()
{
	return new world_map();
}

