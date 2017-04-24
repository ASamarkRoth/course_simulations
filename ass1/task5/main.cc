#include "procs.h"

#include <fstream>

using namespace std;

int main() {
	cout << "Running task 5" << endl;
	ofstream f_res("res_task5.txt");

	vector<double> v_t = {0.11, 0.15, 2.};
	for(auto& time : v_t) {
		for(int i = 0; i!=3;++i) {
			int seed = 100;
			default_random_engine rnd(seed);

			double mean_arrival_time = time;
			double mean_measure_time = 5;
			vector<shared_ptr<Queue>> Qs;
			ProcessList plist;

			for(int j = 1; j != 6; ++j) {
				cout << "Creating process: " << "Q" + to_string(j) << endl;
				Qs.push_back(shared_ptr<Queue>(new Queue("Q"+to_string(j), rnd)));
				plist.AddProcess(Qs[j-1]);
			}

			shared_ptr<Measure> M(new Measure("Measure", rnd, mean_measure_time));
			plist.AddProcess(M);

			shared_ptr<LoadDistr> rnd_load(new RandomLoad(Qs));
			shared_ptr<LoadDistr> robin_load(new RobinLoad(Qs));
			shared_ptr<LoadDistr> opt_load(new OptLoad(Qs));
			vector<shared_ptr<LoadDistr>> loads = {rnd_load, robin_load, opt_load};

			shared_ptr<Generator> G(new Generator("Generator", rnd, mean_arrival_time, loads[i]));
			plist.AddProcess(G);

			G->AddSignal(Signal::Ready, "Generator", get_uni_time(rnd, mean_arrival_time));
			M->AddSignal(Signal::Ready, "Measure", get_exp_time(rnd, mean_measure_time));

			plist.Update();
			cout << plist << endl;

			int j = 0;
			while(M->nbr_measurements < 5*100000) {
				plist.TreatSignal();
				plist.Update();
				cout << plist << endl;
				cout << "LQ = " << Qs[0]->LQ << endl;
				int sum = 0;
				for(int i = 0; i!=5; ++i) sum += Qs[i]->nbr_ready;
				cout << "nbr_ready = " << sum << endl;
				cout << "nbr_arrivals = " << G->nbr_arrivals << endl;
				++j;
			}

			M->Write("task5_"+to_string(mean_arrival_time)+loads[i]->GetName()+".root");


			f_res << "Run with mean arrival time " << mean_arrival_time << " and Load distr " << loads[i]->GetName() << endl;
			f_res << "mean LQ = " << calc_mean(M->v_LQ) << endl;
			f_res << "Little's theorem LQ = " << (1./mean_arrival_time)*0.5 << endl;
			f_res << endl;
		}
	}


}
