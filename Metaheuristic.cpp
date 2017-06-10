#include<iostream>
#include<ctime>
#include"structure.h"
using namespace std;

int main()
{
	srand(time(NULL));
	Instance instance;
		instance.load_from_file("../../Instances/Instancja1.txt");
		instance.generate_solutions();
		instance.save_begin_time();
		//instance.save_solution_to_file(Constance::n_solutions);
		time_t start = time(NULL);
		int duration = 0;
		while (duration <= 5)
		{
			instance.mutation(duration);
			for (int i = 0; i < 50; i++)
				instance.crossing();
			instance.selection(duration);
			duration = difftime(time(NULL), start);
		}
		cout << "koniec\n";
		instance.save_best_solution();
		instance.save_best_time();
	//cout << "start=" << instance.get_solution(1).get_machine_one()[54]->get_start() << endl;

	//cin.ignore();
	//cin.get();
	return 0;

}
