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
	const static int AddJobA = 1;
	const static int AddJobB = 2;
	const static int ServeJobA = 3;
	const static int ServeJobB = 4;

};

class EventList {
public:
	std::vector<Event> event_list;
	void InsertEvent(int type, double time);
};

class State {
public:

	State(std::default_random_engine& re, bool is_B);

	void ProcessEvent(EventList& e);
	void AddJobToBuffer(Event e);
	void Write(std::string s);
	unsigned int NA, NB, nbr_measurements;

	std::default_random_engine rnd_engine;
	bool is_B_priority, is_server_busy;
	std::vector<double> v_NA, v_NB, v_NAB, v_time, v_mean, v_var;

	EventList el;
	std::vector<Event> v_Buffer;

};

double get_exp_time(std::default_random_engine& rnd, double mu);
double calc_mean(std::vector<double>& v);
double calc_stddev(std::vector<double>& v);
std::ostream& operator<<(std::ostream& os, Event e);

#endif
