#include "eventandstate.h"

#include <iostream>
#include <fstream>

using namespace std;

int main() {
	cout << "Running task 1 " << endl;
	ofstream f_res("res_task3.txt");

	vector<double> ArrTime = {2., 1.5, 1.1};

	for(auto& dt : ArrTime) {
		EventList el;
		int seed = 0;
		default_random_engine rnd_engine(seed);


		//Kick-start:
		el.InsertEvent(Event::ArrivalQ1, get_exp_time(rnd_engine, dt));
		el.InsertEvent(Event::Measure, get_exp_time(rnd_engine, 5));

		State s(rnd_engine, dt);

		while(s.nbr_measurements < 10000) {
			cout << "LQ1 = " << s.LQ1 << endl;
			s.ProcessEvent(el);
			auto it_temp = el.event_list.erase(el.event_list.begin());
			for(auto& e : el.event_list) cout << e << endl;
		}

		s.Write("task3_"+to_string(dt)+".root");

		f_res << "Results for dtQ1 = " << dt << endl;
		f_res << "mean(LQ2) = " << calc_mean(s.v_LQ2) << endl;
		f_res << "stddev(LQ2) = " << calc_stddev(s.v_LQ2) << endl;
		f_res << "mean(LQ1) = " << calc_mean(s.v_LQ1) << endl;
		f_res << "stddev(LQ1) = " << calc_stddev(s.v_LQ1) << endl;
		f_res << "mean(time) = " << calc_mean(s.v_mean_time) << endl;
		f_res << "stddev(time) = " << calc_stddev(s.v_mean_time) << endl;
		f_res << endl;
	}

	cout << "##############################################" << endl;



}
