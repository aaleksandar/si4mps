#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int rank, size;
int line = 0;

typedef struct Elem {
	double num;
	int pos;
} elem;

void pline() {
	cout << setw(2) << setfill('0') << rank << ":";
	cout << setw(3) << setfill('0') << ++line << ":";
}

MPI_Datatype MPI_Elem;


int main(int argc, char* argv[]) {
	MPI::Init(argc, argv);
	
	rank = MPI::COMM_WORLD.Get_rank();
	size = MPI::COMM_WORLD.Get_size();
	if (size < 2) MPI::COMM_WORLD.Abort(1);
	
	MPI_Request array_of_requests[size];

	int max;
			
	if (0 == rank) {
	// master
	
		pline(); cout << "broj elemenata niza: ";
		fflush(stdout);
		cin >> max;
		
		pline(); cout << "ucitaj elemente: ";
		double niz[max];
		//elem nizElem[max];
		for (int i=0; i<max; i++) {
			fflush(stdout);
			cin >> niz[i];
			
			//nizElem[i].num = niz[i];
			//nizElem[i].pos = i;
		}
		
		MPI::COMM_WORLD.Bcast(&max, 1, MPI::INT, 0);
		
		for (int i=1; i < size; i++) {
			MPI::COMM_WORLD.Send(&niz[i], 1, MPI::DOUBLE, i, 0);
		}
		
	/*		
		pocetak = 1;
		MPI::COMM_WORLD.Bcast(&pocetak, 1, MPI::INT, 0);
		
		while (!kraj) {
			elem e;
			MPI::COMM_WORLD.Recv(&e, 1, MPI_Elem, MPI_ANY_SOURCE, MPI_ANY_TAG);
			
			niz[ e.pos ] = e.num;
			
			if (++poslati > max) {
				kraj = 1;
				break;
			}
			MPI::COMM_WORLD.Bcast(&kraj, 1, MPI::INT, 0);
			
			MPI::COMM_WORLD.Send(&nizElem[poslati], 1, MPI_Elem, poslati, 0);
		}
		
		*/
	}
	
	else {
	// slave
	
		MPI::COMM_WORLD.Bcast(&max, 1, MPI::INT, 0);
		if (rank < max) {
		
			double e;
			cout << rank;		
			MPI::COMM_WORLD.Recv(&e, 1, MPI::DOUBLE, 0, 0);
			//cout << e << endl;

			/*
			while (!kraj) {
		
				elem e;
			
				MPI::COMM_WORLD.Bcast(&pocetak, 1, MPI::INT, 0);
			
				if (pocetak) {
					MPI::COMM_WORLD.Recv(&e, 1, MPI_Elem, 0, 0);
					e.num = pow(e.num, 1.0/3.0);
					MPI::COMM_WORLD.Send(&e, 1, MPI_Elem, 0, 0);
				}
				MPI::COMM_WORLD.Bcast(&kraj, 1, MPI::INT, 0);
			}
			*/
		}
		
	}

	MPI::Finalize();
	
	return 0;
}
