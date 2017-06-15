#pragma once
#include "base_object.h"
class wheel :
	public base_object
{
public:
	wheel(void);
	void move(bool is_testcase, double x, double y);
	~wheel(void);
};
class cannon :
	public base_object
{
public:
	base_object* fire(double x, double y);
	cannon(void);
	~cannon(void);
};
class object :
	public base_object
{
public:
	object(void);
	~object(void);
};
