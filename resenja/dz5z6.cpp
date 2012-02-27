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

	int ranksA[size / 2 + size % 2],
		ranksB[size / 2];

	int j=0, k=0;
	for (int i=0; i<size; i++)
		if (i % 2 == 0) ranksA[j++] = i;
		else ranksB[k++] = i;
		
	MPI::Group globalGroup = MPI::COMM_WORLD.Get_group();
	
	MPI::Group group;
	MPI::Intracomm comm;


	if (rank % 2 == 0) 
		group = globalGroup.Incl(size / 2 + size % 2, ranksA);
	else 
		group = globalGroup.Incl(size / 2, ranksB);

	comm = MPI::COMM_WORLD.Create(group);

	int newRank = comm.Get_rank();
	int power = newRank * newRank, powerSum;

	comm.Allreduce(&power, &powerSum, 1, MPI::INT, MPI::SUM);
	pline(); cout << rank << ", " << newRank << ", " << powerSum << endl;
	fflush(stdout); 

	group.Free();
	comm.Free();

	MPI::Finalize();
	
	return 0;
}
