#include "stdafx.h"
#include "builder.h"
#include "object_visualiser.h"

base_object_ref builder::base_wheel=new wheel();
base_object_ref builder::base_cannon = new cannon();
base_object_ref builder::base_shell = new base_object();
base_object_ref builder::base_tank = new base_object();
long int  builder::id=0;

base_object_ref& builder::base_wheel_() { return  base_wheel; }
base_object_ref& builder::base_cannon_() { return base_cannon; }
base_object_ref& builder::base_shell_() { return  base_shell; }
base_object_ref& builder::base_tank_() { return base_tank; }
int builder::is_build(false);
builder::builder(void)
{
	if (!is_build)
	{
		object_visualiser* vis0=new object_visualiser();
		object_visualiser_texture* vis1 = new object_visualiser_texture();
		vis1->texture.loadFromFile("1.jpg");
		 
		object_visualiser_texture* viss = new object_visualiser_texture();
		viss->texture.loadFromFile("1.jpg");

		object_visualiser_texture* vis2 = new object_visualiser_texture();
		vis2->texture.loadFromFile("2.jpg");
		object_visualiser_texture* vis3 = new object_visualiser_texture();
		vis3->texture.loadFromFile("2.jpg");


		#define cur base_wheel.object->modules[0]
				cur = new wheel();
				cur->max_action_radius = 500;
				cur->is_flyweight = true;
				cur->vis = vis2;
		#undef cur
		
		#define cur base_cannon.object->modules[1]
				cur = new cannon();
				cur->max_action_radius = 500;
				cur->is_flyweight = true;
				cur->vis = vis3;
		#undef cur
		
		#define cur base_shell.object
				cur->object_wheel() = new wheel();
				cur->object_wheel()->max_action_radius = 500;
				cur->object_wheel()->vis = vis0;
				cur->is_flyweight = false;
				cur->vis = viss;
		#undef cur
		

		#define cur base_tank.object
				cur->is_flyweight = false;
				cur->vis = vis1;
		#undef cur
				is_build = true;
	}

}

builder* builderFactoryE()
{
	return new builder;
}
//
//builder::~builder(void)
//{
//}
