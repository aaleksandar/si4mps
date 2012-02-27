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
	
	int brojVrsta, 
		brojKolona;
		
	if (0 == rank) {
	// master
		pline(); cout << "dimenzije matrice (broj vrsta, broj kolona): ";
		fflush(stdout); cin >> brojVrsta;
		fflush(stdout); cin >> brojKolona;
		
		for (int i=1; i<size; i++) {
			MPI::COMM_WORLD.Send(&brojVrsta, 1, MPI::INT, i, 0);
			MPI::COMM_WORLD.Send(&brojKolona, 1, MPI::INT, i, 0);
		}
		if (brojKolona <= 0 || brojKolona > 100) MPI::COMM_WORLD.Abort(1);
		
		pline(); cout << "ucitaj elemente matrice " << "[" << brojVrsta << "][" << brojKolona << "]" << endl;
		
		MPI::Datatype VRSTA = MPI::DOUBLE.Create_contiguous(brojKolona);
		VRSTA.Commit();
		
		double matrica[brojVrsta][brojKolona];
		
		for (int i=0; i<brojVrsta; i++) {
			for (int j=0; j<brojKolona; j++) {
				fflush(stdout);
				cin >> matrica[i][j];
			}
		}
		
		int toSend[size-1],
			i = 0,
			brojVrstaLeft = brojVrsta;
		for (int i=0; i<size-1; i++) toSend[i] = 0;
		while (brojVrstaLeft > 0) {
			toSend[i++]++;
			i = i % (size-1);
			brojVrstaLeft--;
		}
	
		int vrstaNo = 0;
		for (int i=0; i<size-1; i++) {
			MPI::COMM_WORLD.Send(&toSend[i], 1, MPI::INT, i+1, 0);
		
			for (int j=0; j<toSend[i]; j++)
				MPI::COMM_WORLD.Send(matrica[vrstaNo++], 1, VRSTA, i+1, 0);
		}
		
		VRSTA.Free();
	}
	
	else {
	// slave
		MPI::COMM_WORLD.Recv(&brojVrsta, 1, MPI::INT, 0, 0);
		MPI::COMM_WORLD.Recv(&brojKolona, 1, MPI::INT, 0, 0);
		if (brojKolona <= 0 || brojKolona > 100) MPI::COMM_WORLD.Abort(1);
	
		MPI::Datatype VRSTA = MPI::DOUBLE.Create_contiguous(brojKolona);
		VRSTA.Commit();
		
		int brojVrsta; // prima broj vrsta koji treba da primi
		MPI::COMM_WORLD.Recv(&brojVrsta, 1, MPI::INT, 0, 0);
			
		double vrste[brojVrsta][brojKolona];
		
		for (int i=0; i<brojVrsta; i++) {
			MPI::COMM_WORLD.Recv(vrste[i], 1, VRSTA, 0, 0);
		}

		bool kreni = 1;
		if (rank > 1) MPI::COMM_WORLD.Recv(&kreni, 1, MPI::BOOL, rank-1, 0); // wait prev
		
		for (int j=0; j<brojVrsta; j++) {
			pline();
			for (int i=0; i<brojKolona; i++) cout << vrste[j][i] << " ";
			cout << endl;
		}
		
		if (rank < size-1) MPI::COMM_WORLD.Send(&kreni, 1, MPI::BOOL, rank+1, 0); // start next
		VRSTA.Free();
	}

	MPI::Finalize();
	
	return 0;
}
