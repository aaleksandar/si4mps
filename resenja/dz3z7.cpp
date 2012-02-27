#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int rank, size;
int line = 0;
int summed_dots;

void pline() {
	cout << setw(2) << setfill('0') << rank << ":";
	cout << setw(3) << setfill('0') << ++line << ":";
}



int main(int argc, char* argv[]) {
	MPI::Init(argc, argv);
	
	rank = MPI::COMM_WORLD.Get_rank();
	size = MPI::COMM_WORLD.Get_size();
	if (size < 1) MPI::COMM_WORLD.Abort(1);

	int num;
	double x, y, xy;
	int in_circle = 0;
	
	if (0 == rank) {
	// master
		pline(); cout << "broj tacaka za obradu, po procesu: ";
		fflush(stdout);
		cin >> num;

		MPI::COMM_WORLD.Bcast(&num, 1, MPI::INT, 0);
		
		for (int i = 0; i < num; i++) {
			x = (double)rand() / (double)RAND_MAX;
			y = (double)rand() / (double)RAND_MAX;
			xy = x*x + y*y;
			if (xy < 1) in_circle++;
		}
		// ucestvuje u obradi
		
		MPI::COMM_WORLD.Reduce(&in_circle, &summed_dots, 1, MPI::INT, MPI::SUM, 0);

		long num_of_dots = num * size;
		double pi = 4.0 * (double)summed_dots/(double)num_of_dots;
		
		pline();
		cout << "Pi (approx) = " << pi << endl;
	}
	else {
	// slave
		MPI::COMM_WORLD.Bcast(&num, 1, MPI::DOUBLE, 0);

		srand(time(0));

		for (int i = 0; i < num; i++) {
			x = (double)rand() / (double)RAND_MAX;
			y = (double)rand() / (double)RAND_MAX;
		
			xy = x*x + y*y;
			if (xy < 1) in_circle++;
		}

		MPI::COMM_WORLD.Reduce(&in_circle, &summed_dots, 1, MPI::INT, MPI::SUM, 0);
	}

	MPI::Finalize();
	
	return 0;
}
