#include <iostream>

void better_incrementor(int &x) {
	x++;
}

int main()
{
        int x = 1;
        std::cout << "x initial: " << x << std::endl;
        better_incrementor(x);
        std::cout << "x changed: " << x << std::endl;

        return 0;
}
