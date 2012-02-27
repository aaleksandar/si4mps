#include <mpi.h>
#include <iostream>
#include <iomanip>

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
	
	pline();
	cout << "rang: " << rank << "; broj procesa: " << size << endl;	
	
	MPI::Finalize();
	
	return 0;
}
