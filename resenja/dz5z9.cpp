#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int rank, size;
int line = 0;

#define ITER 5000

void pline() {
	cout << setw(2) << setfill('0') << rank << ":";
	cout << setw(3) << setfill('0') << ++line << ":";
}

int main(int argc, char* argv[]) {
	MPI::Init(argc, argv);
	
	rank = MPI::COMM_WORLD.Get_rank();
	size = MPI::COMM_WORLD.Get_size();
	if (size < 2) MPI::COMM_WORLD.Abort(1);

	srand(rank);
 
	int K, brojGrupa;

	if (0 == rank) {
		pline(); cout << "Koliko clanova u grupi (broj K): ";
		fflush(stdout); cin >> K;
	}
	
	MPI::COMM_WORLD.Bcast(&K, 1, MPI::INT, 0);
	if (K >= size) MPI::COMM_WORLD.Abort(1);
	
	brojGrupa = size / K;
	
	MPI::Intracomm comm = MPI::COMM_WORLD.Split(rank % brojGrupa, rank);
	int newRank = comm.Get_rank(),
		newSize = comm.Get_size();

	double x, y, xy;
	int in_circle = 0, summed_dots;
	for (int i = 0; i < ITER; i++) {
		x = (double)rand() / (double)RAND_MAX;
		y = (double)rand() / (double)RAND_MAX;
	
		xy = x*x + y*y;
		if (xy < 1) in_circle++;
	}

	comm.Allreduce(&in_circle, &summed_dots, 1, MPI::INT, MPI::SUM);
	
	if (0 == newRank) {
		long num_of_dots = ITER * newSize; // TOOPT
		double pi = 4.0 * (double)summed_dots/(double)num_of_dots;
		
		if (0 == rank) {
			double sumPi = pi, piRec = 0;
			
			for (int i=0; i<brojGrupa-1; i++) {
				MPI::COMM_WORLD.Recv(&piRec, 1, MPI::DOUBLE, i+1, 0);
				sumPi += piRec;
			}
			pline(); cout << "Pi: " << sumPi / (double)brojGrupa << endl;
		}
		else {
			MPI::COMM_WORLD.Send(&pi, 1, MPI::DOUBLE, 0, 0);
		}
	}
	
	comm.Free();
	
	MPI::Finalize();
	
	return 0;
}
