#pragma once
class Operation
{
private:
	int start;
	int duration;
	bool maintenance; //1- maintenance,0-nie
	int task_index;
public:
	Operation();
	Operation( Operation &op );
	Operation( const int & start, const int & duration, bool maintenance, const int & task_index );
	bool set_start( const int & start );
	bool set_duration( const int & duration );
	void set_task_index( int index ){ this->task_index = index; }
	bool is_maintenance();
	int get_start();
	int get_duration();
	int get_task_index(){ return task_index; }
};

