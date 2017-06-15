#pragma once
#include "base_object.h"
#include "base_object_inheritors.h"
class builder:public RefC
{
public:
//	typedef OtherVariadicTemplate<Types* ...> SomeOtherVT;
	static base_object_ref base_wheel;
	static base_object_ref base_cannon;
	static base_object_ref base_shell;
	static base_object_ref base_tank;
	static long int id;
	static int is_build;

	base_object_ref& base_wheel_();
	base_object_ref& base_cannon_();
	base_object_ref& base_shell_();
	base_object_ref& base_tank_();

	template<typename first>
	void build_second_recur(first*& new_obj){}

	template<typename first, typename... tail>
	void build_second_recur(first*& new_obj, first* f, tail*... objects)
	{
		f->copy_properties_to(new_obj);
		build_second_recur(new_obj, objects...);
	}

	template<typename first, typename... tail>
	first* build(first* f,tail*... objects)
	{
		first*new_object;
		new_object = NULL;

		f->copy_properties_to(new_object);
		build_second_recur(new_object,f,objects...);

		new_object->id = id;
		id++;

		return new_object;
	}
	builder();

	~builder(void) {};
};

builder* builderFactoryE();