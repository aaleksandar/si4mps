#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int rank, size;
int line = 0;

#define COLS 3
#define ROWS 5

void pline() {
	cout << setw(2) << setfill('0') << rank << ":";
	cout << setw(3) << setfill('0') << ++line << ":";
}

int main(int argc, char* argv[]) {
	MPI::Init(argc, argv);
	
	rank = MPI::COMM_WORLD.Get_rank();
	size = MPI::COMM_WORLD.Get_size();
	if (size < 2) MPI::COMM_WORLD.Abort(1);
	if (size < 1+COLS+ROWS) MPI::COMM_WORLD.Abort(1);
	
	MPI::Group globalGroup = MPI::COMM_WORLD.Get_group();

	if (0 == rank) {
		int matrix[COLS][ROWS], xirtam[ROWS][COLS];
	
		srand(time(0));
		for (int i=0; i<COLS; i++)
			for (int j=0; j<ROWS; j++) {
				matrix[i][j] = 9 * (double)rand() / (double)RAND_MAX;
				xirtam[j][i] = matrix[i][j];
			}
		
		cout << "random matrica: " << endl;
		for (int i=0; i<COLS; i++) {
			for (int j=0; j<ROWS; j++)
				cout << matrix[i][j] << " ";
			cout << endl;
		}
	}
	else {	
		MPI::Group group;
		MPI::Intracomm comm;

		int j=0, k=0;
		for (int i=1; i<size; i++)
			if (i % 2) ranksA[j++] = i;
			else ranksB[k++] = i;

		if (rank % 2) 
			group = globalGroup.Incl(size / 2 + size % 2, ranksA);
		else 
			group = globalGroup.Incl(size / 2, ranksB);

		comm = MPI::COMM_WORLD.Create(group);
		int newRank = comm.Get_rank();
	
	
		pline(); cout << rank << ", " << newRank << ", " << powerSum << endl;
		fflush(stdout); 

		group.Free();
		comm.Free();
	}
	
	
	
	//comm.Free();
	
	MPI::Finalize();
	
	return 0;
}
