long length = 0;
long *Array;
double time;
int size = 0, rank = 0;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Status status;

long first = 0;
long plength = length;
long *part = Array;
long fproc = rank;
long lproc = size - 1;
if (rank != 0) {
	MPI_Recv(&first, 1, MPI_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
		 &status);
	MPI_Recv(&plength, 1, MPI_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
		 &status);
	part = new long[plength];
	MPI_Recv(part, plength, MPI_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
		 &status);
	MPI_Recv(&lproc, 1, MPI_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
		 &status);
}
long l = 0;
long r = plength - 1;
long m;
long x = fproc + (lproc - fproc + 1) / 2;
long y = lproc;
while (x > fproc) {
	division(part, l, r, m);
	long pos = 0;
	long count = 0;
	if (m - l + 1 < = r - m) {
		pos = l;
		count = m - l + 1;
		l = m + 1;
	} else {
		pos = m + 1;
		count = r - m;
		r = m;
	}
	MPI_Send(&pos, 1, MPI_LONG, x, 0, MPI_COMM_WORLD);
	MPI_Send(&count, 1, MPI_LONG, x, 0, MPI_COMM_WORLD);
	MPI_Send(part + pos, count, MPI_LONG, x, 0, MPI_COMM_WORLD);
	MPI_Send(&y, 1, MPI_LONG, x, 0, MPI_COMM_WORLD);
	y = x - 1;
	x = fproc + (y - fproc + 1) / 2;
}

qs(part, l, r);
x = fproc + (lproc - fproc + 1) / 2;
while (x > fproc) {
	long pos;
	long count;
	MPI_Recv(&pos, 1, MPI_LONG, x, 0, MPI_COMM_WORLD, NULL);
	MPI_Recv(&count, 1, MPI_LONG, x, 0, MPI_COMM_WORLD, NULL);
	MPI_Recv(part + pos, count, MPI_LONG, x, 0, MPI_COMM_WORLD, NULL);
	x = fproc + (x - fproc) / 2;
}

if (rank != 0) {
	MPI_Send(&first, 1, MPI_LONG, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
	MPI_Send(&plength, 1, MPI_LONG, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
	MPI_Send(part, plength, MPI_LONG, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
	delete[]part;
} else {
	time = (clock() - time) / CLOCKS_PER_SEC;
	//print(Array, length);
	cout << "Sorting time by " << size << ":      " << time << " sec" <<
	    endl;
	delete[]Array;
}

MPI_Finalize();
