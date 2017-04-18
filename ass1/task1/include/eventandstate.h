class Event {
public:
	int eventtype;
	double eventtime;

	static int Measure = 0;
	static int ArrivalQ1 = 1;
	static int DepartQ1 = 2;
	static int DepartQ2 = 3;

};

class EventList {
public:
	std::vector<Event> event_list;
	void InsertEvent(Event e);
}

class State {
public:
	State() : LQ1(0), LQ2(0), nbr_arrivalsQ1(0), nbr_rejectedQ1(0) {}

	size_t LQ1, LQ2, nbr_arrivalsQ1, nbr_rejectedQ1;
	ProcessEvent();

};

