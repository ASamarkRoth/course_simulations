#include "eventandstate.h"

#include <iostream>
#include <iterator>
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
	cout << "Running task 2 " << endl;
	ofstream f_res("res_task2.txt");

	vector<bool> is_B = {true, false, true};

	int count = 0;
	for(auto B : is_B) {
		EventList el;
		int seed = 0;
		default_random_engine rnd_engine(seed);


		//Kick-start:
		double t_1st_job = get_exp_time(rnd_engine, 1./150);
		el.InsertEvent(Event::AddJobA, t_1st_job);
		el.InsertEvent(Event::Measure, 0.1);

		State s(rnd_engine, B, count);

		while(s.nbr_measurements < 1000) {
			cout << "NA = " << s.NA << endl;
			//if(s.NA > 2000) exit(1);
			s.ProcessEvent(el);
			auto it_temp = el.event_list.erase(el.event_list.begin());
			/*
			for(auto& e : s.v_Buffer){
				cout << "Buffer: ";
				cout << e << endl;
			}
			*/

			copy(el.event_list.begin(), el.event_list.begin()+3, ostream_iterator<Event>(cout, "\n"));
		}

		if(count == 2) s.Write("task2_"+to_string(B)+"_Exp.root");
		else s.Write("task2_"+to_string(B)+".root");

		f_res << "Results for IsBPriority? = " << B << " and NAB" << endl;
		stats(f_res, s.v_NAB);
		f_res << "Results for IsBPriority? = " << B << " and NA" << endl;
		stats(f_res, s.v_NA);
		f_res << "Results for IsBPriority? = " << B << " and NB" << endl;
		stats(f_res, s.v_NB);

		cout << "####################################################" << endl;

		++count;
	}



}
