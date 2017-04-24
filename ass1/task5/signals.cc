#include "signals.h"

#include <algorithm>
#include <iostream>

using namespace std;

ostream& operator<<(ostream& os, Signal s) {
	os << "signal: " << s.SignalType << ", " << s.Destination.Name << ", " << s.ArrivalTime << endl;
	return os;
}

/*
void ProcessList::AddProcess(Process& p) {
	procs.push_back(p);
}

void ProcessList::Update() {
	//Sorting the process list
	sort(procs.begin(), procs.end(),
			[](const Process& a, const Process& b) {
			if(a.SignalList.size() == 0) return false;
			else if(b.SignalList.size() == 0) return true;
			else if(a.SignalList.size() == 0 && b.SignalList.size() == 0) return false;
			else return a.SignalList[0].time < b.SignalList[0].time;
			}
			);
}

void ProcessList::TreatSignal() {
	procs[0].TreatSignal();
	procs[0].RemoveSignal();
}
*/
