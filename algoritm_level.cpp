#include "stdafx.h"
#include "algoritm_level.h"
#include <time.h>
#include "scriptarray.h"
#include "debugger.h"
CDebugger *g_dbg = 0;

algoritm_level* algoritm_levelFactoryE()
{
	return new algoritm_level();
}

void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";

	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}

void LineCallback(asIScriptContext *ctx, int timeOut)
{
	// If the time out is reached we abort the script
	if (timeOut < clock())
		ctx->Abort();

	// It would also be possible to only suspend the script,
	// instead of aborting it. That would allow the application
	// to resume the execution where it left of at a later 
	// time, by simply calling Execute() again.
}
void print(int n)
{
	cout << n << endl;
}
void print_(string s)
{
	cout << s << endl;
}

std::string string_cast(const wchar_t* pSource, unsigned int codePage = CP_ACP)
{
	assert(pSource != 0);
	size_t sourceLength = std::wcslen(pSource);
	if (sourceLength > 0)
	{
		int length = ::WideCharToMultiByte(codePage, 0, pSource, sourceLength, NULL, 0, NULL, NULL);
		if (length == 0)
			return std::string();

		std::vector<char> buffer(length);
		::WideCharToMultiByte(codePage, 0, pSource, sourceLength, &buffer[0], length, NULL, NULL);

		return std::string(buffer.begin(), buffer.end());
	}
	else
		return std::string();

}
CScriptArray * GetFilesInDirectory( wstring *directory)
{
	vector<string> out;
	HANDLE dir;
	WIN32_FIND_DATA file_data;
	
	dir = FindFirstFile((*directory + L"/*").c_str(), &file_data);

	do {
		const wstring file_name = file_data.cFileName;
		const wstring full_file_name = *directory + L"\\" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;
		if (file_name == L"")
			continue;
		if (file_name == L"std.as")
			continue;
		if (file_name == L"std2.as")
			continue;

		if (is_directory)
			continue;

		out.push_back(string_cast(file_name.c_str()));
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);

	asIScriptContext *ctx = asGetActiveContext();
	asIScriptEngine* engine = ctx->GetEngine();
	asITypeInfo* t = engine->GetTypeInfoByDecl("array<string>");
	CScriptArray * out_ = CScriptArray::Create(t, out.size());
	int i = 0;
	for (auto &cur : out)
	{
		out_->InsertAt(i, &out[i]);
		i++;
	}
	return out_;

} // GetFilesInDirectory
std::wstring widen(const std::string& in, std::locale loc = std::locale())
{
	std::wstring out;

	for (int i=0; i < in.size(); i++)
		out += std::use_facet<std::ctype<wchar_t> >(loc).widen(in[i]);


	return out;
}
CScriptArray * GetFilesInDirectory_( string directory)
{
	wstring temp = widen(directory);
	return GetFilesInDirectory(&temp);
}
algoritm_level* algoritm_level::get_script(int n)
{
	other_algoritms[n].second->AddRef();
	return other_algoritms[n].second;
}
void algoritm_level::add_script(string name, string function)
{
	int flag = -1;
	for (int i = 0;i < other_algoritms.size();i++)
		if (other_algoritms[i].first == name)
			flag = i;
	if (flag == -1)
	{
		other_algoritms.push_back(pair<string, algoritm_level*>(name, new algoritm_level(name, function, turn)));
	}
}
void algoritm_level::ReceiveMyObj_(int number,CScriptArray *obj)
{
	obj->AddRef();
	int r = ctx->SetArgObject(number, obj);
}
void algoritm_level::ReceiveMyObj(int number, int data)
{
	int r = ctx->SetArgDWord(number, data);
}
double algoritm_level::return_double()
{
	return ctx->GetReturnDouble();
}
void algoritm_level::ConfigureEngine(asIScriptEngine *engine, play_turn&turn)
{
	int r;
	// Register the script string type
	// Look at the implementation for this function for more information  
	// on how to register a custom string type, and other object types.
	RegisterStdString(engine);
	RegisterScriptArray(engine, true);

	// Register the functions that the scripts will be allowed to use.
	// Note how the return code is validated with an assert(). This helps
	// us discover where a problem occurs, and doesn't pollute the code
	// with a lot of if's. If an error occurs in release mode it will
	// be caught when a script is being built, so it is not necessary
	// to do the verification here as well.


	engine->RegisterObjectType("world_map", sizeof(world_map), asOBJ_REF);
	engine->RegisterObjectBehaviour("world_map", asBEHAVE_FACTORY, "world_map@ new_world_map()", asFUNCTION(world_mapFactoryE), asCALL_CDECL);
	engine->RegisterObjectBehaviour("world_map", asBEHAVE_ADDREF, "void AddRef()", asMETHOD(base_object, AddRef), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("world_map", asBEHAVE_RELEASE, "void Release()", asMETHOD(base_object, Release), asCALL_THISCALL);

	engine->RegisterObjectMethod("world_map", "	bool is_possible_to_move(int x, int y)", asMETHOD(world_map, is_possible_to_move), asCALL_THISCALL);
	engine->RegisterObjectMethod("world_map", "	void set_possible_to_move(int x, int y, bool possible)", asMETHOD(world_map, set_possible_to_move), asCALL_THISCALL);


	engine->RegisterObjectType("base_object", sizeof(base_object), asOBJ_REF);
	engine->RegisterObjectBehaviour("base_object", asBEHAVE_FACTORY, "base_object@ new_base_object()", asFUNCTION(base_objectFactoryE), asCALL_CDECL);
	engine->RegisterObjectBehaviour("base_object", asBEHAVE_ADDREF, "void AddRef()", asMETHOD(base_object, AddRef), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("base_object", asBEHAVE_RELEASE, "void Release()", asMETHOD(base_object, Release), asCALL_THISCALL);

	engine->RegisterObjectType("base_object_ref", sizeof(base_object), asOBJ_REF);
	engine->RegisterObjectBehaviour("base_object_ref", asBEHAVE_FACTORY, "base_object_ref@ base_object_ref()", asFUNCTION(base_object_ref_factory), asCALL_CDECL);
	engine->RegisterObjectBehaviour("base_object_ref", asBEHAVE_ADDREF, "void AddRef()", asMETHOD(base_object_ref, AddRef), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("base_object_ref", asBEHAVE_RELEASE, "void Release()", asMETHOD(base_object_ref, Release), asCALL_THISCALL);

	engine->RegisterObjectMethod("base_object_ref", "base_object_ref@ opAssign(const base_object_ref &in other)",asMETHOD(base_object_ref, operator=), asCALL_THISCALL);

	engine->RegisterObjectMethod("base_object_ref", "	int get_X()", asMETHOD(base_object_ref, get_X), asCALL_THISCALL);
	engine->RegisterObjectMethod("base_object_ref", "	void set_X(int x)", asMETHOD(base_object_ref, set_X), asCALL_THISCALL);
	engine->RegisterObjectMethod("base_object_ref", "	int get_Y()", asMETHOD(base_object_ref, get_Y), asCALL_THISCALL);
	engine->RegisterObjectMethod("base_object_ref", "	void set_Y(int x)", asMETHOD(base_object_ref, set_Y), asCALL_THISCALL);
	

	engine->RegisterGlobalFunction("void print(int n)", asFUNCTION(print), asCALL_CDECL);
	engine->RegisterGlobalFunction("void print_(string s)", asFUNCTION(print_), asCALL_CDECL);

	engine->RegisterObjectType("algoritm_level", 0, asOBJ_REF);
	engine->RegisterObjectBehaviour("algoritm_level", asBEHAVE_FACTORY, "algoritm_level@ new_algoritm_level()", asFUNCTION(algoritm_levelFactoryE), asCALL_CDECL);
	engine->RegisterObjectBehaviour("algoritm_level", asBEHAVE_ADDREF, "void AddRef()", asMETHOD(algoritm_level, AddRef), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("algoritm_level", asBEHAVE_RELEASE, "void Release()", asMETHOD(algoritm_level, Release), asCALL_THISCALL);

	engine->RegisterObjectMethod("algoritm_level", "int prepare_engine_and_context()", asMETHOD(algoritm_level, prepare_engine_and_context), asCALL_THISCALL);
	engine->RegisterObjectMethod("algoritm_level", "int one_turn_algoritm1()", asMETHOD(algoritm_level, one_turn_algoritm1), asCALL_THISCALL);
	engine->RegisterObjectMethod("algoritm_level", "int one_turn_algoritm2()", asMETHOD(algoritm_level, one_turn_algoritm2), asCALL_THISCALL);
	engine->RegisterObjectMethod("algoritm_level", "void end_work()", asMETHOD(algoritm_level, end_work), asCALL_THISCALL);
	engine->RegisterObjectMethod("algoritm_level", "algoritm_level@ get_script(int n)", asMETHOD(algoritm_level, get_script), asCALL_THISCALL);

	engine->RegisterGlobalProperty("algoritm_level scripts", this);
	engine->RegisterObjectProperty("algoritm_level", "bool is_builded", asOFFSET(algoritm_level, is_builded));
	engine->RegisterObjectProperty("algoritm_level", "bool error_while_build", asOFFSET(algoritm_level, error_while_build));
	
	engine->RegisterObjectType("play_turn", 0, asOBJ_REF);
	engine->RegisterObjectBehaviour("play_turn", asBEHAVE_FACTORY, "play_turn@ new_play_turn()", asFUNCTION(play_turnFactoryE), asCALL_CDECL);
	engine->RegisterObjectBehaviour("play_turn", asBEHAVE_ADDREF, "void AddRef()", asMETHOD(play_turn, AddRef), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("play_turn", asBEHAVE_RELEASE, "void Release()", asMETHOD(play_turn, Release), asCALL_THISCALL);

	engine->RegisterObjectType("builder", 0, asOBJ_REF);
	engine->RegisterObjectBehaviour("builder", asBEHAVE_FACTORY, "builder@ new_builder()", asFUNCTION(builderFactoryE), asCALL_CDECL);
	engine->RegisterObjectBehaviour("builder", asBEHAVE_ADDREF, "void AddRef()", asMETHOD(builder, AddRef), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("builder", asBEHAVE_RELEASE, "void Release()", asMETHOD(builder, Release), asCALL_THISCALL);

	engine->RegisterGlobalProperty("play_turn turn", &turn);
	engine->RegisterGlobalProperty("world_map map", turn.current_map);


	engine->RegisterObjectMethod("algoritm_level", "void add_script(string name, string function)", asMETHOD(algoritm_level, add_script), asCALL_THISCALL);

	builder* b=new builder();
	engine->RegisterGlobalProperty("builder b", b);
	engine->RegisterGlobalProperty("int MOVE_STEP_COUNT", &MOVE_STEP_COUNT);
	engine->RegisterGlobalProperty("int BREAK_PROGRAM", &BREAK_PROGRAM);
	engine->RegisterGlobalProperty("int RESTART_PROGRAM", &RESTART_PROGRAM);
	engine->RegisterGlobalProperty("bool PROGRAM_IS_REPEATABLE", &PROGRAM_IS_REPEATABLE);

	engine->RegisterGlobalFunction("array<string> @ GetFilesInDirectory_(string directory)", asFUNCTION(GetFilesInDirectory_), asCALL_CDECL);
	// Register an interface
	engine->RegisterInterface("IMyObj");
	// You can also register methods with the interface if you wish to force the script class to implement them
	// Register a function that receives a handle to the interface

	engine->RegisterObjectMethod("algoritm_level", "void ReceiveMyObj(int number,array<array<double>>@obj)", asMETHOD(algoritm_level, ReceiveMyObj_), asCALL_THISCALL);
	engine->RegisterObjectMethod("algoritm_level", "void ReceiveMyObj(int number,array<int>@obj)", asMETHOD(algoritm_level, ReceiveMyObj_), asCALL_THISCALL);
	engine->RegisterObjectMethod("algoritm_level", "void ReceiveMyObj(int number,int data)", asMETHOD(algoritm_level, ReceiveMyObj), asCALL_THISCALL);
	engine->RegisterObjectMethod("algoritm_level", "double return_double()", asMETHOD(algoritm_level, return_double), asCALL_THISCALL);

	engine->RegisterObjectMethod("play_turn", "array<base_object_ref>@ get_player_object()",asMETHOD(play_turn, get_player_object), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", "void move(base_object_ref@ object, int x, int y)", asMETHOD(play_turn, write_move_to_function_calling_list), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", "void move_(base_object_ref@ object, int x, int y)", asMETHOD(play_turn, write_move_to_function_calling_list_), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", "void fire(base_object_ref@ object, int x, int y)", asMETHOD(play_turn, write_fire_to_function_calling_list), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", "void clean()", asMETHOD(play_turn, clean), asCALL_THISCALL);

	engine->RegisterObjectMethod("play_turn", "int get_turn_number()", asMETHOD(play_turn, get_turn_number), asCALL_THISCALL);

	engine->RegisterObjectMethod("play_turn", "void change_map_size(int x, int y)", asMETHOD(play_turn, change_map_size), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", "void push_back_output(string s, int n)", asMETHOD(play_turn, push_back_output), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", "void push_back_output(string s, string s2)", asMETHOD(play_turn, push_back_output_), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", "void push_back_output(string s)", asMETHOD(play_turn, push_back_output__), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", "void output_clean()", asMETHOD(play_turn, output_clean), asCALL_THISCALL);

	engine->RegisterObjectMethod("play_turn", " void create(base_object_ref@ object, int x, int y,base_object_ref@ o)", asMETHOD(play_turn, write_create_to_function_calling_list_1), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", " void create(base_object_ref@ object, int x, int y,base_object_ref@ o,base_object_ref@ oo)", asMETHOD(play_turn, write_create_to_function_calling_list_2), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", " void create(base_object_ref@ object, int x, int y,base_object_ref@ o,base_object_ref@ oo,base_object_ref@ ooo)", asMETHOD(play_turn, write_create_to_function_calling_list_3), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", " void create(base_object_ref@ object, int x, int y,base_object_ref@ o,base_object_ref@ oo,base_object_ref@ ooo,base_object_ref@ oooo)", asMETHOD(play_turn, write_create_to_function_calling_list_4), asCALL_THISCALL);
	engine->RegisterObjectMethod("play_turn", " void create(base_object_ref@ object, int x, int y,base_object_ref@ o,base_object_ref@ oo,base_object_ref@ ooo,base_object_ref@ oooo,base_object_ref@ ooooo)", asMETHOD(play_turn, write_create_to_function_calling_list_5), asCALL_THISCALL);

	engine->RegisterObjectMethod("play_turn", " int one_turn()", asMETHOD(play_turn, one_turn), asCALL_THISCALL);

	engine->RegisterObjectMethod("play_turn", " void change_texture(string s, base_object_ref&base)", asMETHOD(play_turn, change_texture), asCALL_THISCALL);
	
	engine->RegisterObjectMethod("play_turn", " void set_visible(int x,int y, bool vis)", asMETHOD(play_turn, set_visible), asCALL_THISCALL);

	engine->RegisterObjectMethod("play_turn", " int rand_(int n)", asMETHOD(play_turn, rand_), asCALL_THISCALL);
	
	engine->RegisterObjectMethod("builder", " base_object_ref@ base_wheel_()", asMETHOD(builder, base_wheel_), asCALL_THISCALL);
	engine->RegisterObjectMethod("builder", " base_object_ref@ base_cannon_()", asMETHOD(builder, base_cannon_), asCALL_THISCALL);
	engine->RegisterObjectMethod("builder", " base_object_ref@ base_shell_()", asMETHOD(builder, base_shell_), asCALL_THISCALL);
	engine->RegisterObjectMethod("builder", " base_object_ref@ base_tank_()", asMETHOD(builder, base_tank_), asCALL_THISCALL);
	
	
	

	// It is possible to register the functions, properties, and types in 
	// configuration groups as well. When compiling the scripts it then
	// be defined which configuration groups should be available for that
	// script. If necessary a configuration group can also be removed from
	// the engine, so that the engine configuration could be changed 
	// without having to recompile all the scripts.
}

int algoritm_level::CompileScript(asIScriptEngine *engine)
{
	int r;
	// The builder is a helper class that will load the script file, 
	// search for #include directives, and load any included files as 
	// well.
	CScriptBuilder builder;

	// Build the script. If there are any compiler messages they will
	// be written to the message stream that we set right after creating the 
	// script engine. If there are no errors, and no warnings, nothing will
	// be written to the stream.
	r = builder.StartNewModule(engine, 0);
	if (r < 0)
	{
		cout << "Failed to start new module" << endl;
		return r;
	}
	r = builder.AddSectionFromFile(file_name.c_str());
	if (r < 0)
	{
		cout << "Failed to add script file" << endl;
		return r;
	}
	r = builder.BuildModule();
	if (r < 0)
	{
		cout << "Failed to build the module" << endl;
		return r;
	}

	// The engine doesn't keep a copy of the script sections after Build() has
	// returned. So if the script needs to be recompiled, then all the script
	// sections must be added again.

	// If we want to have several scripts executing at different times but 
	// that have no direct relation with each other, then we can compile them
	// into separate script modules. Each module use their own namespace and 
	// scope, so function names, and global variables will not conflict with
	// each other.

	return 0;
}
algoritm_level::algoritm_level(string file,string func,play_turn&turn_):file_name(file),func_name(func),turn(turn_)
{
	is_builded = false;
	error_while_build = false;
}


int algoritm_level::prepare_engine_and_context()
{
	int r;
	error_while_build = true;
	// Create the script engine
	engine = asCreateScriptEngine();

	if (engine == 0)
	{
		cout << "Failed to create script engine." << endl;
		return -1;
	}

	// The script compiler will write any compiler messages to the callback.
	engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

	// Configure the script engine with all the functions, 
	// and variables that the script should be able to use.
	ConfigureEngine(engine, turn);

	// Compile the script code
	r = CompileScript(engine);
	if (r < 0)
	{
		engine->Release();
		engine = 0;
		return -1;
	}

	// Create a context that will execute the script.
	ctx = engine->CreateContext();
	if (ctx == 0)
	{
		cout << "Failed to create the context." << endl;
		engine->Release();
		return -1;
	}

	// We don't want to allow the script to hang the application, e.g. with an
	// infinite loop, so we'll use the line callback function to set a timeout
	// that will abort the script after a certain time. Before executing the 
	// script the timeOut variable will be set to the time when the script must 
	// stop executing. 
	func = engine->GetModule(0)->GetFunctionByDecl(func_name.c_str());
	if (func == 0)
	{
		cout << "The function 'void main()' was not found." << endl;
		ctx->Release();
		engine->Release();
		return -1;
	}
	is_builded = true;
	error_while_build = false;
	return 0;
}
void InitializeDebugger(asIScriptEngine *engine)
{
	// Create the debugger instance and store it so the context callback can attach
	// it to the scripts contexts that will be used to execute the scripts
	g_dbg = new CDebugger();

	// Let the debugger hold an engine pointer that can be used by the callbacks
	g_dbg->SetEngine(engine);

	// Register the to-string callbacks so the user can see the contents of strings
	//g_dbg->RegisterToStringCallback(engine->GetTypeInfoByName("string"), StringToString);
	//g_dbg->RegisterToStringCallback(engine->GetTypeInfoByName("array"), ArrayToString);
	//g_dbg->RegisterToStringCallback(engine->GetTypeInfoByName("dictionary"), DictionaryToString);

	// Allow the user to initialize the debugging before moving on
	cout << "Debugging, waiting for commands. Type 'h' for help." << endl;
	g_dbg->TakeCommands(0);
}

int algoritm_level::one_turn_algoritm1()
{
	int timeOut;
	if (ctx != 0)
	{
		int r = ctx->SetLineCallback(asFUNCTION(LineCallback), &timeOut, asCALL_CDECL);
		if (r < 0)
		{
			cout << "Failed to set the line callback function." << endl;
			ctx->Release();
			engine->Release();
			return -1;
		}
	}
	// Find the function for the function we want to execute.

	// Prepare the script context with the function we wish to execute. Prepare()
	// must be called on the context before each new script function that will be
	// executed. Note, that if you intend to execute the same function several 
	// times, it might be a good idea to store the function returned by 
	// GetFunctionByDecl(), so that this relatively slow call can be skipped.
	if (ctx != 0)
	{

		int r = ctx->Prepare(func);
		if (r < 0)
		{
			cout << "Failed to prepare the context." << endl;
			ctx->Release();
			engine->Release();
			return -1;
		}
	}
	// Set the timeout before executing the function. Give the function 1 sec
	// to return before we'll abort it.
	timeOut = clock() + 1000;
	//	InitializeDebugger(engine);
		// Execute the function
}
int algoritm_level::one_turn_algoritm2()
{
	int r;
	if (ctx != 0)
	{

//		cout << "Executing the script." << endl;
//		cout << "---" << endl;
		r = ctx->Execute();
//		cout << "---" << endl;
		if (r != asEXECUTION_FINISHED)
		{
			// The execution didn't finish as we had planned. Determine why.
			if (r == asEXECUTION_ABORTED)
				cout << "The script was aborted before it could finish. Probably it timed out." << endl;
			else if (r == asEXECUTION_EXCEPTION)
			{
				cout << "The script ended with an exception." << endl;
	
				// Write some information about the script exception
				asIScriptFunction *func = ctx->GetExceptionFunction();
				cout << "func: " << func->GetDeclaration() << endl;
				cout << "modl: " << func->GetModuleName() << endl;
				cout << "sect: " << func->GetScriptSectionName() << endl;
				cout << "line: " << ctx->GetExceptionLineNumber() << endl;
				cout << "desc: " << ctx->GetExceptionString() << endl;
			}
			else
				cout << "The script ended for some unforeseen reason (" << r << ")." << endl;
		}
		else
		{
//			cout << "The script finished successfully." << endl;
		}
		turn.current_turn++;
	}

	return 0;
}
void algoritm_level::end_work()
{
	// We must release the contexts when no longer using them
	if(ctx!=0)
		ctx->Release();

	// Shut down the engine
	if(engine!=0)
		engine->ShutDownAndRelease();

}



algoritm_level::~algoritm_level()
{
}















