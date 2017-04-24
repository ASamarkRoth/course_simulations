#include "procs.h"

#include <fstream>

using namespace std;

int main() {
	cout << "Running task 5" << endl;
	ofstream f_res("res_task5.txt");

	int seed = 0;
	default_random_engine rnd(seed);

	double mean_arrival_time = 1;
	double mean_measure_time = 5;

	shared_ptr<Generator> G(new Generator("Generator", rnd, mean_arrival_time));
	plist.AddProcess(G);

	vector<shared_ptr<Queue>> Qs;
	for(int j = 1; j != 6; ++j) {
		shared_ptr<Queue> push_back(new Queue("Q"+to_string(j), rnd));
		plist.AddProcess(Qs[j]);
	}

	shared_ptr<Measure> M(new Measure("Measure", rnd, mean_measure_time));
	plist.AddProcess(M);

	G->AddSignal(Signal::Ready, "Generator", get_uni_time(rnd, mean_arrival_time));
	M->AddSignal(Signal::Ready, "Measure", get_exp_time(rnd, mean_measure_time));

	ProcessList plist;


	plist.Update();
	cout << plist << endl;

	int j = 0;
	while(M->nbr_measurements < 1000) {
		plist.TreatSignal();
		plist.Update();
		cout << plist << endl;
		cout << "LQ = " << Q->LQ << endl;
		cout << "nbr_ready = " << Q->nbr_ready << endl;
		++j;
	}



	f_res << "Run with mean arrival time " << mean_arrival_time << endl;
	f_res << "mean LQ = " << calc_mean(M->v_LQ) << endl;
	f_res << "Little's theorem LQ = " << (1/mean_arrival_time)*0.5 << endl;


}
