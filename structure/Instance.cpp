#include "Instance.h"
#include <windows.h>

Instance::Instance()
{
    solutions = new Solutions[Constance::n_solutions + Constance::crossed_solutions];
    solutions_number = Constance::n_solutions;
    begin_time = 0;
}

Instance::~Instance()
{
	delete[] this->solutions;
}

void Instance::set_begin_time( int new_time )
{
    this->begin_time = new_time;
}

void Instance::generate_instance_to_file()
{
    int time_op1, time_op2, machine_number_one, machine_number_two, ready_time;
    for( int i = 1; i <= Constance::n_instances; i++ )
    {
        ostringstream i_s;
        i_s << i;                     //konwersja z int na string
        string file_name;
        file_name = "../Instances/Instancja" + i_s.str() + ".txt";   //nazwa pliku (scie¿ka)
        ofstream file;
        file.open(file_name.c_str());
        if( file.good() )
        {
            file << Constance::n_tasks << ";" << endl;

            for ( int i = 1; i <= Constance::n_tasks; i++ ) //operacje
            {
                time_op1 = (rand()%Constance::time_op) + 1;
                time_op2 = (rand()%Constance::time_op) + 1;         //do poustawiania
                machine_number_one = 1;
                machine_number_two = 2;
                ready_time = (rand()%Constance::ready_time_op);


                file << time_op1 << ";"
                        << time_op2 << ";"
                                << machine_number_one << ";"
                                        << machine_number_two << ";"
                                                << ready_time << ";" << endl;
            }

            file << "MAINTENANCE" << endl;
            int start_maintenance = (rand()%Constance::time_op) + 5;
            for ( int i = 1; i <= Constance::n_maintenance*2; i++ )  //maintenancy
            {
                if( i > Constance::n_maintenance ) machine_number_one = 2;
                else machine_number_one = 1;
                file << i << ";"
                    << machine_number_one << ";"
                        << Constance::duration_maintenance << ";"
                            << start_maintenance << ";" << endl;
                start_maintenance = start_maintenance + Constance::duration_maintenance + (rand()%Constance::time_op) + 1;
            }
        }
        else cout << "Blad otwarcia pliku" << endl;

        file.close();
    }
}

void Instance::load_from_file(const string & filename)
{
	string temp;
	string temp2;
	ifstream file;
	int index1 = 0;
	int index2 = 0;
	file.open(filename);
	if (file.is_open())
	{
		getline(file, temp);
		for (int i = 0; i < Constance::n_tasks; i++)
		{
			getline(file, temp);
			while (temp[index2] != ';')
				index2++;
			temp2 = temp.substr(index1, index2);
			tasks[i].get_operation1()->set_duration(atoi(temp2.c_str()));

			index2++;
			index1 = index2;
			while (temp[index2] != ';')
				index2++;
			temp2 = temp.substr(index1, index2);
			tasks[i].get_operation2()->set_duration(atoi(temp2.c_str()));

			index2 = 0;
			for (int j = 0; j < 4; j++)
			{
				while (temp[index2] != ';')
					index2++;
				index2++;
			}

			temp2 = temp.substr(index2);
			temp2.pop_back();
			tasks[i].set_ready_time(atoi(temp2.c_str()));
			tasks[i].set_index(i);
			index1 = index2 = 0;
		}
		//wczytywanie maintenance
		int task_index = 0;
		int duration = 0;
		int start = 0;
		getline(file, temp);
		for (int i = 0; i < Constance::n_maintenance * 2; i++)
		{
			getline(file, temp);
			while (temp[index2] != ';')
				index2++;
			temp2 = temp.substr(index1, index2);
			task_index = atoi(temp2.c_str());

			index2++;
			while (temp[index2] != ';')
				index2++;

			index2++;
			index1 = index2;
			while (temp[index2] != ';')
				index2++;
			temp2 = temp.substr(index1, index2-index1);
			duration = atoi(temp2.c_str());

			index2++;
			index1 = index2;
			while (temp[index2] != ';')
				index2++;
			temp2 = temp.substr(index1, index2-index1);
			start = atoi(temp2.c_str());
			index1 = index2 = 0;
			maintenance[i] = Operation(start, duration, true, task_index);
		}
	}
	else
		cout << "nie dziala" << endl;

	file.close();
}

void Instance::insertion_sort_machine_one( int solution_index, int operations_on_machine )
{
    int j;
    Operation* pom;
    for ( int i = 1; i < operations_on_machine; i++ )
    {
        j = i - 1;
        while ( (j >= 0) && (solutions[solution_index].get_machine_one()[j + 1]->get_start() < solutions[solution_index].get_machine_one()[j]->get_start()) )
        {
            pom = solutions[solution_index].get_machine_one()[j + 1];
            solutions[solution_index].get_machine_one()[j + 1] = solutions[solution_index].get_machine_one()[j];
            solutions[solution_index].get_machine_one()[j] = pom;
            j--;
        }
    }
}

