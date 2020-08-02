#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	MPI_Init(&argc, &argv);

	int rank = -1, processCount = -1;
	MPI_Comm_size(MPI_COMM_WORLD, &processCount);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Group world_group;
	MPI_Comm_group(MPI_COMM_WORLD, &world_group);
	
	int scope_ranks[27] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};

	MPI_Group scope_group;
	MPI_Group_incl(world_group, 27, scope_ranks, &scope_group);

	MPI_Comm scope_comm;
	MPI_Comm_create(MPI_COMM_WORLD, scope_group, &scope_comm);

	char buf[1];

	int scopeRank = -1, scopeSize = -1;
	
	if (MPI_COMM_NULL != scope_comm) { 	//только scope
		MPI_Comm_rank(scope_comm, &scopeRank);
		MPI_Comm_size(scope_comm, &scopeSize);
		
		if (scopeRank != 0) {
			char letter = 'a' + scopeRank - 1;
			buf[0] = letter - 32;
			cout << "Process " << scopeRank << " sends letter " << char(letter - 32) << endl;
			MPI_Send(buf, 1, MPI_CHAR, 0, 0, scope_comm);
		}
		
		else {
			MPI_Status status;
			char recLetter[27];
			for (int i = 1; i < scopeSize; i++) {	
				MPI_Recv(recLetter + (i - 1), 1, MPI_CHAR, i, 0, scope_comm, &status);
			}
			cout << "Result: ";
			for (int i = 0; i < 27; i++) {
				cout << recLetter[i];
			};
			cout << endl;
		}
	}

	MPI_Group_free(&world_group);
	MPI_Group_free(&scope_group);

	MPI_Finalize();
	return 0;
}
