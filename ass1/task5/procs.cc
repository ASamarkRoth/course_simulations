#include "procs.h"

#include <iostream>
#include <algorithm>

using namespace std;


Signal::Signal(int type, string dest, double time, int meas) : SignalType(type), ArrivalTime(time), Destination(dest), Meas(meas) { }

ostream& operator<<(ostream& os, Signal s) {
	os << "signal: " << s.SignalType << ", " << s.Destination << ", " << s.ArrivalTime;
	return os;
}

void Process::AddSignal(int type, string dest, double time, int meas) {
	Signal s(type, dest, time, meas);
	if(SignalList.size() == 0) {
		SignalList.push_back(s);
		return;
	}
	auto it = SignalList.begin();
	while(it != SignalList.end() && s.ArrivalTime >= it->ArrivalTime) {
		++it;
	}

	it = SignalList.insert(it, s);
}

void Process::RemoveSignal() {
	if(SignalList.size() != 0) {
		//delete SignalList[0];
		SignalList.erase(SignalList.begin());
	}
	else {
		std::cout << "SignalList empty!!!!" << std::endl;
		exit(0);
	}
}
void ProcessList::AddProcess(shared_ptr<Process> p) {
	procs.push_back(p);
}

void ProcessList::Update() {
	cout << "Updating process list" << endl;
	//Sorting the process list
	sort(procs.begin(), procs.end(),
			[](const shared_ptr<Process> a, const shared_ptr<Process> b) {
			if(a->SignalList.size() == 0) return false;
			else if(b->SignalList.size() == 0) return true;
			else if(a->SignalList.size() == 0 && b->SignalList.size() == 0) return false;
			else return a->SignalList[0].ArrivalTime < b->SignalList[0].ArrivalTime;
			}
			);
}

void ProcessList::TreatSignal() {
	cout << "Treating signal from ProcessList" << endl;
	Signal x = procs[0]->SignalList[0];
	shared_ptr<Process> p = FetchProcess(x);
	p->TreatSignal(x);
	procs[0]->RemoveSignal();
}

shared_ptr<Process> ProcessList::FetchProcess(Signal x) {
	auto it = find_if(procs.begin(), procs.end(),
			[x] (const shared_ptr<Process> p) {
				return x.Destination == p->GetName();
				}
			);
	return (*it);
}

std::ostream& operator<<(std::ostream& os, ProcessList pl) {
	os << "Listing processes: " << endl;
	for(auto p : pl.procs) {
		if(p->SignalList.size() == 0) continue;
		os << p->GetName() << " - " << p->SignalList[0] << endl;
	}
	return os;
}

std::ostream& operator<<(ostream& os, Process* p) {
	for(unsigned int i = 0; i != p->SignalList.size(); ++i) {
		os << i << " " << p->SignalList[i] << endl;
	}
	return os;
}

void Generator::TreatSignal(Signal x) {
	cout << "Treating signal in " << this->GetName() << " - " << x << endl;

	switch(x.SignalType) {
		case Signal::Ready:
			AddSignal(Signal::Arrival, "Q", x.ArrivalTime);
			AddSignal(Signal::Ready, "Generator", x.ArrivalTime + get_uni_time(rnd_engine, t_mean));
			//for(auto& s : SignalList) cout << s << endl;
			break;
	}
}

void Queue::TreatSignal(Signal x) {
	cout << "Treating signal in " << this->GetName() << " - " << x << endl;
	double t_mean = 0.5;
	switch(x.SignalType) {
		case Signal::Ready:
			if(LQ > 1){
				AddSignal(Signal::Ready, this->GetName(), x.ArrivalTime + get_exp_time(rnd_engine, t_mean));
				++nbr_ready;
			}
			--LQ;
			//for(auto& s : SignalList) cout << s << endl;
			break;
		case Signal::Arrival:
			if(LQ == 0) AddSignal(Signal::Ready, this->GetName(), x.ArrivalTime + get_exp_time(rnd_engine, t_mean));
			++LQ;
			break;
		case Signal::Measure:
			AddSignal(Signal::Arrival, "Measure", x.ArrivalTime, LQ);
			break;
	}
}
void Measure::TreatSignal(Signal x) {
	cout << "Treating signal in " << this->GetName() << " - " << x << endl;
	switch(x.SignalType) {
		case Signal::Ready:
			AddSignal(Signal::Ready, this->GetName(), x.ArrivalTime + get_exp_time(rnd_engine, t_mean));
			AddSignal(Signal::Measure, "Q", x.ArrivalTime);
			break;
		case Signal::Arrival:
			++nbr_measurements;
			cout << "Measuring: " << x.Meas << endl;
			v_LQ.push_back(x.Meas);
			v_time.push_back(x.ArrivalTime);
			break;
	}
}

double get_exp_time(default_random_engine& rnd, double mu) {
	exponential_distribution<double> dist_exp(1/mu);
	double rand = dist_exp(rnd);
	return rand;
}

double get_uni_time(default_random_engine& rnd, double mu) {
	uniform_real_distribution<double> dist_uni(0, 2*mu);
	double rand = dist_uni(rnd);
	cout << "rnd = " << rand << endl;
	return rand;
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
