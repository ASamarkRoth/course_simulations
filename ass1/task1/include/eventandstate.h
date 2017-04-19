#ifndef EVENTANDSTATE_H
#define EVENTANDSTATE_H 1

#include <vector>

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
	void InsertEvent(Event e);
};

class State {
public:

	State();

	void ProcessEvent(Event& e);
	std::size_t LQ1, LQ2, nbr_arrivalsQ1, nbr_rejectedQ1, nbr_measurements;

	std::default_random_engine rnd;

};

#endif
