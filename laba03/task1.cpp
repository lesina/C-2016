#include<iostream>

void bad_incrementor(int x)
{
        x++;
        std::cout << "x changed: " << x << std::endl;
}

int main() {
	int a;
	std::cin >> a;
	bad_incrementor(a);
	std::cout << "final x: " << a << std::endl;
	return 0;
}
