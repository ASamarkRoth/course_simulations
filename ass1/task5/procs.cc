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
	cout << "About to invoke TreatSignal!" << endl;
	if(!p) cout << "No process with that name" << endl;
	p->TreatSignal(x);
	procs[0]->RemoveSignal();
}

shared_ptr<Process> ProcessList::FetchProcess(Signal x) {
	cout << "Fetching process with name: " << x.Destination << endl;
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
			++nbr_arrivals;
			AddSignal(Signal::Arrival, "Q" + to_string(this->Load->GetQ(rnd_engine)), x.ArrivalTime);
			//AddSignal(Signal::Arrival, "Q" + to_string(1), x.ArrivalTime);
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
			for(int j = 1; j != 6; ++j) {
				AddSignal(Signal::Measure, "Q"+to_string(j), x.ArrivalTime);
			}
			break;
		case Signal::Arrival:
			if(measured < 4) {
				LQ += x.Meas;
				++measured;
			}
			else {
				LQ += x.Meas;
				++measured;
				++nbr_measurements;
				cout << "Measuring: " << LQ << endl;
				v_LQ.push_back(LQ/1);
				v_time.push_back(x.ArrivalTime);
				measured = LQ = 0;
			}
			break;
	}
}

#include "TString.h"
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TColor.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLatex.h"

void Measure::Write(string s) {
	cout << "Writing to file task4.root" << endl;
	TFile* f_out = new TFile(TString(s), "RECREATE");

	TGraph* g;
  TCanvas* C = new TCanvas();

  //double marker_size[runs] = {1, 2, 1, 2, 2};
  //double marker_style[runs] = {21, 21, 22, 22, 20};
  //red, green, blue, cyan, black
  //TColor marker_colour[runs] = {TColor(1,0,0), TColor(0,1,0), TColor(0,0,1), TColor(0,1,1), TColor(0,0,0)};
  TLegend* leg = new TLegend(0.7,0.7,0.55,0.9);
  //leg->SetHeader("Collimator","C"); // option "C" allows to center the header, WHY DON'T WORK?
  //leg->SetHeader("The Legend Title",""); // option "C" allows to center the header
  //TString legend_labels[runs] = {TString("Simple, 1 mm diameter"), TString("Simple, 1.5 mm diameter"),TString("Integrated cones (i)"),TString("Integrated cones (ii)"), TString("Integrated cylinders")};

	TString s_x[2] = {"Time (s)", "Time (s)"};
	TString s_y[4] = {"Number of customers", "Mean customers", "StdDev customers"};

	g = new TGraph(v_LQ.size(), &(v_time[0]), &(v_LQ[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[0]);
	g->GetXaxis()->SetTitle(s_x[0]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();

	g = new TGraph(v_mean.size(), &(v_time[0]), &(v_mean[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[1]);
	g->GetXaxis()->SetTitle(s_x[1]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();

	g = new TGraph(v_var.size(), &(v_time[0]), &(v_var[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[2]);
	g->GetXaxis()->SetTitle(s_x[1]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();


	f_out->Close();


}

unsigned int RandomLoad::GetQ(std::default_random_engine& rnd) {
	cout << "Random Load " << endl;
	uniform_int_distribution<int> dist_uni(1, 5);
	int rand = dist_uni(rnd);
	cout << "rnd = " << rand << endl;
	return rand;
}

unsigned int RobinLoad::GetQ(std::default_random_engine& rnd) {
	cout << "Robin Load " << endl;
	if(index > 5) index = 1;
	return index++;
}

unsigned int OptLoad::GetQ(std::default_random_engine& rnd) {
	unsigned int small = Qs[0]->LQ;
	vector<unsigned int> v_small = {1};
	for(unsigned int j = 1; j != Qs.size(); ++j) {
		if(small == Qs[j]->LQ) v_small.push_back(j+1);
		else if(small > Qs[j]->LQ) {
			small = Qs[j]->LQ;
			v_small.clear();
			v_small = {j+1};
		}
	}
	if(v_small.size() == 1) return v_small[0];
	for(auto& t : v_small) cout << t << ", ";
	cout << "v_small.size() = " << v_small.size() << endl;
	uniform_int_distribution<unsigned int> dist_uni(0, v_small.size()-1);
	unsigned int rand = dist_uni(rnd);
	cout << "Rand = " << rand << endl;
	if(v_small[rand] == 0) cout << "OptLoad = 0" << endl;
	return v_small[rand];

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


