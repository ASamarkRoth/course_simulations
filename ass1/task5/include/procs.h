#ifndef PROCS_H
#define PROCS_H 1

#include <vector>
#include <string>
#include <iostream>
#include <memory>

class Process;

class Signal  {
public:

	Signal(int type, std::string dest, double time);

	int SignalType;
	double ArrivalTime;
	std::string Destination;

	const static int Ready = 1;
	const static int Arrival = 2;
	const static int Measure = 0;

};

std::ostream& operator<<(std::ostream& os, const Signal* s);

class Process {
public:
	std::vector<Signal> SignalList;

	virtual ~Process() = default;
	virtual void TreatSignal() = 0;

	void AddSignal(int type, std::string dest, double time);

	void RemoveSignal();

	virtual std::string GetName() = 0;

};

class Generator : public Process {
public:
	Generator(std::string n) : Process(), Name(n) {}

	virtual ~Generator() {}

	virtual void TreatSignal() override;

	std::string Name;
	virtual std::string GetName() {return Name;}

};

class Queue : public Process {
public:
	Queue(std::string n) : Process(), Name(n) {}

	virtual ~Queue() {}

	virtual void TreatSignal() override;

	std::string Name;
	virtual std::string GetName() {return Name;}

};

class Measure : public Process {
public:
	Measure(std::string n) : Process(), Name(n) {}

	virtual ~Measure() {}

	virtual void TreatSignal() override;

	std::string Name;
	virtual std::string GetName() {return Name;}

};

std::ostream& operator<<(std::ostream& os, Process* p);

class ProcessList {
public:
	ProcessList() = default;

	void AddProcess(std::shared_ptr<Process> p);
	void Update();
	void TreatSignal();

	std::vector<std::shared_ptr<Process>> procs;

};

std::ostream& operator<<(std::ostream& os, ProcessList pl);

#endif
