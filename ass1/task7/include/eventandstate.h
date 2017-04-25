#ifndef EVENTANDSTATE_H
#define EVENTANDSTATE_H 1

#include <vector>
#include <string>
#include <random>
#include <iostream>


class Event {
public:

	Event(int etype, double etime) : eventtype(etype), eventtime(etime) {}

	int eventtype;
	double eventtime;

	const static int Measure = 0;
	const static int Arrival= 1;
	const static int Depart= 2;

};

class EventList {
public:
	std::vector<Event> event_list;
	void InsertEvent(int type, double time);
};

class State {
public:

	State(std::default_random_engine& re, double l);

	void ProcessEvent(EventList& e);
	void Write(std::string s);
	unsigned int NC, nbr_measurements;
	double lambda;

	std::default_random_engine rnd_engine;
	std::vector<double> v_NC, v_arrival_time, v_time, v_mean, v_var, v_mean_time;
	EventList el;

};

double get_exp_time(std::default_random_engine& rnd, double mu);
double get_uni_time(std::default_random_engine& rnd);
double calc_mean(std::vector<double>& v);
double calc_stddev(std::vector<double>& v);
std::ostream& operator<<(std::ostream& os, Event e);

#endif
