#include "eventandstate.h"

#include <iostream>

using namespace std;

int main() {
	cout << "Running task 1 " << endl;

	EventList el;

	el.InsertEvent(Event(1, 1.));
	el.InsertEvent(Event(2, 2.));
	el.InsertEvent(Event(3, 5.));
	el.InsertEvent(Event(0, 4.));

	State s;

	while(s.nbr_measurements < 4) {
		s.ProcessEvent(el.event_list[0]);
		el.event_list.erase(el.event_list.begin());
		++s.nbr_measurements;
	}


}
