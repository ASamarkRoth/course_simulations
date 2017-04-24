#include "procs.h"

using namespace std;

int main() {
	cout << "Running task 5" << endl;

	shared_ptr<Generator> G(new Generator());
	shared_ptr<Queue> Q(new Queue());
	shared_ptr<Measure> M(new Measure());

	G->AddSignal(Signal::Ready, G, 1);
	G->AddSignal(Signal::Ready, G, 3);
	Q->AddSignal(Signal::Ready, Q, 0);
	M->AddSignal(Signal::Ready, M, 1.5);
	G->AddSignal(Signal::Ready, G, 2);
	Q->AddSignal(Signal::Ready, Q, 2);

	ProcessList plist;

	plist.AddProcess(G);
	plist.AddProcess(Q);
	plist.AddProcess(M);

	plist.Update();
	cout << plist << endl;

	int j = 0;
	while(j < 6) {
		plist.TreatSignal();
		plist.Update();
		cout << plist << endl;
		++j;
	}




}
