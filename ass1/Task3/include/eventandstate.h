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
	const static int ArrivalQ1 = 1;
	const static int DepartQ1 = 2;
	const static int DepartQ2 = 3;

};

class EventList {
public:
	std::vector<Event> event_list;
	void InsertEvent(int type, double time);
};

class State {
public:

	State(std::default_random_engine& re, double dt);

	void ProcessEvent(EventList& e);
	void Write(std::string s);
	unsigned int LQ1, LQ2, nbr_arrivalsQ1, nbr_rejectedQ1, nbr_measurements;

	std::default_random_engine rnd_engine;
	double dtQ1;
	std::vector<double> v_LQ1, v_LQ2, v_rej_ratio, v_time, v_mean, v_var;
	EventList el;

};

double get_exp_time(std::default_random_engine& rnd, double mu);
double calc_mean(std::vector<double>& v);
double calc_stddev(std::vector<double>& v);
std::ostream& operator<<(std::ostream& os, Event e);

#endif
