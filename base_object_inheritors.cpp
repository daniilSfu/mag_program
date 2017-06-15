#include "stdafx.h"
#include "base_object_inheritors.h"
#include "builder.h"

wheel::wheel(void)
{
}
void wheel::move(bool is_testcase, double x, double y)
{
	if ((x > this->max_action_radius) || (y > this->max_action_radius))
		throw exception();
}
wheel::~wheel(void)
{
}
base_object* cannon::fire(double x, double y)
{
	builder b;

	base_object* shell=b.build(b.base_shell.object);
	shell->step_count = step_count - 1;

	if (step_count < 0)
		throw exception();

	return shell;
}

cannon::cannon(void)
{
}
cannon::~cannon(void)
{
}

object::object(void)
{
}
object::~object(void)
{
}
