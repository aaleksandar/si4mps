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

typedef struct
{
	double r;
	double i;
	int c;
} Complex;

int main(int argc, char* argv[]) {
	MPI::Init(argc, argv);
	
	rank = MPI::COMM_WORLD.Get_rank();
	size = MPI::COMM_WORLD.Get_size();
	if (size < 2) MPI::COMM_WORLD.Abort(1);
	
	int numOfElem;
	
	MPI::Datatype SLICE;
	
		
	if (0 == rank) {
	// master
		pline(); cout << "broj kompleksnih elemenata: "; 
		fflush(stdout); cin >> numOfElem;
		
		Complex complexArray[numOfElem];
		
		for (int i=0; i<numOfElem; i++) {
			pline(); cout << "element " << i << " (realni1, realni2, ceo broj): ";
			fflush(stdout); 
			cin >> complexArray[i].r;
			cin >> complexArray[i].i;
			cin >> complexArray[i].c;
		}
		
		
		int toSend[size-1],
			i = 0,
			numOfElemLeft = numOfElem;
		for (int i=0; i<size-1; i++) toSend[i] = 0;
		while (numOfElemLeft > 0) {
			toSend[i++]++;
			i = i % (size-1);
			numOfElemLeft--;
		}
		
		int position = 0;
		for (int i=0; i<size-1; i++) {			
			MPI::COMM_WORLD.Send(&toSend[i], 1, MPI::INT, i+1, 0);
			
			SLICE = MPI::CHAR.Create_contiguous(toSend[i] * sizeof(Complex));
			SLICE.Commit();
			
			if (toSend[i] == 0) continue;
			
			MPI::COMM_WORLD.Send(&complexArray[position], 1, SLICE, i+1, 0);
			position += toSend[i];
		}
		
		MPI::Datatype COMPLEX = MPI::CHAR.Create_contiguous(sizeof(Complex));
		COMPLEX.Commit();
		
		
		Complex c, max;
		double absMax = 0, tmp;
			
		for (int i=0; i<size-1; i++) {
			for (int j=0; j<toSend[i]; j++) {
				MPI::COMM_WORLD.Recv(&c, 1, COMPLEX, i+1, 0);
				
				if (c.c == 0) { // realni i imaginarni
					tmp = sqrt(c.r*c.r + c.i*c.i);
				}
				else { // moduo i argument
					tmp = c.r * (cos(c.i) + sin(c.i));
				}

				if (abs(tmp) > abs(absMax)) {
					absMax = tmp;
					max = c;
				}
			}
		}
		
		pline();
		cout << "Maksimalni apsolutni kompleksni broj je: " 
			<< max.r
			<< ", "
			<< max.i
			<< ", "
			<< max.c
			<< endl;
		fflush(stdout);

		COMPLEX.Free();	
	}
	
	else {
	// slave
		int toProcess;
		MPI::COMM_WORLD.Recv(&toProcess, 1, MPI::INT, 0, 0);
		
		SLICE = MPI::CHAR.Create_contiguous(toProcess * sizeof(Complex));
		SLICE.Commit();
			
		Complex complexArray[toProcess],
			max;
			
		double absMax = 0;
		
		if (toProcess > 0) {
			MPI::COMM_WORLD.Recv(complexArray, 1, SLICE, 0, 0);
			
			double tmp;
			for (int i=0; i<toProcess; i++) {
				Complex c = complexArray[i];
		
				if (c.c == 0) { // realni i imaginarni
					tmp = sqrt(c.r*c.r + c.i*c.i);
				}
				else { // moduo i argument
					tmp = c.r * (cos(c.i) + sin(c.i));
				}

				if (abs(tmp) > abs(absMax)) {
					absMax = tmp;
					max = c;
				}
			}
		}
		
		if (toProcess > 0) {
			MPI::Datatype COMPLEX = MPI::CHAR.Create_contiguous(sizeof(Complex));
			COMPLEX.Commit();
			for (int i=0; i<toProcess; i++) {
				MPI::COMM_WORLD.Send(&max, 1, COMPLEX, 0, 0);
			}
			COMPLEX.Free();
		}
	}
	
	SLICE.Free();

	MPI::Finalize();
	
	return 0;
}
