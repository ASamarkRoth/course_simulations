#include "procs.h"
#include "write.h"

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
	//cout/gc << "Running task 5" << endl;
	ofstream f_res("res_task5.txt");

	vector<double> v_t = {0.11, 0.15, 2.};
	//vector<double> v_t = {0.30};
	for(auto& m_time : v_t) {
		for(int i = 0; i!=3;++i) {
			int seed = 1;
			default_random_engine rnd(seed);

			double mean_arrival_time = m_time;
			double mean_measure_time = 5;
			vector<shared_ptr<Queue>> Qs;
			ProcessList plist;

			for(int j = 1; j != 6; ++j) {
				//cout/gc << "Creating process: " << "Q" + to_string(j) << endl;
				Qs.push_back(shared_ptr<Queue>(new Queue("Q"+to_string(j), rnd)));
				plist.AddProcess(Qs[j-1]);
			}

			shared_ptr<Measure> M(new Measure("Measure", rnd, mean_measure_time));
			plist.AddProcess(M);

			shared_ptr<LoadDistr> rnd_load(new RandomLoad(Qs));
			shared_ptr<LoadDistr> robin_load(new RobinLoad(Qs));
			shared_ptr<LoadDistr> opt_load(new SmallestQueLoad(Qs));
			vector<shared_ptr<LoadDistr>> loads = {rnd_load, robin_load, opt_load};

			shared_ptr<Generator> G(new Generator("Generator", rnd, mean_arrival_time, loads[i]));
			plist.AddProcess(G);

			G->AddSignal(Signal::Ready, "Generator", get_uni_time(rnd, mean_arrival_time));
			M->AddSignal(Signal::Ready, "Measure", get_exp_time(rnd, mean_measure_time));

			plist.Update();
			//cout/gc << plist << endl;

			int j = 0;
			while(plist.procs[0]->SignalList[0].ArrivalTime < 100000) {
			//while(M->nbr_measurements < 2) {
				plist.TreatSignal();
				//cout << "Before sorting ... " << endl;
				//cout << plist << endl;
				plist.Update();
				//cout << plist;
				//cout << "LQ = " << Qs[0]->LQ << endl;
				int sum = 0;
				for(int i = 0; i!=5; ++i) sum += Qs[i]->nbr_ready;
				//cout/gc << "nbr_ready = " << sum << endl;
				//cout/gc << "nbr_arrivals = " << G->nbr_arrivals << endl;
				++j;
				//cout << ">>>>>>>>>> " << endl;
			}

#ifdef ROOTSYS
			M->Write("task5_"+to_string(mean_arrival_time)+loads[i]->GetName()+".root");
#endif

			write_txt(M->v_LQ, "task5_"+to_string(mean_arrival_time)+loads[i]->GetName()+".txt");

			f_res << "Run with mean arrival time " << mean_arrival_time << " and Load distr " << loads[i]->GetName() << endl;
			stats(f_res, M->v_LQ);
			f_res << "Little's theorem LQ = " << (1./mean_arrival_time)*0.5 << endl;
			f_res << endl;
		}
	}


}
