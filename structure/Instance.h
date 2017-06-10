#pragma once
#include "Solutions.h"
#include "Task.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

class Instance
{
private:
	Task tasks [Constance::n_tasks];
	Operation maintenance[Constance::n_maintenance*2];

	Solutions* solutions;
	int begin_time;
	void insertion_sort_machine_one( int index, int operations_on_machine );
	void insertion_sort_machine_two( int index, int operations_on_machine );
	void insertion_sort_solutions( int chosen_solutions_amount );
	int target_function( int index );

public:
	Instance();
	~Instance();
	int solutions_number;
	void generate_instance_to_file();
	void load_from_file(const string & filename);
	void generate_solutions();
	void save_solution_to_file(int number);
	void save_best_solution();
	void save_best_time();
	void save_begin_time();
	void selection( int time );
	void crossing();
	void mutation(int time);
	void set_begin_time( int new_time );
	Task get_task(int index){ return tasks[index]; }
	Operation get_maintenance(int index){ return maintenance[index]; }
	Solutions get_solution(int index){ return solutions[index]; }
};