void Instance::insertion_sort_machine_two( int solution_index, int operations_on_machine )
{
    int j;
    Operation* pom;
    for ( int i = 1; i < operations_on_machine; i++ )
    {
        j = i - 1;
        while ( (j >= 0) && (solutions[solution_index].get_machine_two()[j + 1]->get_start() < solutions[solution_index].get_machine_two()[j]->get_start()) )
        {
            pom = solutions[solution_index].get_machine_two()[j + 1];
            solutions[solution_index].get_machine_two()[j + 1] = solutions[solution_index].get_machine_two()[j];
            solutions[solution_index].get_machine_two()[j] = pom;
            j--;
        }
    }
}

void Instance::insertion_sort_solutions( int chosen_solutions_amount )
{
    int j;
    Solutions pom;
    for ( int i = chosen_solutions_amount + 1; i < Constance::crossed_solutions + Constance::n_solutions; i++ )
    {
        j = i - 1;
        while ( (j >= chosen_solutions_amount) && (target_function( j + 1 ) < target_function( j )) )
        {
            pom = solutions[j + 1];
            solutions[j + 1] = solutions[j];
            solutions[j] = pom;
            j--;
        }
    }
}

void Instance::generate_solutions()
{
    int task_index, used[Constance::n_tasks], index_on_machine, j, operations_on_machine;

    for ( int ind = 0; ind < Constance::n_solutions; ind++ )
    {
        for ( int i = 0; i < Constance::n_tasks; i++ ) used[i] = 0;
        j = 0;
        task_index = rand() % (Constance::n_tasks);
        index_on_machine = 0;

        //maszyna  nr  1
        for ( int k = 0; k < Constance::n_maintenance; k++ )
        {
            solutions[ind].get_machine_one()[k] = &maintenance[k];
        }
        operations_on_machine = Constance::n_maintenance;

        while ( j < Constance::n_tasks )
        {
            while( used[task_index] == 1 )  task_index = rand()%(Constance::n_tasks);

            index_on_machine = 0;
            while( index_on_machine < Constance::n_tasks + Constance::n_maintenance )
            {
                if( index_on_machine == 0 )
                {
                    if( tasks[task_index].get_ready_time() + tasks[task_index].get_operation1()->get_duration() < solutions[ind].get_machine_one()[index_on_machine]->get_start())
                    {
                        tasks[task_index].get_operation1()->set_start( tasks[task_index].get_ready_time() );
                        solutions[ind].get_machine_one()[operations_on_machine] = new Operation(*tasks[task_index].get_operation1());
                        break;
                    }
                }
                else if(  solutions[ind].get_machine_one()[index_on_machine] == nullptr )
                {
                    if( tasks[task_index].get_ready_time() <= solutions[ind].get_machine_one()[index_on_machine - 1]->get_start() + solutions[ind].get_machine_one()[index_on_machine - 1]->get_duration())
                    {
                        tasks[task_index].get_operation1()->set_start( solutions[ind].get_machine_one()[index_on_machine - 1]->get_start() + solutions[ind].get_machine_one()[index_on_machine - 1]->get_duration() );
                        solutions[ind].get_machine_one()[operations_on_machine] = new Operation(*tasks[task_index].get_operation1());
                        break;
                    }
                    else
                    {
                        tasks[task_index].get_operation1()->set_start( tasks[task_index].get_ready_time() );
                        solutions[ind].get_machine_one()[operations_on_machine] = new Operation(*tasks[task_index].get_operation1());
                        break;
                    }
                }
                else
                {
                    if( (tasks[task_index].get_ready_time() <= solutions[ind].get_machine_one()[index_on_machine - 1]->get_start() + solutions[ind].get_machine_one()[index_on_machine - 1]->get_duration()) &&
                        solutions[ind].get_machine_one()[index_on_machine - 1]->get_start() + solutions[ind].get_machine_one()[index_on_machine - 1]->get_duration() + tasks[task_index].get_operation1()->get_duration() < solutions[ind].get_machine_one()[index_on_machine]->get_start())
                    {
                        tasks[task_index].get_operation1()->set_start( solutions[ind].get_machine_one()[index_on_machine - 1]->get_start() + solutions[ind].get_machine_one()[index_on_machine - 1]->get_duration() );
                        solutions[ind].get_machine_one()[operations_on_machine] = new Operation(*tasks[task_index].get_operation1());
                        break;
                    }

                }
                index_on_machine++;
            }
            operations_on_machine++;
            insertion_sort_machine_one(ind, operations_on_machine);
            used[task_index] = 1;
            j++;

        }
		solutions[ind].set_machine_one_operations_number(operations_on_machine);
        //maszyna nr 2
        for ( int i = 0; i < Constance::n_tasks; i++ ) used[i] = 0;
        index_on_machine = 0;
        j = 0;

        task_index = rand() % (Constance::n_tasks);

        for ( int k = Constance::n_maintenance ; k < Constance::n_maintenance * 2; k++ )
        {
            solutions[ind].get_machine_two()[k - Constance::n_maintenance] = &maintenance[k];
        }
        operations_on_machine = Constance::n_maintenance;

        while ( j < Constance::n_tasks )
        {
            while( used[task_index] == 1 )  task_index = rand()%(Constance::n_tasks);

            index_on_machine = 0;
            while( index_on_machine < Constance::n_tasks + Constance::n_maintenance )
            {
                if( index_on_machine == 0 )
                {//zmiana ponizszego warunku
                    if( tasks[task_index].get_operation1()->get_start() + tasks[task_index].get_operation1()->get_duration() + tasks[task_index].get_operation2()->get_duration() < solutions[ind].get_machine_two()[index_on_machine]->get_start() )
                    {
                        tasks[task_index].get_operation2()->set_start( tasks[task_index].get_operation1()->get_start() + tasks[task_index].get_operation1()->get_duration() );
                        solutions[ind].get_machine_two()[operations_on_machine] = new Operation(*tasks[task_index].get_operation2());
                        break;
                    }
                }
                else if(  solutions[ind].get_machine_two()[index_on_machine] == nullptr )
                {
                    tasks[task_index].get_operation2()->set_start( solutions[ind].get_machine_two()[index_on_machine - 1]->get_start() + solutions[ind].get_machine_two()[index_on_machine - 1]->get_duration() );
                    solutions[ind].get_machine_two()[operations_on_machine] = new Operation(*tasks[task_index].get_operation2());
                    break;
                }
                else
                {
                    if( (tasks[task_index].get_operation1()->get_start() + tasks[task_index].get_operation1()->get_duration() <= solutions[ind].get_machine_two()[index_on_machine - 1]->get_start() + solutions[ind].get_machine_two()[index_on_machine - 1]->get_duration()) &&
                        solutions[ind].get_machine_two()[index_on_machine - 1]->get_start() + solutions[ind].get_machine_two()[index_on_machine - 1]->get_duration() + tasks[task_index].get_operation2()->get_duration() < solutions[ind].get_machine_two()[index_on_machine]->get_start())
                    {
                        tasks[task_index].get_operation2()->set_start( solutions[ind].get_machine_two()[index_on_machine - 1]->get_start() + solutions[ind].get_machine_two()[index_on_machine - 1]->get_duration() );
                        solutions[ind].get_machine_two()[operations_on_machine] = new Operation(*tasks[task_index].get_operation2());
                        break;
                    }
                }
                index_on_machine++;
            }
            operations_on_machine++;

            insertion_sort_machine_two(ind, operations_on_machine);
            used[task_index] = 1;
            j++;
        }
		solutions[ind].set_machine_two_operations_number(operations_on_machine);
    }
}

