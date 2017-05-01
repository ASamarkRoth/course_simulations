#include "eventandstate.h"

#include <iostream>
#include <fstream>

using namespace std;

void stats(ofstream& ofs, vector<double> v) {
	double mean = calc_mean(v);
	double stddev = calc_stddev(v);
	double stddev_mean = stddev/sqrt(v.size());
	ofs << "mean = " << mean << endl;
	ofs << "stddev of mean estimate = " << stddev_mean << endl;
	ofs << "95% conf int = " << mean - 1.96*stddev_mean << ", " << mean + 1.96*stddev_mean << endl;
	ofs << "90% conf int = " << mean - 1.645*stddev_mean << ", " << mean + 1.645*stddev_mean << endl;
	ofs << "99% conf int = " << mean - 2.576*stddev_mean << ", " << mean + 2.576*stddev_mean << endl;
	cout << endl;
}

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
		el.InsertEvent(Event::Measure, 1000 + get_exp_time(rnd_engine, 5));

		State s(rnd_engine, dt);

		while(s.nbr_measurements < 10000) {
			cout << "LQ1 = " << s.LQ1 << endl;
			s.ProcessEvent(el);
			auto it_temp = el.event_list.erase(el.event_list.begin());
			for(auto& e : el.event_list) cout << e << endl;
		}

#ifdef ROOTYES
		s.Write("task3_"+to_string(dt)+".root");
#endif

		f_res << "Results for dtQ1 = " << dt << " and LQ1 " << endl;
		stats(f_res, s.v_LQ1);
		f_res << "Results for dtQ1 = " << dt << " and mean time " << endl;
		stats(f_res, s.v_mean_time);
		f_res << endl;
	}

	cout << "##############################################" << endl;



}
