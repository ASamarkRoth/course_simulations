#include "eventandstate.h"

#include <iostream>
#include <algorithm>
#include <math.h>

#include "TString.h"
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TColor.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLatex.h"

using namespace std;

void EventList::InsertEvent(int type, double time) {
	Event e(type, time);
	if(event_list.size() == 0) {
		event_list.push_back(e);
		return;
	}

	auto it = event_list.begin();
	while(it != event_list.end() && e.eventtime > it->eventtime) {
		++it;
	}

	it = event_list.insert(it, e);
}

State::State(default_random_engine& re, double dt) : LQ1(0), LQ2(0), nbr_arrivalsQ1(0), nbr_rejectedQ1(0), nbr_measurements(0), rnd_engine(re), dtQ1(dt) {

}

void State::ProcessEvent(EventList& el) {
	Event e = el.event_list[0];
	switch (e.eventtype){
		case Event::ArrivalQ1:
			cout << "ArrivalQ1: " << e << endl;
			++LQ1;
			el.InsertEvent(Event::ArrivalQ1, e.eventtime + get_exp_time(rnd_engine, dtQ1));
			if(LQ1 == 1) el.InsertEvent(Event::DepartQ1, e.eventtime + get_exp_time(rnd_engine, 1));
			v_in_Q.push_back(e.eventtime);
			break;
		case Event::DepartQ1:
			cout << "DepartQ1: " << e << endl;
			--LQ1;
			++LQ2;
			if(LQ1 > 0) el.InsertEvent(Event::DepartQ1, e.eventtime + get_exp_time(rnd_engine, 1));
			if(LQ2 == 1) el.InsertEvent(Event::DepartQ2, e.eventtime + get_exp_time(rnd_engine, 1));
			break;
		case Event::DepartQ2:
			cout << "DepartQ2: " << e << endl;
			--LQ2;
			if(LQ2 > 0) el.InsertEvent(Event::DepartQ2, e.eventtime + get_exp_time(rnd_engine, 1));
			v_mean_time.push_back(e.eventtime - v_in_Q[0]);
			v_in_Q.erase(v_in_Q.begin());
			break;
		case Event::Measure:
			cout << "Measuring: " << LQ1 << ", "<< LQ2 << ", " << e.eventtime << endl;
			++nbr_measurements;
			el.InsertEvent(Event::Measure, e.eventtime + get_exp_time(rnd_engine, 5));
			v_LQ1.push_back(LQ1);
			v_LQ2.push_back(LQ2);
			v_mean.push_back(calc_mean(v_LQ2));
			v_var.push_back(calc_stddev(v_LQ2));
			v_time.push_back(e.eventtime);
			break;
	}
}
void State::Write(string s) {
	cout << "Writing to file task3.root" << endl;
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
	TString s_y[4] = {"Length Q1", "Length Q2", "Mean LQ2", "StdDev LQ2"};

	g = new TGraph(v_LQ2.size(), &(v_time[0]), &(v_LQ1[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[0]);
	g->GetXaxis()->SetTitle(s_x[0]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();

	g = new TGraph(v_LQ2.size(), &(v_time[0]), &(v_LQ2[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[1]);
	g->GetXaxis()->SetTitle(s_x[1]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();

	g = new TGraph(v_mean.size(), &(v_time[0]), &(v_mean[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[2]);
	g->GetXaxis()->SetTitle(s_x[1]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();

	g = new TGraph(v_var.size(), &(v_time[0]), &(v_var[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[3]);
	g->GetXaxis()->SetTitle(s_x[1]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();


	f_out->Close();


}

double get_exp_time(default_random_engine& rnd, double mu) {
	exponential_distribution<double> dist_exp(1/mu);
	double rand = dist_exp(rnd);
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

ostream& operator<<(ostream& os, Event e) {
	os << "Event: " << e.eventtype << ", " << e.eventtime << " s";
	return os;
}