int Instance::target_function( int index )
{
    int sum_of_end_time = 0;
    for( int i = 0; i < Constance::n_tasks + Constance::n_maintenance; i++ )
    {
        if( !solutions[index].get_machine_one()[i]->is_maintenance() ) sum_of_end_time += solutions[index].get_machine_one()[i]->get_start() + solutions[index].get_machine_one()[i]->get_duration();
        if( !solutions[index].get_machine_two()[i]->is_maintenance() ) sum_of_end_time += solutions[index].get_machine_two()[i]->get_start() + solutions[index].get_machine_two()[i]->get_duration();
    }
    return sum_of_end_time;
}

void Instance::selection( int time )
{
    int solution_index, chosen_solutions_amount = 0, proportion ;
    Solutions temp;

    if( time < 1 ) proportion = 0.3 * (Constance::n_solutions );
    else if( time < 2 ) proportion = 0.4 * (Constance::n_solutions );
    else if( time < 3 ) proportion = 0.5 * (Constance::n_solutions );
    else if( time < 4 ) proportion = 0.6 * (Constance::n_solutions );
    else if( time < 5 ) proportion = 0.7 * (Constance::n_solutions );
    else proportion = 0.8 * ( Constance::n_solutions );

    //ruletka
    while( proportion < Constance::n_solutions)
    {
        solution_index = rand() % ( Constance::crossed_solutions + Constance::n_solutions - chosen_solutions_amount ) + chosen_solutions_amount;
        temp = solutions[solution_index];
        solutions[solution_index] = solutions[chosen_solutions_amount];
        solutions[chosen_solutions_amount] = temp;
        chosen_solutions_amount++;
        proportion++;
    }
    //turniej
    insertion_sort_solutions( chosen_solutions_amount );
	solutions_number = Constance::n_solutions;
	//for (int i = 0; i < Constance::n_solutions; i++)
		//cout << target_function(i) << endl;
	//cout << endl;
}

