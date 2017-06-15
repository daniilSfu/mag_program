#include "stdafx.h"
#include "base_object.h"
#include <cmath>
#include <string>
#include "all_const.h"
using namespace std;

base_object* base_objectFactoryE()
{
	return new base_object();
}
base_object_ref*base_object_ref_factory()
{
	return new base_object_ref();
}

base_object::base_object():modules(10, NULL)
{
	refC = 1;
	step_count= STEP_COUNT;
	hp = 10;
	visible = false;
}
void base_object::move(bool is_testcase,double x,double y)
{
	if (is_testcase)
	{
		this->object_wheel()->move(is_testcase, abs(this->x - x), abs(this->y - y));
	}
	else
	{ 
		this->x=x;
		this->y=y;
		step_count--;
		if(step_count<0) 
			throw exception();
	}

  }

//переписать
bool base_object::is_valid()
{
	return true;
}

bool base_object::copy_properties_to(base_object*& obj)
{
	if(obj==NULL)
	{
		obj=new base_object();
		(*obj) = {*this};

	}
	if(!is_flyweight)
	{
		for(int i=0;i<modules.size();i++)
			if(modules[i]!=NULL)
			{
				modules[i]->copy_properties_to(obj->modules[i]);
			}

		if(obj->is_valid())
			return true;
		else
			return false;
	}
	else
	{
		obj=this;
		return true;
	}
}
//подставить константу вместо необходимого количества шагов
base_object* base_object::fire(double x,double y)
{
 	auto temp= object_cannon()->fire(abs(this->x - x), abs(this->y - y));
	temp->x = this->x;
	temp->y = this->y;
	return temp;

}

void base_object::draw(double x, double y, sf::RenderWindow * window)
{
	if (visible == true)
	{
		vis->visualise(x, y, window);
		for (int i = 0;i < modules.size();i++)
			if (modules[i] != NULL)
				modules[i]->vis->visualise(x, y, window);
	}
}



base_object::~base_object(void)
{
}
