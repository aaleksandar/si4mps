#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>


using namespace std;

int rank, size;
int line = 0;

void pline() {
	cout << setw(2) << setfill('0') << rank << ":";
	cout << setw(3) << setfill('0') << ++line << ":";
}


int main(int argc, char* argv[]) {
	MPI::Init(argc, argv);
	
	rank = MPI::COMM_WORLD.Get_rank();
	size = MPI::COMM_WORLD.Get_size();
	if (size < 1) MPI::COMM_WORLD.Abort(1);

	double num;

	if (0 == rank) {
		pline(); cout << "unesi broj: ";
		fflush(stdout);
		cin >> num;
		
		MPI::COMM_WORLD.Bcast(&num, 1, MPI::DOUBLE, 0);
	}
	else {
		MPI::COMM_WORLD.Bcast(&num, 1, MPI::DOUBLE, 0);
		pline();
		cout << num << endl;
	}

	MPI::Finalize();
	
	return 0;
}