void Instance::crossing()
{
	//losowanie rozwi¹zañ

	int solution1_index = rand() % Constance::n_solutions;

	int solution2_index = rand() % Constance::n_solutions;
	while (solution2_index == solution1_index)
		solution2_index = rand() % Constance::n_solutions;

	//wstawianie maintenance
	Solutions *solution1, *solution2;
	solution1 = new Solutions;
	solution2 = new Solutions;
	int dupa = 0;

	for (int i = 0; i < Constance::n_maintenance; i++)
	{
		solution1->insert_operation(1, &maintenance[i],dupa,maintenance[i].get_start());
		solution1->insert_operation(2, &maintenance[Constance::n_maintenance + i], dupa, maintenance[Constance::n_maintenance + i].get_start());
		solution2->insert_operation(1, &maintenance[i],dupa,maintenance[i].get_start());
		solution2->insert_operation(2, &maintenance[Constance::n_maintenance + i], dupa, maintenance[Constance::n_maintenance + i].get_start());
	}
	//wstawianie operacji
	//pierwsze wylosowane do drugiego tworzonego, maszyna 1
	int i = 0;
	int i1 = 0;
	int last_operation_end = 0;
	int index = 0;
	while (i < Constance::n_tasks/2)
	{
		if (solutions[solution1_index].get_machine_one()[i1]->is_maintenance())
			;
		else
		{
			solution2->insert_operation(1, solutions[solution1_index].get_machine_one()[i1],last_operation_end, max(last_operation_end, tasks[solutions[solution1_index].get_machine_one()[i1]->get_task_index()].get_ready_time()));
			i++;
		}
		i1++;
	}
	i1 = 0;
	for (int i = 0; i < (Constance::n_tasks + Constance::n_maintenance); i++)
	{
		if (!solutions[solution2_index].get_machine_one()[i]->is_maintenance())
		{
			i1 = 0;
			while (i1 < solution2->get_machine_one_operations_number()
				&& (solutions[solution2_index].get_machine_one()[i]->get_task_index() != solution2->get_machine_one()[i1]->get_task_index() || solution2->get_machine_one()[i1]->is_maintenance()))
			{
				i1++;
			}
			if (i1 == solution2->get_machine_one_operations_number())
			{
				solution2->insert_operation(1, solutions[solution2_index].get_machine_one()[i],last_operation_end, max(last_operation_end, tasks[solutions[solution2_index].get_machine_one()[i]->get_task_index()].get_ready_time()));
			}
		}
	}
	//pierwsze wylosowane do drugiego tworzonego, maszyna 2
	i1 = i = 0;
	last_operation_end = 0;
	while (i < Constance::n_tasks / 2)
	{
		if (solutions[solution1_index].get_machine_two()[i1]->is_maintenance())
			;
		else
		{
			while (solution2->get_machine_one()[index]->get_task_index() != solutions[solution1_index].get_machine_two()[i1]->get_task_index() || solution2->get_machine_one()[index]->is_maintenance())
				index++;
			last_operation_end = max(last_operation_end, solution2->get_machine_one()[index]->get_start() + solution2->get_machine_one()[index]->get_duration());
			index = 0;
			solution2->insert_operation(2, solutions[solution1_index].get_machine_two()[i1],last_operation_end, max(last_operation_end, tasks[solutions[solution1_index].get_machine_two()[i1]->get_task_index()].get_ready_time()));
			i++;
		}
		i1++;
	}

	i1 = 0;
	for (int i = 0; i < (Constance::n_tasks + Constance::n_maintenance); i++)
	{
		if (!solutions[solution2_index].get_machine_two()[i]->is_maintenance())
		{
			i1 = 0;
			while (i1 < solution2->get_machine_two_operations_number()
				&& (solutions[solution2_index].get_machine_two()[i]->get_task_index() != solution2->get_machine_two()[i1]->get_task_index() || solution2->get_machine_two()[i1]->is_maintenance()))
			{
				i1++;
			}
			if (i1 == solution2->get_machine_two_operations_number())
			{
				while (solution2->get_machine_one()[index]->get_task_index() != solutions[solution2_index].get_machine_two()[i]->get_task_index() || solution2->get_machine_one()[index]->is_maintenance())
					index++;
				last_operation_end = max(last_operation_end, solution2->get_machine_one()[index]->get_start() + solution2->get_machine_one()[index]->get_duration());
				index = 0;
				solution2->insert_operation(2, solutions[solution2_index].get_machine_two()[i],last_operation_end, max(last_operation_end, tasks[solutions[solution2_index].get_machine_two()[i]->get_task_index()].get_ready_time()));
			}
		}
	}
	//drugie wylosowane do pierwszego tworzonego, maszyna 1
	i1 = i = 0;
	last_operation_end = 0;
	while (i < Constance::n_tasks / 2)
	{
		if (solutions[solution2_index].get_machine_one()[i1]->is_maintenance())
			;
		else
		{
			solution1->insert_operation(1, solutions[solution2_index].get_machine_one()[i1],last_operation_end, max(last_operation_end, tasks[solutions[solution2_index].get_machine_one()[i1]->get_task_index()].get_ready_time()));
			i++;
		}
		i1++;
	}

	i1 = 0;
	for (int i = 0; i < (Constance::n_tasks + Constance::n_maintenance); i++)
	{
		if (!solutions[solution1_index].get_machine_one()[i]->is_maintenance())
		{
			i1 = 0;
			while (i1 < solution1->get_machine_one_operations_number()
				&& (solutions[solution1_index].get_machine_one()[i]->get_task_index() != solution1->get_machine_one()[i1]->get_task_index() || solution1->get_machine_one()[i1]->is_maintenance()))
			{
				i1++;
			}
			if (i1 == solution1->get_machine_one_operations_number())
			{
				solution1->insert_operation(1, solutions[solution1_index].get_machine_one()[i], last_operation_end, max(last_operation_end, tasks[solutions[solution1_index].get_machine_one()[i]->get_task_index()].get_ready_time()));
			}
		}
	}
	//drugie wylosowane do pierwszego tworzonego, maszyna 2
	i1 = i = 0;
	last_operation_end = 0;
	while (i < Constance::n_tasks / 2)
	{
		if (solutions[solution2_index].get_machine_two()[i1]->is_maintenance())
			;
		else
		{
			while (solution1->get_machine_one()[index]->get_task_index() != solutions[solution2_index].get_machine_two()[i1]->get_task_index() || solution1->get_machine_one()[index]->is_maintenance())
				index++;
			last_operation_end = max(last_operation_end, solution1->get_machine_one()[index]->get_start() + solution1->get_machine_one()[index]->get_duration());
			index = 0;
			solution1->insert_operation(2, solutions[solution2_index].get_machine_two()[i1], last_operation_end, max(last_operation_end, tasks[solutions[solution2_index].get_machine_two()[i1]->get_task_index()].get_ready_time()));
			i++;
		}
		i1++;
	}

	i1 = 0;
	for (int i = 0; i < (Constance::n_tasks + Constance::n_maintenance); i++)
	{
		if (!solutions[solution1_index].get_machine_two()[i]->is_maintenance())
		{
			i1 = 0;
			while (i1 < solution1->get_machine_two_operations_number()
				&& (solutions[solution1_index].get_machine_two()[i]->get_task_index() != solution1->get_machine_two()[i1]->get_task_index() || solution1->get_machine_two()[i1]->is_maintenance()))
			{
				i1++;
			}
			if (i1 == solution1->get_machine_two_operations_number())
			{
				while (solution1->get_machine_one()[index]->get_task_index() != solutions[solution1_index].get_machine_two()[i]->get_task_index() || solution1->get_machine_one()[index]->is_maintenance())
					index++;
				last_operation_end = max(last_operation_end, solution1->get_machine_one()[index]->get_start() + solution1->get_machine_one()[index]->get_duration());
				index = 0;
				solution1->insert_operation(2, solutions[solution1_index].get_machine_two()[i], last_operation_end, max(last_operation_end, tasks[solutions[solution1_index].get_machine_two()[i]->get_task_index()].get_ready_time()));
			}
		}
	}
	solutions[solutions_number] = *solution1;
	solutions_number++;
	solutions[solutions_number] = *solution2;
	solutions_number++;
	//cout << "solutions number=" << solutions_number << endl;
}

