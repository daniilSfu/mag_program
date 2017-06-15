#pragma once
#include <vector>
#include "object_visualiser.h"
using namespace std;


//переделать конструктор по умолчанию!!!!!!!!!!!!!!
class RefC
{
private:
	int refC;
public:
	RefC() { refC = 1; }
	void AddRef() { refC++; }
	void Release()
	{
		if (!--refC)
			delete this;
	}
};
class base_object
{
private:
public:
	int hp;
	int refC;
	bool visible;
	void AddRef() { refC++; }
	void Release()
	{
		if (!--refC)
			delete this;
	}

	int id;
	object_visualiser*vis;
	vector<base_object*> modules;
	base_object*&object_wheel() { return modules[0]; };
	base_object*&object_cannon() { return modules[1]; };
	int step_count;
	bool operator==(const base_object&x)
	{
		return id==x.id;
	}
	int cost;
	int weigth;
	double x;
	double y;
	int angle;//in deg

	bool is_flyweight;
	int max_action_radius;
	virtual void move(bool is_testcase, double x, double y);
	bool is_valid();
	bool copy_properties_to(base_object*& obj);
	virtual base_object* fire(double x, double y);

	virtual void draw(double x, double y, sf::RenderWindow* window);

	//base_object* operator=(const base_object& _rval)
	//{
	//	x = _rval.x;
	//	y = _rval.y;
	//	modules = _rval.modules;
	//	texture = _rval.texture;
	//	return this;
	//}

	base_object(void);
	~base_object(void);
};

class base_object_ref
{
public:
	base_object*object;
	base_object_ref&operator= (const base_object_ref& right)
	{
		object = right.object;
		return *this;
	}
	base_object_ref(const base_object_ref& b)
	{
		object = b.object;
	}
	base_object_ref(base_object* b)
	{
		object = b;
	}
	base_object_ref()
	{
		object = new base_object();
	}
	void AddRef()
	{
		if(object->refC !=(-1))
			object->refC++;
	}
	void Release()
	{
		if (object->refC != (-1))
			object->refC--;
//		if (!object->refC)
//			delete object;
	}
	int get_X()
	{
		return object->x;
	}
	void set_X(int x_)
	{
		object->x=x_;
	}
	int get_Y()
	{
		return object->y;
	}
	void set_Y(int y_)
	{
		object->y = y_;
	}


};
base_object_ref*base_object_ref_factory();



base_object* base_objectFactoryE();
