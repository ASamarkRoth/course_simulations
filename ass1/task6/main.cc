#include "eventandstate.h"
#include "write.h"

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
	ofstream f_res("res_task6.txt");

	double lambda = 4./60.;
	unsigned int days = 1000;

	vector<double> overtime, mean_time;

	int seed = 0;
	default_random_engine rnd_engine(seed);

	State s(rnd_engine, lambda);

	for(unsigned int j = 0; j != days; ++j) {
		cout << "***********************************" << endl;
		cout << "Day " << j << endl;

		EventList el;

		//Kick-start:
		el.InsertEvent(Event::Arrival, get_exp_time(rnd_engine, 1./lambda));

		double last_time;
		while(el.event_list.size() > 0) {
			last_time = el.event_list[0].eventtime;
			s.ProcessEvent(el);
			auto it_temp = el.event_list.erase(el.event_list.begin());
			cout << "LQ = " << s.NC << endl;
			//cout << "Event list: " << endl;
			//for(auto& e : el.event_list) cout << e << endl;
		}

		overtime.push_back(last_time);

	}

	//s.Write("task6.root");

	f_res << "Results for run " << endl;
	stats(f_res, overtime);
	stats(f_res, s.v_mean_time);
	f_res << endl;
	cout << "##############################################" << endl;



}