void Instance::mutation(int time)
{
    int task_index_first = rand() % ( Constance::n_tasks + Constance::n_maintenance ),
        task_index_second = rand() % ( Constance::n_tasks + Constance::n_maintenance ),
        solution_index = 0;
    bool swapped = false;
    Operation* temp;
	int proportion;
	if (time < 1) proportion = 0.5 * (Constance::n_solutions);
	else if (time < 2) proportion = 0.4 * (Constance::n_solutions);
	else if (time < 3) proportion = 0.3 * (Constance::n_solutions);
	else if (time < 4) proportion = 0.2 * (Constance::n_solutions);
	else if (time < 5) proportion = 0.1 * (Constance::n_solutions);
	else proportion = 0 * (Constance::n_solutions);
    //maszyna 1
    while( solution_index < proportion)
    {
        while( task_index_first >= task_index_second ||
               solutions[solution_index].get_machine_one()[task_index_first]->is_maintenance() ||
               solutions[solution_index].get_machine_one()[task_index_second]->is_maintenance() )
        {
            task_index_first = rand() % ( Constance::n_tasks + Constance::n_maintenance );
            task_index_second = rand() % ( Constance::n_tasks + Constance::n_maintenance );
        }

        if( task_index_first != 0 && task_index_second != Constance::n_solutions - 1
            &&
            solutions[solution_index].get_machine_one()[task_index_first + 1]->get_start() - ( solutions[solution_index].get_machine_one()[task_index_first - 1]->get_start() + solutions[solution_index].get_machine_one()[task_index_first - 1]->get_duration() )
            >= solutions[solution_index].get_machine_one()[task_index_second]->get_duration()
            &&
            tasks[ solutions[solution_index].get_machine_one()[task_index_second]->get_task_index() ].get_ready_time() <= solutions[solution_index].get_machine_one()[task_index_first]->get_start()
            &&
            solutions[solution_index].get_machine_one()[task_index_second + 1]->get_start() - ( solutions[solution_index].get_machine_one()[task_index_second - 1]->get_start() + solutions[solution_index].get_machine_one()[task_index_second - 1]->get_duration() )
            >= solutions[solution_index].get_machine_one()[task_index_first]->get_duration() )
        {
            solutions[solution_index].get_machine_one()[task_index_first]->set_start( solutions[solution_index].get_machine_one()[task_index_second]->get_start() );
            solutions[solution_index].get_machine_one()[task_index_second]->set_start( solutions[solution_index].get_machine_one()[task_index_first - 1]->get_start() + solutions[solution_index].get_machine_one()[task_index_first - 1]->get_duration() );
            temp = solutions[solution_index].get_machine_one()[task_index_first];
            solutions[solution_index].get_machine_one()[task_index_first] = solutions[solution_index].get_machine_one()[task_index_second];
            solutions[solution_index].get_machine_one()[task_index_second] = temp;
            swapped = true;
        }
        else if( task_index_first == 0 && task_index_second != Constance::n_solutions - 1
                 &&
                 solutions[solution_index].get_machine_one()[task_index_first + 1]->get_start() - tasks[ solutions[solution_index].get_machine_one()[task_index_second]->get_task_index() ].get_ready_time()
                 >= solutions[solution_index].get_machine_one()[task_index_second]->get_duration()
                 &&
                 tasks[ solutions[solution_index].get_machine_one()[task_index_second]->get_task_index() ].get_ready_time() <= solutions[solution_index].get_machine_one()[task_index_first]->get_start()
                 &&
                 solutions[solution_index].get_machine_one()[task_index_second + 1]->get_start() - ( solutions[solution_index].get_machine_one()[task_index_second - 1]->get_start() + solutions[solution_index].get_machine_one()[task_index_second - 1]->get_duration() )
                 >= solutions[solution_index].get_machine_one()[task_index_first]->get_duration() )
        {
            solutions[solution_index].get_machine_one()[task_index_first]->set_start( solutions[solution_index].get_machine_one()[task_index_second]->get_start() );
            solutions[solution_index].get_machine_one()[task_index_second]->set_start( tasks[ solutions[solution_index].get_machine_one()[task_index_second]->get_task_index() ].get_ready_time() );
            temp = solutions[solution_index].get_machine_one()[task_index_first];
            solutions[solution_index].get_machine_one()[task_index_first] = solutions[solution_index].get_machine_one()[task_index_second];
            solutions[solution_index].get_machine_one()[task_index_second] = temp;
            swapped = true;
        }
        else if( task_index_second == Constance::n_solutions - 1 && task_index_first == 0
                 &&
                 solutions[solution_index].get_machine_one()[task_index_first + 1]->get_start() - tasks[ solutions[solution_index].get_machine_one()[task_index_second]->get_task_index() ].get_ready_time()
                 >= solutions[solution_index].get_machine_one()[task_index_second]->get_duration()
                 &&
                 tasks[ solutions[solution_index].get_machine_one()[task_index_second]->get_task_index() ].get_ready_time() <= solutions[solution_index].get_machine_one()[task_index_first]->get_start() )
        {
            solutions[solution_index].get_machine_one()[task_index_first]->set_start( solutions[solution_index].get_machine_one()[task_index_second]->get_start() );
            solutions[solution_index].get_machine_one()[task_index_second]->set_start( tasks[ solutions[solution_index].get_machine_one()[task_index_second]->get_task_index() ].get_ready_time() );
            temp = solutions[solution_index].get_machine_one()[task_index_first];
            solutions[solution_index].get_machine_one()[task_index_first] = solutions[solution_index].get_machine_one()[task_index_second];
            solutions[solution_index].get_machine_one()[task_index_second] = temp;
            swapped = true;
        }

        if( swapped )
        {
            //znalezienie 2 operacji o indeksie rownym 1 operacji
			int index_on_machine_two_like_on_one = 0;
            for( int ind = 0; ind < Constance::n_tasks + Constance::n_maintenance; ind++ )
            {
                if( solutions[solution_index].get_machine_one()[task_index_second]->get_task_index() == solutions[solution_index].get_machine_two()[ind]->get_task_index() )
                {
                    index_on_machine_two_like_on_one = ind;
                    break;
                }
            }

            int index_on_machine = 0; //szukanie przerw na 2 maszynie
            //korekcja na drugiej maszynie
            while( index_on_machine < Constance::n_tasks + Constance::n_maintenance )
            {
                if( index_on_machine == 0 )
                {//zmiana ponizszego warunku
                    if( solutions[solution_index].get_machine_one()[task_index_second]->get_start() + solutions[solution_index].get_machine_one()[task_index_second]->get_duration() + solutions[solution_index].get_machine_two()[index_on_machine_two_like_on_one]->get_duration() < solutions[solution_index].get_machine_two()[index_on_machine]->get_start() )
                    {
                        solutions[solution_index].get_machine_two()[index_on_machine_two_like_on_one]->set_start( solutions[solution_index].get_machine_one()[task_index_second]->get_start() + solutions[solution_index].get_machine_one()[task_index_second]->get_duration() );
                        break;
                    }
                }
                else if( index_on_machine == Constance::n_tasks + Constance::n_maintenance - 1 )
                {
                    if( solutions[solution_index].get_machine_one()[task_index_second]->get_start() + solutions[solution_index].get_machine_one()[task_index_second]->get_duration() >= solutions[solution_index].get_machine_two()[index_on_machine]->get_start() + solutions[solution_index].get_machine_two()[index_on_machine]->get_duration() )
                        solutions[solution_index].get_machine_two()[index_on_machine_two_like_on_one]->set_start( solutions[solution_index].get_machine_one()[task_index_second]->get_start() + solutions[solution_index].get_machine_one()[task_index_second]->get_duration() );
                    else
                        solutions[solution_index].get_machine_two()[index_on_machine_two_like_on_one]->set_start( solutions[solution_index].get_machine_two()[index_on_machine]->get_start() + solutions[solution_index].get_machine_two()[index_on_machine]->get_duration() );
                    break;
                }
                else
                {
                    if( (solutions[solution_index].get_machine_one()[task_index_second]->get_start() + solutions[solution_index].get_machine_one()[task_index_second]->get_duration() <= solutions[solution_index].get_machine_two()[index_on_machine]->get_start() + solutions[solution_index].get_machine_two()[index_on_machine]->get_duration())
                        &&
                        solutions[solution_index].get_machine_two()[index_on_machine]->get_start() + solutions[solution_index].get_machine_two()[index_on_machine]->get_duration() + solutions[solution_index].get_machine_two()[index_on_machine_two_like_on_one]->get_duration() < solutions[solution_index].get_machine_two()[index_on_machine + 1]->get_start())
                    {
                        solutions[solution_index].get_machine_two()[index_on_machine_two_like_on_one]->set_start( solutions[solution_index].get_machine_two()[index_on_machine]->get_start() + solutions[solution_index].get_machine_two()[index_on_machine]->get_duration() );
                        break;
                    }
                }
                index_on_machine++;
            }

            insertion_sort_machine_two(solution_index, Constance::n_tasks + Constance::n_maintenance );
            solution_index++;
            swapped = false;
        }
        else
        {
            task_index_first = 1;
            task_index_second = 0; //first wiekszy od second wiec while polosuje nowe
        }
    }
}

