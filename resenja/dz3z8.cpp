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

	int max;
	int chunk_size;
			
	if (0 == rank) {
	// master
		pline(); cout << "broj elemenata niza: ";
		fflush(stdout);
		cin >> max;
		
		MPI::COMM_WORLD.Bcast(&max, 1, MPI::INT, 0);
		if (max % size) MPI::COMM_WORLD.Abort(1); // ako nije celobrojni umnozak
		
		chunk_size = max / size;
		
		pline(); cout << "ucitaj elemente: ";
		int niz[max];
		for (int i=0; i<max; i++) {
			fflush(stdout);
			cin >> niz[i];
		}
		
		int chunk[chunk_size];

		MPI::COMM_WORLD.Scatter(niz, chunk_size, MPI::INT, chunk, chunk_size, MPI::INT, 0);
	
		for (int i=0; i<chunk_size; i++) {
			chunk[i] *= chunk[i];
		}
		
		MPI::COMM_WORLD.Gather(chunk, chunk_size, MPI::INT, niz, chunk_size, MPI::INT,0);

		pline(); cout << "kvadriran niz: ";
		for (int i=0; i<max; i++) {
			cout << niz[i] << " ";
		}
		cout << endl;
	}
	else {
	// slave
		MPI::COMM_WORLD.Bcast(&max, 1, MPI::INT, 0);
		if (max % size) MPI::COMM_WORLD.Abort(1); // ako nije celobrojni umnozak
		
		chunk_size = max / size;
		
		int chunk[chunk_size];
		MPI::COMM_WORLD.Scatter(0, chunk_size, MPI::INT, chunk, chunk_size, MPI::INT, 0);
		
		for (int i=0; i<chunk_size; i++) {
			chunk[i] *= chunk[i];
		}

		MPI::COMM_WORLD.Gather(chunk, chunk_size, MPI::INT, 0, chunk_size, MPI::INT,0);
	}

	MPI::Finalize();
	
	return 0;
}
