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
	if (size < 2) MPI::COMM_WORLD.Abort(1);

	int num;

	if (0 == rank) {
	// master
		pline(); cout << "broj tacaka za obradu, po procesu: ";
		fflush(stdout);
		cin >> num;

		for (int i = 1; i < size; i++) {
			MPI::COMM_WORLD.Send(&num, 1, MPI::INT, i, 0);
		}

		int in_circle = 0;
		for (int i = 1; i < size; i++) {
			int fragment;
			MPI::COMM_WORLD.Recv(&fragment, 1, MPI::INT, i, 0);
			in_circle += fragment;
		}

		long num_of_dots = num * (size - 1);
		double pi = 4.0 * (double)in_circle/(double)num_of_dots;
		
		pline();
		cout << "Pi (approx) = " << pi << endl;
	}
	else {
	// slave
		MPI::COMM_WORLD.Recv(&num, 1, MPI::INT, 0, 0);

		double x, y, xy;
		int in_circle = 0;
		srand(time(0));

		for (int i = 0; i < num; i++) {
			x = (double)rand() / (double)RAND_MAX;
			y = (double)rand() / (double)RAND_MAX;
		
			xy = x*x + y*y;
			if (xy < 1) in_circle++;
		}

		MPI::COMM_WORLD.Send(&in_circle, 1, MPI::INT, 0, 0);
	}

	MPI::Finalize();
	
	return 0;
}