void Instance::save_best_solution()
{
	int best_time, idle_index_m1 = 1, maintenance_index_m1 = 1, idle_index_m2 = 1, maintenance_index_m2 = 1, all_idle_duration_m1 = 0,
		all_idle_duration_m2 = 0;
	ofstream file;

		file.open("../../Solutions/Solutionbest.txt");
		int min = target_function(0);
		int solution_number = 0;
		for (int i = 0; i < Constance::n_solutions;i++)
		if (target_function(i)<min)
		{
			min = target_function(i);
			solution_number = i;
		}

		if (file.good())
		{
			best_time = target_function(solution_number);

			file << best_time << "," <<
				begin_time << endl;
			file << "M1: ";
			for (int i = 0; i < Constance::n_tasks + Constance::n_maintenance; i++)
			{
				if (i == 0 && solutions[solution_number].get_machine_one()[i]->get_start() != 0)
				{
					file << "idle" << idle_index_m1 << "_" << "M1,0," << solutions[solution_number].get_machine_one()[i]->get_start() << ",";
					idle_index_m1++;
					all_idle_duration_m1 += solutions[solution_number].get_machine_one()[i]->get_start();
				}
				else if (i != 0 && solutions[solution_number].get_machine_one()[i - 1]->get_start() + solutions[solution_number].get_machine_one()[i - 1]->get_duration() !=
					solutions[solution_number].get_machine_one()[i]->get_start())
				{
					file << "idle" << idle_index_m1 << "_" << "M1," <<
						solutions[solution_number].get_machine_one()[i - 1]->get_start() + solutions[solution_number].get_machine_one()[i - 1]->get_duration() <<
						"," << solutions[solution_number].get_machine_one()[i]->get_start() -
						(solutions[solution_number].get_machine_one()[i - 1]->get_start() + solutions[solution_number].get_machine_one()[i - 1]->get_duration()) << ",";
					idle_index_m1++;
					all_idle_duration_m1 += solutions[solution_number].get_machine_one()[i]->get_start() -
						(solutions[solution_number].get_machine_one()[i - 1]->get_start() + solutions[solution_number].get_machine_one()[i - 1]->get_duration());
				}
				if (solutions[solution_number].get_machine_one()[i]->is_maintenance())
				{
					file << "maint" << maintenance_index_m1 << "_M1," <<
						solutions[solution_number].get_machine_one()[i]->get_start() <<
						"," << solutions[solution_number].get_machine_one()[i]->get_duration() << ",";
					maintenance_index_m1++;
				}
				else    file << "op1_" << solutions[solution_number].get_machine_one()[i]->get_task_index() <<
					"," << solutions[solution_number].get_machine_one()[i]->get_start() <<
					"," << solutions[solution_number].get_machine_one()[i]->get_duration() << ",";
			}
			file << endl << "M2: ";
			for (int i = 0; i < Constance::n_tasks + Constance::n_maintenance; i++)
			{
				if (i == 0 && solutions[solution_number].get_machine_two()[i]->get_start() != 0)
				{
					file << "idle" << idle_index_m2 << "_" << "M2,0," << solutions[solution_number].get_machine_two()[i]->get_start() << ",";
					idle_index_m2++;
					all_idle_duration_m2 += solutions[solution_number].get_machine_two()[i]->get_start();
				}
				else if ( i != 0 && solutions[solution_number].get_machine_two()[i - 1]->get_start() + solutions[solution_number].get_machine_two()[i - 1]->get_duration() !=
					solutions[solution_number].get_machine_two()[i]->get_start())
				{
					file << "idle" << idle_index_m2 << "_" << "M2," <<
						solutions[solution_number].get_machine_two()[i - 1]->get_start() + solutions[solution_number].get_machine_two()[i - 1]->get_duration() <<
						"," << solutions[solution_number].get_machine_two()[i]->get_start() -
						(solutions[solution_number].get_machine_two()[i - 1]->get_start() + solutions[solution_number].get_machine_two()[i - 1]->get_duration()) << ",";
					idle_index_m2++;
					all_idle_duration_m2 += solutions[solution_number].get_machine_two()[i]->get_start() -
						(solutions[solution_number].get_machine_two()[i - 1]->get_start() + solutions[solution_number].get_machine_two()[i - 1]->get_duration());
				}
				if (solutions[solution_number].get_machine_two()[i]->is_maintenance())
				{
					file << "maint" << maintenance_index_m2 << "_M2," <<
						solutions[solution_number].get_machine_two()[i]->get_start() <<
						"," << solutions[solution_number].get_machine_two()[i]->get_duration() << ",";
					maintenance_index_m2++;
				}
				else    file << "op2_" << solutions[solution_number].get_machine_two()[i]->get_task_index() <<
					"," << solutions[solution_number].get_machine_two()[i]->get_start() <<
					"," << solutions[solution_number].get_machine_two()[i]->get_duration() << ",";
			}
			file << endl << "M1_maint: " << maintenance_index_m1 << "," << maintenance_index_m1 * Constance::duration_maintenance << endl <<
				"M2_maint: " << maintenance_index_m2 << "," << maintenance_index_m2 * Constance::duration_maintenance << endl <<
				"M1_idle: " << idle_index_m1 << "," << all_idle_duration_m1 << endl <<
				"M2_idle: " << idle_index_m2 << "," << all_idle_duration_m2 << endl;
		}
		else cout << "Blad otwarcia pliku" << endl;
		file.close();
}

void Instance::save_best_time()
{
	string file_name;
	ofstream file;
	file_name = "../Results.txt";   //nazwa pliku (scie¿ka)
	file.open(file_name.c_str(),ios_base::app);
	int min = target_function(0);
	for (int i = 0; i < Constance::n_solutions; i++)
	if (target_function(i)<min)
	{
		min = target_function(i);
	}
	file << min << endl;
	cout << "min=" << min << endl;
	file.close();
}

void Instance::save_begin_time()
{
	string file_name;
	ofstream file;
	file_name = "../Begin.txt";   //nazwa pliku (scie¿ka)
	file.open(file_name.c_str(),ios_base::app);
	int max = target_function(0);
	for (int i = 0; i < Constance::n_solutions; i++)
	if (target_function(i) > max)
	{
		max = target_function(i);
	}
	set_begin_time(max);
	file << max << endl;
	file.close();
}
