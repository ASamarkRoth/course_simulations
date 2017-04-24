#ifndef SIGNALS_H
#define SIGNALS_H 1

#include "procs.h"

#include <iostream>
#include <vector>

class Process;

class Signal  {
public:

	Signal(int type, double time) : SignalType(type), ArrivalTime(time) { }

	int SignalType;
	double ArrivalTime;
	//Process& Destination;

	const static int Ready = 1;
	const static int Arrival = 2;
	const static int Measure = 0;

};

std::ostream& operator<<(std::ostream& os, const Signal& s);

class ProcessList {
public:
	ProcessList() = default;

	void AddProcess(Process& p);
	void Update();
	void TreatSignal();

	std::vector<Process*> procs;

};

#endif
