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
	while(it != event_list.end() && e.eventtime >= it->eventtime) {
		++it;
	}

	it = event_list.insert(it, e);
}

State::State(default_random_engine& re, bool is_B) : NA(0), NB(0), nbr_measurements(0), rnd_engine(re), is_B_priority(is_B), is_server_busy(false) {

}

void State::ProcessEvent(EventList& el) {
	Event e = el.event_list[0];
	cout << "Processing event: " << endl;
	switch (e.eventtype){
		case Event::AddJobA:
			cout << "AddJobA " << e << endl;
			el.InsertEvent(Event::AddJobA, e.eventtime + get_exp_time(rnd_engine, 1./150));
			//If buffer empty and server not busy, need to trigger serve.
			if(v_Buffer.size() == 0 && !is_server_busy) {
				el.InsertEvent(Event::ServeJobA, e.eventtime + 0.002);
				is_server_busy = true;
				break;
			}
			AddJobToBuffer(e);
			++NA;
			break;
		case Event::AddJobB:
			cout << "AddJobB " << e << endl;
			//If buffer empty, need to trigger serve.
			if(v_Buffer.size() == 0 && !is_server_busy) {
				el.InsertEvent(Event::ServeJobB, e.eventtime + 0.004);
				is_server_busy = true;
				break;
			}
			AddJobToBuffer(e);
			++NB;
			break;
		case Event::ServeJobA:
			cout << "ServeJobA: " << e << endl;
			if(v_Buffer.size() != 0) {
				if(v_Buffer[0].eventtype == Event::AddJobB) {
					cout << "Processing Job B" << endl;
						el.InsertEvent(Event::ServeJobB, e.eventtime + 0.004);
						auto it = v_Buffer.erase(v_Buffer.begin());
						--NB;
					}
					else {
						cout << "Processing Job A" << endl;
						el.InsertEvent(Event::ServeJobA, e.eventtime + 0.002);
						auto it = v_Buffer.erase(v_Buffer.begin());
						--NA;
						el.InsertEvent(Event::AddJobB, e.eventtime + 1);
					}
					is_server_busy = true;
			}
			else {
				is_server_busy = false;
				el.InsertEvent(Event::AddJobB, e.eventtime + 1);
			}
			break;
		case Event::ServeJobB:
			cout << "ServeJobB: " << e << endl;
			if(v_Buffer.size() != 0) {
				if(v_Buffer[0].eventtype == Event::AddJobB) {
						el.InsertEvent(Event::ServeJobB, e.eventtime + 0.004);
						auto it = v_Buffer.erase(v_Buffer.begin());
						--NB;
					}
					else {
						el.InsertEvent(Event::ServeJobA, e.eventtime + 0.002);
						auto it = v_Buffer.erase(v_Buffer.begin());
						--NA;
						el.InsertEvent(Event::AddJobB, e.eventtime + 1);
					}
					is_server_busy = true;
			}
			else {
				is_server_busy = false;
			}
			break;
		case Event::Measure:
			cout << "Measuring: " << NA << ", "<< NB << ", " << e.eventtime << endl;
			++nbr_measurements;
			el.InsertEvent(Event::Measure, e.eventtime + 0.1);
			v_NA.push_back(NA);
			v_NB.push_back(NB);
			v_NAB.push_back(v_Buffer.size());
			v_mean.push_back(calc_mean(v_NAB));
			v_var.push_back(calc_stddev(v_NAB));
			v_time.push_back(e.eventtime);
			break;
	}
}

void State::AddJobToBuffer(Event e) {
	cout << "Adding to buffer Job " << e.eventtype << endl;
	if(v_Buffer.size() == 0) {
		v_Buffer.push_back(e);
		return;
	}
	if( (e.eventtype == Event::AddJobA && !is_B_priority) || (e.eventtype == Event::AddJobB && is_B_priority)) {
		auto it = find_if(v_Buffer.begin(), v_Buffer.end(), [e] (const Event& b) {return b.eventtype != e.eventtype;});
		v_Buffer.insert(it, e);
	}
	else v_Buffer.push_back(e);

}

void State::Write(string s) {
	cout << "Writing to file task1.root" << endl;
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
	TString s_y[5] = {"NAB", "NA", "NB", "Mean NAB", "StdDev NAB"};

	g = new TGraph(v_NAB.size(), &(v_time[0]), &(v_NAB[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[0]);
	g->GetXaxis()->SetTitle(s_x[0]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();

	g = new TGraph(v_NAB.size(), &(v_time[0]), &(v_NA[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[1]);
	g->GetXaxis()->SetTitle(s_x[1]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();

	g = new TGraph(v_NB.size(), &(v_time[0]), &(v_NB[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[2]);
	g->GetXaxis()->SetTitle(s_x[1]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();

	g = new TGraph(v_mean.size(), &(v_time[0]), &(v_mean[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[3]);
	g->GetXaxis()->SetTitle(s_x[1]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();

	g = new TGraph(v_var.size(), &(v_time[0]), &(v_var[0]));
	g->Draw("ACP"); //For the first one, one needs to draw axis with "A". Option "SAME" is not needed with TGraph!
	//Axis objects for TGraph are created after it has been drawn, thus they need to be defined here.
	g->SetTitle("");
	g->GetYaxis()->SetTitle(s_y[4]);
	g->GetXaxis()->SetTitle(s_x[1]);
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->CenterTitle();
	g->Write();


	f_out->Close();


}

double get_exp_time(default_random_engine& rnd, double mu) {
	exponential_distribution<double> dist_exp(1/mu);
	double rand = dist_exp(rnd);
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


