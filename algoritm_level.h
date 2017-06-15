#pragma once
#include "play_turn.h"

#include <iostream>  // cout
#include <assert.h>  // assert()
#include <string.h>  // strstr()
#include <conio.h>   // kbhit(), getch()
#include <set>
#include "angelscript.h"
#include "scriptstdstring.h"
#include "scriptbuilder.h"


using namespace std;

//void GetFilesInDirectory(std::vector<string> &out, const wstring &directory);
//void GetFilesInDirectory_(std::vector<string> &out, const string &directory);


class algoritm_level :
	public RefC
{
public:
	string func_name;
	string file_name;
	bool is_builded;
	asIScriptObject *obj_;
	bool error_while_build;
	vector<pair<string, algoritm_level*>> other_algoritms;
	play_turn&turn;
	int prepare_engine_and_context();
	int one_turn_algoritm2();

	int one_turn_algoritm1();
	void end_work();
	int  CompileScript(asIScriptEngine *engine);
	void add_script(string name, string function);
	void ConfigureEngine(asIScriptEngine *engine, play_turn&turn);
	algoritm_level* get_script(int n);
	void ReceiveMyObj_(int number, CScriptArray *obj);
	void ReceiveMyObj(int number, int data);
	double return_double();

	asIScriptEngine *engine;
	asIScriptContext *ctx;
	asIScriptFunction *func;

//	void algoritm_step(play_turn&cur,visualise&v)
//	{
//		builder build_;
//		#define object0 cur.current_map->objects_player1[0]
//
//		cur.write_move_to_function_calling_list(base_object_ref(&object0), 10, 10);
//		cur.write_fire_to_function_calling_list(&object0, 17, 17);
//		cur.write_camera_move_to_function_calling_list(&v, 5, 5);
//
////		cur.write_create_to_function_calling_list(&object0, 17, 17, &cur.current_map->objects_player1, build_.base_tank, build_.base_cannon);
//
//	}

	algoritm_level(string file, string func, play_turn&turn_);
	algoritm_level():turn(*(new play_turn()))
	{
		is_builded = false;
		error_while_build = false;
	}
	~algoritm_level();
};

algoritm_level* algoritm_levelFactoryE();