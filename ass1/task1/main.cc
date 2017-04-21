#include "eventandstate.h"

#include <iostream>
#include <fstream>

using namespace std;

int main() {
	cout << "Running task 1 " << endl;
	ofstream f_res("res_task1.txt");

	vector<double> dtQ1 = {0.1, 1, 2, 5, 20};
	//vector<double> dtQ1 = {1, 2, 5};

	for(auto& dt : dtQ1) {
		EventList el;
		int seed = 0;
		default_random_engine rnd_engine(seed);


		//Kick-start:
		el.InsertEvent(Event::ArrivalQ1, dt);
		el.InsertEvent(Event::Measure, get_exp_time(rnd_engine, 5));

		State s(rnd_engine, dt);

		while(s.nbr_measurements < 10000) {
			cout << "LQ1 = " << s.LQ1 << endl;
			if(s.LQ1 > 10) {
				exit(1);
			}
			s.ProcessEvent(el);
			auto it_temp = el.event_list.erase(el.event_list.begin());
			for(auto& e : el.event_list) cout << e << endl;
		}

		s.Write("task1_"+to_string(dt)+".root");

		f_res << "Results for dtQ1 = " << dt << endl;
		f_res << "mean(LQ2) = " << calc_mean(s.v_LQ2) << endl;
		f_res << "stddev(LQ2) = " << calc_stddev(s.v_LQ2) << endl;
		f_res << "mean(LQ1) = " << calc_mean(s.v_LQ1) << endl;
		f_res << "stddev(LQ1) = " << calc_stddev(s.v_LQ1) << endl;
		f_res << "mean(rejected ratio) = " << calc_mean(s.v_rej_ratio) << endl;
		f_res << "stddev(rejected ratio) = " << calc_stddev(s.v_rej_ratio) << endl;
		f_res << endl;
	}



}
