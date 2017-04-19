#include "eventandstate.h"

#include <iostream>
#include <random>

using namespace std;

void EventList::InsertEvent(Event e) {
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

State() : LQ1(0), LQ2(0), nbr_arrivalsQ1(0), nbr_rejectedQ1(0), nbr_measurements(0), rnd(0) {

}

//Fixa struktur med exp dist och rand. Obs, hur fås tiden? Se compile.cc

void State::ProcessEvent(Event& e) {
		switch (e.eventtype){
			case Event::ArrivalQ1:
				cout << "This is ArrivalQ1 " << endl;
				break;
			case Event::DepartQ1:
				cout << "This is DepartQ1 " << endl;
				break;
			case Event::DepartQ2:
				cout << "This is DepartQ2 " << endl;
				break;
			case Event::Measure:
				cout << "This is Measure " << endl;
				break;
		}
}
