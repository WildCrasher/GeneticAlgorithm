#pragma once

class Constance
{
    public:
        static const int n_instances = 50;
		static const int n_tasks = 50;
        static const int n_operations = n_tasks*2;
        static const int n_maintenance = n_tasks/10;
        static const int n_solutions = 100;
        static const int duration_maintenance = 5;
        static const int crossed_solutions = 100;
        static const int time_op = 20;
        static const int ready_time_op = 6;
};
