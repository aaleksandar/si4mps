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
	
	int offsets[4] = {0, 20, 40, 60};
	int brojOcena[4], brojStudenata;
	
	MPI::Datatype BROJOCENA;
	BROJOCENA = MPI::INT.Create_contiguous(4);
	BROJOCENA.Commit();
	
	MPI::Datatype OCENE;
	
		
	if (0 == rank) {
	// master	
		// init	
		for (int i=0; i<4; i++) {
			pline(); cout << "broj ocena na godini " << i+1 << ".: ";
			fflush(stdout); cin >> brojOcena[i];
			if (brojOcena[i] > 20) brojOcena[i] = 20;
		}
		
		pline(); cout << "broj studenata: ";
		fflush(stdout); cin >> brojStudenata;
		
		int oceneSvihStudenata[brojStudenata][80];
		for (int i=0; i<brojStudenata; i++)
			for (int j=0; j<80; j++) oceneSvihStudenata[i][j] = 0;
		
		for (int i=0; i<brojStudenata; i++) {
			cout << "Student " << i+1 << "." << endl;
			
			for (int j=0; j<4; j++) {
				cout << "-- " <<  j+1 << ". godina: ";
				for (int k=0; k<brojOcena[j]; k++) {
					fflush(stdout); cin >> oceneSvihStudenata[i][20*j + k]; // upisi ocenu
				}
			}
		}
		OCENE = MPI::INT.Create_indexed(4, brojOcena, offsets);
		OCENE.Commit();

		// init [end]
		
		int toSend[size-1],
			i = 0,
			brojStudenataLeft = brojStudenata;
		for (int i=0; i<size-1; i++) toSend[i] = 0;
		while (brojStudenataLeft > 0) {
			toSend[i++]++;
			i = i % (size-1);
			brojStudenataLeft--;
		}
		
		int studentNo = 0;
		for (int i=0; i<size-1; i++) {
			MPI::COMM_WORLD.Send(brojOcena, 1, BROJOCENA, i+1, 0);
			MPI::COMM_WORLD.Send(&toSend[i], 1, MPI::INT, i+1, 0);
	
			for (int j=0; j<toSend[i]; j++) {
				MPI::COMM_WORLD.Send(oceneSvihStudenata[studentNo++], 1, OCENE, i+1, 0);
			}
		}
		
		double prosek;
		brojStudenataLeft = brojStudenata;

		for (int i=0; i<size-1; i++) {
			for (int j=0; j<toSend[i]; j++) {
				MPI::COMM_WORLD.Recv(&prosek, 1, MPI::DOUBLE, i+1, 0);
				pline();
				cout << "Prosek studenta " << i+j+1 << ". je: " << prosek << endl;
				// TODO fix student index
				fflush(stdout);
			}
		}
	}
	
	else {
	// slave
		// init	
		MPI::COMM_WORLD.Recv(brojOcena, 1, BROJOCENA, 0, 0);
		
		int toProcess = 0;
		MPI::COMM_WORLD.Recv(&toProcess, 1, MPI::INT, 0, 0);

		
		OCENE = MPI::INT.Create_indexed(4, brojOcena, offsets);
		OCENE.Commit();
			
		int ocene[80];
		
		double prosek = 0, delilac = 0;
		for (int i=0; i<toProcess; i++) {
			MPI::COMM_WORLD.Recv(ocene, 1, OCENE, 0, 0);
	
			for (int j=0; j<4; j++) {
				for (int k=0; k<brojOcena[j]; k++) {
					prosek += ocene[20*j + k]; // upisi ocenu
				}
				delilac += brojOcena[j];
			}
			prosek /= delilac;
			
			MPI::COMM_WORLD.Send(&prosek, 1, MPI::DOUBLE, 0, 0);
			
			prosek = 0;
			delilac = 0;
		}
	}
	
	BROJOCENA.Free();
	OCENE.Free();

	MPI::Finalize();
	
	return 0;
}
