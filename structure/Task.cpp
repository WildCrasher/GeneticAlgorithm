#include "Task.h"


Task::Task() :op1(), op2()
{
	ready_time = 0;
	index = 0;
}

Task::Task(const int & start1, const int & dur1, const int & start2, const int & dur2, const int & ready_time,const int & index)
: op1(start1, dur1, 0,index), op2(start2,dur2,0,index)
{
this->ready_time = ready_time;
this->index = index;
}

Operation* Task::get_operation1()
{
	return &op1;
}

Operation* Task::get_operation2()
{
	return &op2;
}

void Task::set_index(const int & index)
{
	this->index = index;
	op1.set_task_index(index);
	op2.set_task_index(index);
}
