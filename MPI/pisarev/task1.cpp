#include <cmath>

class integrator {

 private:
	double (*function) (double);
	double xhi;
	double xlo;

 public:

	void set(double (*func) (double), double XLo, double XHi)	//вводит указатель на функцию, верхнюю и нижнюю границу в класс
	{
		function = func;
		xlo = XLo;
		xhi = XHi;
	}

	double (*get()) (double)	//возвращает указатель на функцию, хранящуюся в классе
	{
		return function;
	}

	double rect(int npoints) {	//интегрирование по числу точек методом прямоугольников
		double sum = 0, L, dx;
		int i;
		L = xhi - xlo;
		dx = L / npoints;
		for (i = 0; i < npoints; i++)
			sum += (*function) (xlo + dx * i) * dx;
		return sum;
	}

	double rect(double epsilon) {	//интегрирование с заданной точностью методом прямоугольников
		double sum = 0, oldSum = 0, L, dx;
		int i, npoints = 10;
		L = xhi - xlo;
		dx = L / npoints;
		for (i = 0; i < npoints; i++)
			sum += (*function) (xlo + dx * i) * dx;

		while (abs(sum - oldSum) > epsilon) {
			npoints *= 2;
			oldSum = sum;
			sum = 0;
			dx = L / npoints;
			for (i = 0; i < npoints; i++)
				sum += (*function) (xlo + dx * i) * dx;
		}
		return sum;
	}

	double trapez(int npoints) {	//интегрирование по числу точек методом трапеций
		double sum = 0, L, dx;
		int i;
		L = xhi - xlo;
		dx = L / npoints;
		sum += 0.5 * ((*function) (xlo) + (*function) (xhi)) * dx;
		for (i = 1; i < npoints; i++)
			sum += (*function) (xlo + dx * i) * dx;
		return sum;
	}

	double trapez(double epsilon) {	//интегрирование с заданной точностью методом трапеций
		double sum = 0, oldSum = 0, L, dx;
		int i, npoints = 10;
		L = xhi - xlo;
		dx = L / npoints;
		sum += 0.5 * ((*function) (xlo) + (*function) (xhi)) * dx;
		for (i = 1; i < npoints; i++)
			sum += (*function) (xlo + dx * i) * dx;
		while (abs(sum - oldSum) > epsilon) {
			npoints *= 2;
			oldSum = sum;
			sum = 0;
			dx = L / npoints;
			sum +=
			    0.5 * ((*function) (xlo) + (*function) (xhi)) * dx;
			for (i = 1; i < npoints; i++)
				sum += (*function) (xlo + dx * i) * dx;
		}
		return sum;
	}

	double simpson(int npoints) {	//интегрирование по числу точек методом парабол
		double sum = 0, L, dx;
		int i;
		L = xhi - xlo;
		dx = L / npoints;
		sum += 1.0 / 6.0 * ((*function) (xlo) +
				    4 * (*function) (xlo + dx / 2.0) +
				    (*function) (xhi)) * dx;
		for (i = 1; i < npoints; i++)
			sum +=
			    1.0 / 3.0 * ((*function) (xlo + dx * i) +
					 2 * (*function) (xlo +
							  dx * (i + 0.5))) * dx;
		return sum;
	}

	double simpson(double epsilon) {	//интегрирование с заданной точностью методом парабол
		double sum = 0, oldSum = 0, L, dx;
		int i, npoints = 10;
		L = xhi - xlo;
		dx = L / npoints;
		sum += 1.0 / 6.0 * ((*function) (xlo) +
				    4 * (*function) (xlo + dx / 2.0) +
				    (*function) (xhi)) * dx;
		for (i = 1; i < npoints; i++)
			sum +=
			    1.0 / 3.0 * ((*function) (xlo + dx * i) +
					 2 * (*function) (xlo +
							  dx * (i + 0.5))) * dx;
		while (abs(sum - oldSum) > epsilon) {
			npoints *= 2;
			oldSum = sum;
			sum = 0;
			dx = L / npoints;
			sum += 1.0 / 6.0 * ((*function) (xlo) +
					    4 * (*function) (xlo + dx / 2.0) +
					    (*function) (xhi)) * dx;
			for (i = 1; i < npoints; i++)
				sum +=
				    1.0 / 3.0 * ((*function) (xlo + dx * i) +
						 2 * (*function) (xlo + dx * (i + 0.5))) * dx;

	}
		return sum;
	}

};
