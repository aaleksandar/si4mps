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
	if (size < 2 || size > 32) MPI::COMM_WORLD.Abort(1);
	
	int brojVrsta, 
		brojKolona = size - 1;	
	
	if (0 == rank) {
	// master
		pline(); cout << "unesi broj vrsta matrice: ";
		fflush(stdout); cin >> brojVrsta;
		
		for (int i=1; i<brojKolona+1; i++)
			MPI::COMM_WORLD.Send(&brojVrsta, 1, MPI::INT, i, 0);
		if (brojVrsta <= 0 || brojVrsta > 100) MPI::COMM_WORLD.Abort(1);
		
		pline(); cout << "ucitaj elemente matrice " << "[" << brojVrsta << "][" << brojKolona << "]" << endl;
		
		MPI::Datatype KOLONA = MPI::DOUBLE.Create_contiguous(brojVrsta);
		KOLONA.Commit();
		
		double matrica[brojVrsta][brojKolona];
		
		for (int i=0; i<brojVrsta; i++) {
			for (int j=0; j<brojKolona; j++) {
				fflush(stdout);
				cin >> matrica[i][j];
			}
		}
		
		cout << "matrica, nakon neopisive patnje: " << endl;
		fflush(stdout);
	
		double kolona[brojVrsta];
		
		for (int i=0; i<brojKolona; i++) {
			for (int j=0; j<brojVrsta; j++)
				kolona[j] = matrica[j][i];
			MPI::COMM_WORLD.Send(kolona, 1, KOLONA, i+1, 0);
		}
		
		KOLONA.Free();
	}
	
	else {
	// slave
		MPI::COMM_WORLD.Recv(&brojVrsta, 1, MPI::INT, 0, 0);
		if (brojVrsta <= 0 || brojVrsta > 100) MPI::COMM_WORLD.Abort(1);
	
		MPI::Datatype KOLONA = MPI::DOUBLE.Create_contiguous(brojVrsta);
		KOLONA.Commit();
		
		double kolona[brojVrsta];
		MPI::COMM_WORLD.Recv(kolona, 1, KOLONA, 0, 0);
		
		bool kreni = 1;
		int prev = rank - 1,
			next = rank + 1;
			 
		if (prev == 0) prev = size - 1;
		if (next == size) next = 1;	 
			 
		for (int i=0; i<brojVrsta; i++) {			
			if (rank != 1 || i != 0) { // wait prev
				MPI::COMM_WORLD.Recv(&kreni, 1, MPI::BOOL, prev, 0);
			}
			
			fflush(stdout); // pa zar mora svuda??
			cout << kolona[i] << " "; // ispis
			if (rank == size-1) {
				fflush(stdout);
				cout << endl;
			}
			fflush(stdout);
			
			if (rank != size-1 || i != brojVrsta-1) { // signal next
				MPI::COMM_WORLD.Send(&kreni, 1, MPI::BOOL, next, 0);
			}
		}

		KOLONA.Free();
	}

	MPI::Finalize();
	
	return 0;
}
