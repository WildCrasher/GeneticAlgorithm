#include "Solutions.h"
#include <ctime>
#include <cstdlib>


Solutions::Solutions()
{
    for (int i = 0; i < (Constance::n_operations/2 + Constance::n_maintenance); i++ )
    {
        machine_one[i] = nullptr;
        machine_two[i] = nullptr;
    }
	machine_one_operations_number = 0;
	machine_two_operations_number = 0;
}
Solutions::~Solutions()
{
}
Operation** Solutions::get_machine_one()
{
    return machine_one;
}
Operation** Solutions::get_machine_two()
{
    return machine_two;
}

void Solutions::insert_operation(int machine_number, Operation * operation,int & inserted_end_time, int insert_time)
{
	Operation *temp = new Operation(*operation);
	int i = 0;
	if (machine_number == 1)
	{
		if (machine_one_operations_number == 0)
		{
			temp->set_start(insert_time);
		}
		else if (machine_one[0]->get_start() >= (temp->get_duration() + insert_time))
		{
			temp->set_start(insert_time);
		}
		else
		{
			while (i < machine_one_operations_number - 1
				&& (i < Constance::n_tasks + Constance::n_maintenance - 1)
				&& ((machine_one[i + 1]->get_start() - (machine_one[i]->get_start() + machine_one[i]->get_duration())) < temp->get_duration()))
			{
				i++;
			}

			if (machine_one[i]->get_start() + machine_one[i]->get_duration() >= insert_time)
				temp->set_start(machine_one[i]->get_start() + machine_one[i]->get_duration()); //ustawia czas na pierwsz¹ woln¹ przerwê

			else
				temp->set_start(insert_time);
		}
		if (machine_one_operations_number < Constance::n_tasks + Constance::n_maintenance)
		{
			machine_one[machine_one_operations_number] = temp; //wstawianie
			inserted_end_time = temp->get_start() + temp->get_duration();
			machine_one_operations_number++;
			this->insertion_sort_machine_one(machine_one_operations_number);
		}
	}

	else if (machine_number == 2)
	{
		if (machine_two_operations_number == 0)
		{
			temp->set_start(insert_time);
		}
		else if (machine_two[0]->get_start() >= (temp->get_duration() + insert_time))
		{
			temp->set_start(insert_time);
		}
		else
		{
			while (i < machine_two_operations_number - 1
				&& (i < Constance::n_tasks + Constance::n_maintenance - 1)
				&& ((machine_two[i + 1]->get_start() - (machine_two[i]->get_start() + machine_two[i]->get_duration())) < temp->get_duration()))
			{
				i++;
			}

			if (machine_two[i]->get_start() + machine_two[i]->get_duration() >= insert_time)
				temp->set_start(machine_two[i]->get_start() + machine_two[i]->get_duration()); //ustawia czas na pierwsz¹ woln¹ przerwê

			else
				temp->set_start(insert_time);
		}
		if (machine_two_operations_number < Constance::n_tasks + Constance::n_maintenance)
		{
			machine_two[machine_two_operations_number] = temp; //wstawianie
			inserted_end_time = temp->get_start() + temp->get_duration();
			machine_two_operations_number++;
			this->insertion_sort_machine_two(machine_two_operations_number);
		}
	}
}

void Solutions::insertion_sort_machine_one(int operations_on_machine)
{
	int j;
	Operation* pom;
	for (int i = 1; i < operations_on_machine; i++)
	{
		j = i - 1;
		while ((j >= 0) && (this->get_machine_one()[j + 1]->get_start() < this->get_machine_one()[j]->get_start()))
		{
			pom = this->get_machine_one()[j + 1];
			this->get_machine_one()[j + 1] = this->get_machine_one()[j];
			this->get_machine_one()[j] = pom;
			j--;
		}
	}
}

void Solutions::insertion_sort_machine_two(int operations_on_machine)
{
	int j;
	Operation* pom;
	for (int i = 1; i < operations_on_machine; i++)
	{
		j = i - 1;
		while ((j >= 0) && (this->get_machine_two()[j + 1]->get_start() < this->get_machine_two()[j]->get_start()))
		{
			pom = this->get_machine_two()[j + 1];
			this->get_machine_two()[j + 1] = this->get_machine_two()[j];
			this->get_machine_two()[j] = pom;
			j--;
		}
	}
}

void Solutions::show(int machine)
{
	using std::cout;
	using std::endl;
	if (machine == 1)
	{
		for (int i = 0; i < Constance::n_tasks+Constance::n_maintenance; i++)
		{
			if (this->get_machine_one()[i]->is_maintenance())
				cout << "m";
			cout << this->get_machine_one()[i]->get_task_index() << " ";
		}
		cout << endl<<endl;
		for (int i = 0; i < Constance::n_tasks + Constance::n_maintenance; i++)
		{
			if (this->get_machine_one()[i]->is_maintenance())
				cout << "m";
			cout << this->get_machine_one()[i]->get_start() << " ";
		}
		cout << endl << endl;
		for (int i = 0; i < Constance::n_tasks + Constance::n_maintenance; i++)
		{
			if (this->get_machine_one()[i]->is_maintenance())
				cout << "m";
			cout << this->get_machine_one()[i]->get_duration() << " ";
		}
	}
	else if (machine == 2)
	{
		for (int i = 0; i < Constance::n_tasks + Constance::n_maintenance; i++)
		{
			if (this->get_machine_two()[i]->is_maintenance())
				cout << "m";
			cout << this->get_machine_two()[i]->get_task_index() << " ";
		}
		cout << endl << endl;
		for (int i = 0; i < Constance::n_tasks + Constance::n_maintenance; i++)
		{
			if (this->get_machine_two()[i]->is_maintenance())
				cout << "m";
			cout << this->get_machine_two()[i]->get_start() << " ";
		}
		cout << endl << endl;
		for (int i = 0; i < Constance::n_tasks + Constance::n_maintenance; i++)
		{
			if (this->get_machine_two()[i]->is_maintenance())
				cout << "m";
			cout << this->get_machine_two()[i]->get_duration() << " ";
		}
	}
	else
	{
		cout << "Bledny numer maszyny";
	}
	cout << endl;
}
