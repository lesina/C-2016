#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {

	int M = 0;
	int N = 10000000;
	int X, Y;
	for (int i = 0; i < N; i++) {
		X = rand()%2 - 1;
		Y = rand()%2 - 1;
		if (X*X + Y*Y <= 1) {
			M++;
		}
	}
	M = (double) M;
	N = (double) N;
	std::cout << 4.0*M/N << std::endl;

	return 0;
}
