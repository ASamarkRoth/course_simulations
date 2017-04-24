#include "procs.h"

#include <iostream>
#include <algorithm>

using namespace std;


Signal::Signal(int type, shared_ptr<Process> dest, double time) : SignalType(type), ArrivalTime(time), Destination(dest) { }

ostream& operator<<(ostream& os, Signal* s) {
	os << "signal: " << s->SignalType << ", " << s->Destination->GetName() << ", " << s->ArrivalTime;
	return os;
}

void Process::AddSignal(int type, shared_ptr<Process> dest, double time) {
	Signal* s = new Signal(type, dest, time);
	if(SignalList.size() == 0) {
		SignalList.push_back(s);
		return;
	}
	auto it = SignalList.begin();
	while(it != SignalList.end() && s->ArrivalTime > (*it)->ArrivalTime) {
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
			else return a->SignalList[0]->ArrivalTime < b->SignalList[0]->ArrivalTime;
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

// This class defines a simple queuing system with one server. It inherits Proc so that we can use time and the
// signal names without dot notation

/*
class QS extends Proc{
	public int numberInQueue = 0, accumulated, noMeasurements;
	public Proc sendTo;
	Random slump = new Random();

	public void TreatSignal(Signal x){
		switch (x.signalType){

			case ARRIVAL:{
				numberInQueue++;
				if (numberInQueue == 1){
					SignalList.SendSignal(READY,this, time + 0.2*slump.nextDouble());
				}
			} break;

			case READY:{
				numberInQueue--;
				if (sendTo != null){
					SignalList.SendSignal(ARRIVAL, sendTo, time);
				}
				if (numberInQueue > 0){
					SignalList.SendSignal(READY, this, time + 0.2*slump.nextDouble());
				}
			} break;

			case MEASURE:{
				noMeasurements++;
				accumulated = accumulated + numberInQueue;
				SignalList.SendSignal(MEASURE, this, time + 2*slump.nextDouble());
			} break;
		}
	}
}
*/

std::ostream& operator<<(ostream& os, Process* p) {
	for(unsigned int i = 0; i != p->SignalList.size(); ++i) {
		os << i << " " << p->SignalList[i] << endl;
	}
	return os;
}

void Generator::TreatSignal() {
	cout << "Treating signal in generator" << endl;
	AddSignal(

}
void Queue::TreatSignal() {
	cout << "Treating signal in queue" << endl;
}
void Measure::TreatSignal() {
	cout << "Treating signal in queue" << endl;
}

//Denna klass ärver Proc, det gör att man kan använda time och signalnamn utan punktnotation
//It inherits Proc so that we can use time and the signal names without dot notation

/*
class Gen extends Proc{

	//Slumptalsgeneratorn startas:
	//The random number generator is started:
	Random slump = new Random();

	//Generatorn har två parametrar:
	//There are two parameters:
	public Proc sendTo;    //Anger till vilken process de genererade kunderna ska skickas //Where to send customers
	public double lambda;  //Hur många per sekund som ska generas //How many to generate per second

	//Här nedan anger man vad som ska göras när en signal kommer //What to do when a signal arrives
	public void TreatSignal(Signal x){
		switch (x.signalType){
			case READY:{
				SignalList.SendSignal(ARRIVAL, sendTo, time);
				SignalList.SendSignal(READY, this, time + (2.0/lambda)*slump.nextDouble());}
				break;
		}
	}
}
*/
