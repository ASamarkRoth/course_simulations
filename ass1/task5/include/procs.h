#ifndef PROCS_H
#define PROCS_H 1

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <random>

class Process;

class Signal  {
public:

	Signal(int type, std::string dest, double time, int meas);

	int SignalType;
	double ArrivalTime;
	std::string Destination;
	int Meas;

	const static int Ready = 1;
	const static int Arrival = 2;
	const static int Measure = 0;

};

std::ostream& operator<<(std::ostream& os, const Signal* s);

class Process {
public:
	std::vector<Signal> SignalList;
	std::default_random_engine rnd_engine;

	Process(std::default_random_engine& rnd) : rnd_engine(rnd) {}

	virtual ~Process() = default;
	virtual void TreatSignal(Signal x) = 0;

	virtual void AddSignal(int type, std::string dest, double time, int meas = 0);

	virtual void RemoveSignal();

	virtual std::string GetName() const = 0;

};

class LoadDistr;

class Generator : public Process {
public:
	Generator(std::string n, std::default_random_engine& rnd, double t, std::shared_ptr<LoadDistr>& load) : Process(rnd), Name(n), t_mean(t), Load(load), nbr_arrivals(0) {}

	virtual ~Generator() {}

	virtual void TreatSignal(Signal x) override;

	const std::string Name;
	virtual std::string GetName() const {return Name;}

	double t_mean;
	std::shared_ptr<LoadDistr> Load;
	unsigned int nbr_arrivals;
	std::vector<double> arrival_times;

};

class Queue : public Process {
public:
	Queue(std::string n, std::default_random_engine& rnd) : Process(rnd), Name(n), LQ(0), nbr_ready(0) {}

	virtual ~Queue() {}

	virtual void TreatSignal(Signal x) override;

	const std::string Name;
	virtual std::string GetName() const {return Name;}

	unsigned int LQ, nbr_ready;
	std::vector<double> arrival_time, w_time;

};

class Measure : public Process {
public:
	Measure(std::string n, std::default_random_engine& rnd, double t) : Process(rnd), Name(n), nbr_measurements(0), t_mean(t), LQ(0), measured(0) {}

	virtual ~Measure() {}

	virtual void TreatSignal(Signal x) override;

	const std::string Name;
	virtual std::string GetName() const {return Name;}

	unsigned int nbr_measurements, LQ, measured;
	double t_mean;
	std::vector<double> v_LQ, v_time, v_mean, v_var;

	void Write(std::string s);

};

std::ostream& operator<<(std::ostream& os, Process* p);

class ProcessList {
public:
	ProcessList() = default;

	void AddProcess(std::shared_ptr<Process> p);
	void Update();
	std::shared_ptr<Process> FetchProcess(Signal x);
	void TreatSignal();

	std::vector<std::shared_ptr<Process>> procs;

};

std::ostream& operator<<(std::ostream& os, ProcessList pl);

class LoadDistr {
public:

	std::vector<std::shared_ptr<Queue>> Qs;

	LoadDistr(std::vector<std::shared_ptr<Queue>>& queues) : Qs(queues) {}

	virtual ~LoadDistr() = default;

	virtual unsigned int GetQ(std::default_random_engine&) = 0;

	virtual std::string GetName() = 0;
};

class RandomLoad : public LoadDistr {
public:
	virtual ~RandomLoad() = default;

	RandomLoad(std::vector<std::shared_ptr<Queue>>& queues) : LoadDistr(queues) {}

	virtual unsigned int GetQ(std::default_random_engine&);

	virtual std::string GetName() {return Name;}
	std::string Name = "Random";
};

class RobinLoad : public LoadDistr {
public:
	virtual ~RobinLoad() = default;

	RobinLoad(std::vector<std::shared_ptr<Queue>>& queues) : LoadDistr(queues), index(1) {}

	virtual unsigned int GetQ(std::default_random_engine&);

	unsigned int index;

	virtual std::string GetName() {return Name;}
	std::string Name = "Robin";
};

class SmallestQueLoad : public LoadDistr {
public:
	virtual ~SmallestQueLoad() = default;

	SmallestQueLoad(std::vector<std::shared_ptr<Queue>>& queues) : LoadDistr(queues), index(1) {}

	virtual unsigned int GetQ(std::default_random_engine&);

	unsigned int index;

	virtual std::string GetName() {return Name;}
	std::string Name = "Opt";
};

double get_exp_time(std::default_random_engine& rnd, double mu);
double get_uni_time(std::default_random_engine& rnd, double mu);
double calc_mean(std::vector<double>& v);
double calc_stddev(std::vector<double>& v);

#endif
