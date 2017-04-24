#include "procs.h"

#include <iostream>
#include <algorithm>

using namespace std;


Signal::Signal(int type, string dest, double time) : SignalType(type), ArrivalTime(time), Destination(dest) { }

ostream& operator<<(ostream& os, Signal s) {
	os << "signal: " << s.SignalType << ", " << s.Destination << ", " << s.ArrivalTime;
	return os;
}

void Process::AddSignal(int type, string dest, double time) {
	Signal s(type, dest, time);
	if(SignalList.size() == 0) {
		SignalList.push_back(s);
		return;
	}
	auto it = SignalList.begin();
	while(it != SignalList.end() && s.ArrivalTime > it->ArrivalTime) {
		++it;
	}

	it = SignalList.insert(it, s);
}

void Process::RemoveSignal() {
	if(SignalList.size() != 0) {
		//delete SignalList[0];
		SignalList.erase(SignalList.begin());
	}
	else {
		std::cout << "SignalList empty!!!!" << std::endl;
		exit(0);
	}
}
void ProcessList::AddProcess(shared_ptr<Process> p) {
	procs.push_back(p);
}

void ProcessList::Update() {
	cout << "Updating process list" << endl;
	//Sorting the process list
	sort(procs.begin(), procs.end(),
			[](const shared_ptr<Process> a, const shared_ptr<Process> b) {
			if(a->SignalList.size() == 0) return false;
			else if(b->SignalList.size() == 0) return true;
			else if(a->SignalList.size() == 0 && b->SignalList.size() == 0) return false;
			else return a->SignalList[0].ArrivalTime < b->SignalList[0].ArrivalTime;
			}
			);
}

void ProcessList::TreatSignal() {
	cout << "Treating signal from ProcessList" << endl;
	procs[0]->TreatSignal();
	procs[0]->RemoveSignal();
}

std::ostream& operator<<(std::ostream& os, ProcessList pl) {
	os << "Listing processes: " << endl;
	for(auto p : pl.procs) {
		if(p->SignalList.size() == 0) continue;
		os << p->SignalList[0] << endl;
	}
	return os;
}

std::ostream& operator<<(ostream& os, Process* p) {
	for(unsigned int i = 0; i != p->SignalList.size(); ++i) {
		os << i << " " << p->SignalList[i] << endl;
	}
	return os;
}

void Generator::TreatSignal() {
	cout << "Treating signal in generator" << endl;

}
void Queue::TreatSignal() {
	cout << "Treating signal in queue" << endl;
}
void Measure::TreatSignal() {
	cout << "Treating signal in queue" << endl;
}

