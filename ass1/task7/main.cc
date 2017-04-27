#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

struct Component {
	int nbr;
	double t_end;
};

double get_uni_time(default_random_engine& rnd) {
	uniform_real_distribution<double> dist_uni(1, 5);
	double rand = dist_uni(rnd);
	return rand;
}

void remove_comp(vector<Component>& sys, int nbr) {
	auto it = find_if(sys.begin(), sys.end(),
			[nbr](const Component& c) {
				return c.nbr == nbr;
				}
			);
	if(it == sys.end()) return;
	sys.erase(it);
}


ostream& operator<<(ostream& os, Component& c) {
	os << "Component: " << c.nbr << ", " << c.t_end;
	return os;
}

double calc_mean(vector<double>& v) {
	double sum = accumulate(v.begin(), v.end(), 0.0);
	return sum/v.size();
}

double calc_stddev(vector<double>& v) {
	double mean = calc_mean(v);
	vector<double> diff(v.size());
	transform(v.begin(), v.end(), diff.begin(), [mean](double x) { return x - mean; });
	double sq_sum = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	double stdev = sqrt(sq_sum / v.size());
	return stdev;
}

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
	cout << "Running task 7 " << endl;
	ofstream f_res("res_task7.txt");

	vector<Component> sys;

	vector<double> v_life;

	int seed = 0;
	default_random_engine rnd(seed);

	int runs = 1000;

	for(int i = 0; i != runs; ++i) {
		for(int j = 1; j != 6; ++j) {
			sys.push_back({j, get_uni_time(rnd)});
		}

		sort(sys.begin(),sys.end(),
				[](const Component& a, const Component& b) {
				return a.t_end < b.t_end;
				}
				);

		for(auto& c : sys) cout << c << endl;

		double last_time;
		while(sys.size() != 0) {
			cout << " ¤ " << endl;
			Component c = sys[0];
			last_time = c.t_end;
			remove_comp(sys, c.nbr);
			if(c.nbr == 1) {
				remove_comp(sys, 2);
				remove_comp(sys, 5);
			}
			else if(c.nbr == 3) {
				remove_comp(sys, 3);
				remove_comp(sys, 4);
			}
		for(auto& c : sys) cout << c << endl;
		}

		cout << "Life time = " << last_time << endl;
		v_life.push_back(last_time);

	}


	//s.Write("task6.root");

	f_res << "Results for run " << endl;
	stats(f_res, v_life);
	f_res << endl;



}
