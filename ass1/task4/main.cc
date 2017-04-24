#include "eventandstate.h"
#include "write.h"

#include <iostream>
#include <fstream>

using namespace std;

int main() {
	cout << "Running task 1 " << endl;
	ofstream f_res("res_task3.txt");

	vector<double> N = {1000, 1000, 1000, 100, 100, 100};
	vector<double> x = {100, 10, 200, 10, 10, 10};
	vector<double> lambda = {8, 80, 4, 4, 4, 4};
	vector<double> M = {1000, 1000, 1000, 1000, 4000, 4000};
	vector<double> T = {1, 1, 1, 4, 1, 4};
	vector<double> M_delay = {0, 0, 0, 10, 10, 10};

	for(unsigned int j = 0; j != x.size(); ++j) {
		EventList el;
		int seed = 0;
		default_random_engine rnd_engine(seed);

		//Kick-start:
		el.InsertEvent(Event::Arrival, get_exp_time(rnd_engine, 1./lambda[j]));
		el.InsertEvent(Event::Measure, M_delay[j] + T[j]);

		State s(rnd_engine, N[j], x[j], lambda[j], M[j], T[j]);

		while(s.nbr_measurements < M[j]) {
			s.ProcessEvent(el);
			auto it_temp = el.event_list.erase(el.event_list.begin());
			//for(auto& e : el.event_list) cout << e << endl;
		}

		s.Write("task4_"+to_string(j)+".root");

		write_txt(s.v_NC, "task4_"+to_string(j)+".txt");

		f_res << "Results for run = " << j << endl;
		f_res << "mean(NC) = " << calc_mean(s.v_NC) << endl;
		f_res << "stddev(NC) = " << calc_stddev(s.v_NC) << endl;
		f_res << endl;
	}

	cout << "##############################################" << endl;



}
