// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "base_object.h"
#include "main_game_class.h"
#include "iostream"
#include "builder.h"
#include "algoritm_level.h"
#include "all_const.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "");
//	vector<string> s;
//	GetFilesInDirectory_(s, "C:\\PABCWork.NET");
//	for (int i = 0;i < s.size();i++)
//	{
//		cout << s[i].c_str() << endl;
//	}
	main_constants_class const_;
	world_map map;
	const_.current_play_turn_player1.current_map = &map;
	const_.current_visualise_player1.current_map = &map;
	const_.current_play_turn_player1.current_visualise = &const_.current_visualise_player1;
	string file_;
	cout << "input script file path" << endl;
	cin >> file_;
	builder build_;

	algoritm_level alg(file_, "int main()", const_.current_play_turn_player1);
	alg.prepare_engine_and_context();
	alg.one_turn_algoritm1();
	alg.one_turn_algoritm2();

	map.objects_player1.push_back(*build_.build(build_.base_tank.object,build_.base_wheel.object,build_.base_cannon.object));
	map.objects_player1.front().x=10;
	map.objects_player1.front().y=10;
	map.objects_player1.front().visible = false;
	const_.current_play_turn_player1.current_player_object = &map.objects_player1;
	map.grid[10][10].push_back(&map.objects_player1.front());


	const_.current_visualise_player1.camera_x = 0;
	const_.current_visualise_player1.camera_y = 0;

	alg.one_turn_algoritm1();
	alg.one_turn_algoritm2();



	do
	{
		//      		alg.algoritm_step(const_.current_play_turn_player1, const_.current_visualise_player1);
		alg.one_turn_algoritm1();
		alg.one_turn_algoritm2();
		int n = const_.current_play_turn_player1.one_turn();
		if (n == BREAK_PROGRAM) 
			break;

	}
	while (PROGRAM_IS_REPEATABLE);
	alg.end_work();
	//while(true)
	//{
	//	const_.current_play_turn_player1.visualise_class->prepare_to_draw();
	//	const_.current_play_turn_player1.move_turn_count=6;
	//	const_.current_play_turn_player1.write_move_to_function_calling_list(&object,10,10);
	//	const_.current_play_turn_player1.take_action(const_.current_play_turn_player1.move_turn_player[0],0);
	//	const_.current_play_turn_player1.visualise_class->after_draw_and_hold_window();
	//}
	return 0;
}