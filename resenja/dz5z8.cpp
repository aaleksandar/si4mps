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

	MPI::Intracomm comm = MPI::COMM_WORLD.Split(rank % 2, rank);

	int newRank = comm.Get_rank();
	int power = newRank * newRank, powerSum;

	comm.Allreduce(&power, &powerSum, 1, MPI::INT, MPI::SUM);
	pline(); cout << rank << ", " << newRank << ", " << powerSum << endl;
	fflush(stdout); 

	comm.Free();

	MPI::Finalize();
	
	return 0;
}
