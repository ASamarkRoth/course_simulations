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

State::State(std::default_random_engine& re, double l) : lambda(l), nbr_measurements(0), rnd_engine(re) {
	NC = 0;
}

void State::ProcessEvent(EventList& el) {
	Event e = el.event_list[0];
	double new_time;
	switch (e.eventtype){
		case Event::Arrival:
			cout << "Arrival: " << e << endl;
			++NC;
			new_time = e.eventtime + get_uni_time(rnd_engine);
			if(new_time < 8*60) {
				el.InsertEvent(Event::Arrival, new_time);
			}
			if(NC == 1) {
				el.InsertEvent(Event::Depart, e.eventtime + get_uni_time(rnd_engine));
			}
			v_arrival_time.push_back(e.eventtime);
			break;
		case Event::Depart:
			cout << "Depart: " << e << endl;
			--NC;
			if(NC > 0) el.InsertEvent(Event::Depart, e.eventtime + get_uni_time(rnd_engine));
			v_mean_time.push_back(e.eventtime - v_arrival_time[0]);
			v_arrival_time.erase(v_arrival_time.begin());
			break;
			/*
		case Event::Measure:
			cout << "Measuring: " << NC << ", " << e.eventtime << endl;
			++nbr_measurements;
			el.InsertEvent(Event::Measure, e.eventtime + T);
			v_NC.push_back(NC);
			v_mean.push_back(calc_mean(v_NC));
			v_var.push_back(calc_stddev(v_NC));
			v_time.push_back(e.eventtime);
			break;
			*/
	}
}
void State::Write(string s) {
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

	g = new TGraph(v_NC.size(), &(v_time[0]), &(v_NC[0]));
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

double get_uni_time(default_random_engine& rnd) {
	uniform_real_distribution<double> dist_uni(10, 20);
	double rand = dist_uni(rnd);
	cout << "rnd = " << rand << endl;
	return rand;
}

