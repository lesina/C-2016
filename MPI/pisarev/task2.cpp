#include <iostream>
#include <stdio.h>
#include <cmath>
#include "mpi.h"

class integrator {

 private:
	double (*function) (double);
	double xhi;
	double xlo;

	//следующие данные нужны для распараллеливания методов класса
	int argc;
	char **argv;

 public:

	void set(double (*func) (double), double XLo, double XHi, int argc, char **argv)	//вводит указатель на функцию, верхнюю и нижнюю границу в класс
	{
		function = func;
		xlo = XLo;
		xhi = XHi;
		this->argc = argc;
		this->argv = argv;
	}

	double (*get()) (double)	//возвращает указатель на функцию, хранящуюся в классе
	{
		return function;
	}

	double rect(int npoints) {	//интегрирование по числу точек методом прямоугольников
		double dx;
		int i, rank, size;
		MPI_Status status;
		MPI_Init(&argc, &argv);
		//Определяем свой номер в группе:
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		//Определяем размер группы:
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		dx = (xhi - xlo) / npoints;
		double cur_a, cur_b, d_ba, cur_h;
		if (!rank) {	//Это процесс-мастер
			//Определяем размер диапазона для каждого процесса:
			d_ba = (xhi - xlo) / size;
			//Оставляем первый диапазон для мастера:
			cur_a = xlo + d_ba;
			cur_h = dx;
			//Рассылаем исходные данные подчиненным процессам:
			for (i = 1; i < size; i++) {
				cur_b = cur_a + d_ba;
				MPI_Send(&cur_a, 1, MPI_DOUBLE, i, 98,
					 MPI_COMM_WORLD);
				MPI_Send(&cur_b, 1, MPI_DOUBLE, i, 99,
					 MPI_COMM_WORLD);
				MPI_Send(&cur_h, 1, MPI_DOUBLE, i, 100,
					 MPI_COMM_WORLD);
				cur_a += d_ba;
			}
			cur_a = xlo;
			cur_b = xlo + d_ba;
		} else {	//Это один из подчиненных процессов
			//Получаем исходные данные:
			MPI_Recv(&cur_a, 1, MPI_DOUBLE, 0, 98,
				 MPI_COMM_WORLD, &status);
			MPI_Recv(&cur_b, 1, MPI_DOUBLE, 0, 99,
				 MPI_COMM_WORLD, &status);
			MPI_Recv(&cur_h, 1, MPI_DOUBLE, 0, 100,
				 MPI_COMM_WORLD, &status);
		}

		double s1, sum = 0;

		printf("Process %d. A=%.4f B=%.4f h=%.10f\n",
		       rank, cur_a, cur_b, cur_h);

		for (; cur_a < cur_b; cur_a += cur_h)
			sum += (*function) (cur_a) * cur_h;

		MPI_Barrier(MPI_COMM_WORLD);

		if (!rank) {	//Это процесс-мастер
			//Собираем результаты расчетов:
			for (i = 1; i < size; i++) {
				MPI_Recv(&s1, 1, MPI_DOUBLE, i, 101,
					 MPI_COMM_WORLD, &status);
				sum += s1;
			}
		} else
			//Это подчиненный процесс, отправляем результаты 
			//мастеру:
			MPI_Send(&sum, 1, MPI_DOUBLE, 0, 101, MPI_COMM_WORLD);

		MPI_Finalize();
		if (!rank){
		return sum;
		}

	}

	double rect(double epsilon) {	//интегрирование с заданной точностью методом прямоугольников
		double sum = 0, oldSum = 0, dx, midsum;
		int i, npoints = 10, rank, size;
		double cur_a, cur_b, d_ba, cur_h;
		MPI_Status status;
		dx = (xhi - xlo) / npoints;
		for (i = 0; i < npoints; i++)
			sum += (*function) (xlo + dx * i) * dx;

		MPI_Init(&argc, &argv);
		//Определяем свой номер в группе:
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		//Определяем размер группы:
		MPI_Comm_size(MPI_COMM_WORLD, &size);

		while (true) {
			midsum = 0;
			if (!rank) {	//Это процесс-мастер
				npoints *= 2;
				oldSum = sum;
				sum = 0;
				dx = (xhi - xlo) / npoints;
				//Определяем размер диапазона для каждого процесса:
				d_ba = (xhi - xlo) / size;
				//Оставляем первый диапазон для мастера:
				cur_a = xlo + d_ba;
				cur_h = dx;
				//Рассылаем исходные данные подчиненным процессам:
				for (i = 1; i < size; i++) {
					cur_b = cur_a + d_ba;
					MPI_Send(&cur_a, 1, MPI_DOUBLE, i, 98,
						 MPI_COMM_WORLD);
					MPI_Send(&cur_b, 1, MPI_DOUBLE, i, 99,
						 MPI_COMM_WORLD);
					MPI_Send(&cur_h, 1, MPI_DOUBLE, i, 100,
						 MPI_COMM_WORLD);
					cur_a += d_ba;
				}
				cur_a = xlo;
				cur_b = xlo + d_ba;
			} else {	//Это один из подчиненных процессов
				//Получаем исходные данные:
				MPI_Recv(&cur_a, 1, MPI_DOUBLE, 0, 98,
					 MPI_COMM_WORLD, &status);
				MPI_Recv(&cur_b, 1, MPI_DOUBLE, 0, 99,
					 MPI_COMM_WORLD, &status);
				MPI_Recv(&cur_h, 1, MPI_DOUBLE, 0, 100,
					 MPI_COMM_WORLD, &status);
			}

			double s1;

			printf("Process %d. A=%.4f B=%.4f h=%.10f\n",
			       rank, cur_a, cur_b, cur_h);

			for (; cur_a < cur_b; cur_a += cur_h)
				midsum += (*function) (cur_a) * cur_h;

			MPI_Barrier(MPI_COMM_WORLD);

			if (!rank) {	//Это процесс-мастер
				//Собираем результаты расчетов:
				for (i = 1; i < size; i++) {
					MPI_Recv(&s1, 1, MPI_DOUBLE, i, 101,
						 MPI_COMM_WORLD, &status);
					sum += s1;
				}
				sum += midsum;
				if (std::abs(sum - oldSum) < epsilon) {
					return sum;
				}
			} else
				//Это подчиненный процесс, отправляем результаты 
				//мастеру:
				MPI_Send(&midsum, 1, MPI_DOUBLE, 0, 101,
					 MPI_COMM_WORLD);

		}
		MPI_Finalize();
	}

	double trapez(int npoints) {	//интегрирование по числу точек методом трапеций
		double dx;
		int i, rank, size;
		MPI_Status status;
		MPI_Init(&argc, &argv);
		//Определяем свой номер в группе:
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		//Определяем размер группы:
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		dx = (xhi - xlo) / npoints;
		double cur_a, cur_b, d_ba, cur_h, sum = 0;
		if (!rank) {	//Это процесс-мастер
			//Определяем размер диапазона для каждого процесса:
			d_ba = (xhi - xlo - dx) / size;
			//Оставляем первый диапазон для мастера:
			cur_a = xlo + dx + d_ba;
			cur_h = (xhi - xlo - dx) / npoints;
			//Рассылаем исходные данные подчиненным процессам:
			for (i = 1; i < size; i++) {
				cur_b = cur_a + d_ba;
				MPI_Send(&cur_a, 1, MPI_DOUBLE, i, 98,
					 MPI_COMM_WORLD);
				MPI_Send(&cur_b, 1, MPI_DOUBLE, i, 99,
					 MPI_COMM_WORLD);
				MPI_Send(&cur_h, 1, MPI_DOUBLE, i, 100,
					 MPI_COMM_WORLD);
				cur_a += d_ba;
			}
			cur_a = xlo + dx;
			cur_b = xlo + dx + d_ba;
			sum +=
			    0.5 * ((*function) (xlo) + (*function) (xhi)) * dx;

		} else {	//Это один из подчиненных процессов
			//Получаем исходные данные:
			MPI_Recv(&cur_a, 1, MPI_DOUBLE, 0, 98,
				 MPI_COMM_WORLD, &status);
			MPI_Recv(&cur_b, 1, MPI_DOUBLE, 0, 99,
				 MPI_COMM_WORLD, &status);
			MPI_Recv(&cur_h, 1, MPI_DOUBLE, 0, 100,
				 MPI_COMM_WORLD, &status);
		}

		double s1;

		printf("Process %d. A=%.4f B=%.4f h=%.10f\n",
		       rank, cur_a, cur_b, cur_h);

		for (; cur_a < cur_b; cur_a += cur_h)
			sum += (*function) (cur_a) * cur_h;

		MPI_Barrier(MPI_COMM_WORLD);

		if (!rank) {	//Это процесс-мастер
			//Собираем результаты расчетов:
			for (i = 1; i < size; i++) {
				MPI_Recv(&s1, 1, MPI_DOUBLE, i, 101,
					 MPI_COMM_WORLD, &status);
				sum += s1;
			}
			return sum;
		} else
			//Это подчиненный процесс, отправляем результаты 
			//мастеру:
			MPI_Send(&sum, 1, MPI_DOUBLE, 0, 101, MPI_COMM_WORLD);

		MPI_Finalize();
	}

	double trapez(double epsilon) {	//интегрирование с заданной точностью методом трапеций
		double sum = 0, oldSum = 0, dx, midsum;
		int i, npoints = 10, rank, size;
		double cur_a, cur_b, d_ba, cur_h;
		MPI_Status status;
		dx = (xhi - xlo) / npoints;
		sum += 0.5 * ((*function) (xlo) + (*function) (xhi)) * dx;
		for (i = 1; i < npoints; i++)
			sum += (*function) (xlo + dx * i) * dx;

		MPI_Init(&argc, &argv);
		//Определяем свой номер в группе:
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		//Определяем размер группы:
		MPI_Comm_size(MPI_COMM_WORLD, &size);

		while (true) {
			midsum = 0;
			if (!rank) {	//Это процесс-мастер
				npoints *= 2;
				oldSum = sum;
				dx = (xhi - xlo) / npoints;
				sum = 0;
				//Определяем размер диапазона для каждого процесса:
				d_ba = (xhi - xlo - dx) / size;
				//Оставляем первый диапазон для мастера:
				cur_a = xlo + dx + d_ba;
				cur_h = (xhi - xlo - dx) / npoints;
				//Рассылаем исходные данные подчиненным процессам:
				for (i = 1; i < size; i++) {
					cur_b = cur_a + d_ba;
					MPI_Send(&cur_a, 1, MPI_DOUBLE, i, 98,
						 MPI_COMM_WORLD);
					MPI_Send(&cur_b, 1, MPI_DOUBLE, i, 99,
						 MPI_COMM_WORLD);
					MPI_Send(&cur_h, 1, MPI_DOUBLE, i, 100,
						 MPI_COMM_WORLD);
					cur_a += d_ba;
				}
				cur_a = xlo + dx;
				cur_b = xlo + d_ba + dx;
				sum +=
				    0.5 * ((*function) (xlo) +
					   (*function) (xhi)) * dx;
			} else {	//Это один из подчиненных процессов
				//Получаем исходные данные:
				MPI_Recv(&cur_a, 1, MPI_DOUBLE, 0, 98,
					 MPI_COMM_WORLD, &status);
				MPI_Recv(&cur_b, 1, MPI_DOUBLE, 0, 99,
					 MPI_COMM_WORLD, &status);
				MPI_Recv(&cur_h, 1, MPI_DOUBLE, 0, 100,
					 MPI_COMM_WORLD, &status);
			}

			double s1;

			printf("Process %d. A=%.4f B=%.4f h=%.10f\n",
			       rank, cur_a, cur_b, cur_h);

			for (; cur_a < cur_b; cur_a += cur_h)
				midsum += (*function) (cur_a) * cur_h;

			MPI_Barrier(MPI_COMM_WORLD);

			if (!rank) {	//Это процесс-мастер
				//Собираем результаты расчетов:
				for (i = 1; i < size; i++) {
					MPI_Recv(&s1, 1, MPI_DOUBLE, i, 101,
						 MPI_COMM_WORLD, &status);
					sum += s1;
				}
				sum += midsum;
				if (std::abs(sum - oldSum) < epsilon) {
					return sum;
				}
			} else
				//Это подчиненный процесс, отправляем результаты 
				//мастеру:
				MPI_Send(&midsum, 1, MPI_DOUBLE, 0, 101,
					 MPI_COMM_WORLD);

		}
		MPI_Finalize();
	}

	double simpson(int npoints) {	//интегрирование по числу точек методом парабол
		double dx;
		int i, rank, size;
		MPI_Status status;
		MPI_Init(&argc, &argv);
		//Определяем свой номер в группе:
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		//Определяем размер группы:
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		dx = (xhi - xlo) / npoints;
		double cur_a, cur_a2, cur_b, d_ba, cur_h, sum = 0;
		if (!rank) {	//Это процесс-мастер
			//Определяем размер диапазона для каждого процесса:
			d_ba = (xhi - xlo - dx) / size;
			//Оставляем первый диапазон для мастера:
			cur_a = xlo + dx + d_ba;
			cur_h = (xhi - xlo - dx) / npoints;
			cur_a2 = cur_a + (dx / 2);
			//Рассылаем исходные данные подчиненным процессам:
			for (i = 1; i < size; i++) {
				cur_b = cur_a + d_ba;
				MPI_Send(&cur_a2, 1, MPI_DOUBLE, i, 97,
					 MPI_COMM_WORLD);
				MPI_Send(&cur_a, 1, MPI_DOUBLE, i, 98,
					 MPI_COMM_WORLD);
				MPI_Send(&cur_b, 1, MPI_DOUBLE, i, 99,
					 MPI_COMM_WORLD);
				MPI_Send(&cur_h, 1, MPI_DOUBLE, i, 100,
					 MPI_COMM_WORLD);
				cur_a += d_ba;
				cur_a2 += d_ba;
			}
			cur_a = xlo + dx;
			cur_b = xlo + dx + d_ba;
			cur_a2 = cur_a + (dx / 2);
			sum += 1.0 / 6.0 * ((*function) (xlo) +
					    4 * (*function) (xlo + dx / 2.0) +
					    (*function) (xhi)) * dx;

		} else {	//Это один из подчиненных процессов
			//Получаем исходные данные:
			MPI_Recv(&cur_a2, 1, MPI_DOUBLE, 0, 97,
				 MPI_COMM_WORLD, &status);
			MPI_Recv(&cur_a, 1, MPI_DOUBLE, 0, 98,
				 MPI_COMM_WORLD, &status);
			MPI_Recv(&cur_b, 1, MPI_DOUBLE, 0, 99,
				 MPI_COMM_WORLD, &status);
			MPI_Recv(&cur_h, 1, MPI_DOUBLE, 0, 100,
				 MPI_COMM_WORLD, &status);
		}

		double s1;

		printf("Process %d. A=%.4f B=%.4f h=%.10f\n",
		       rank, cur_a, cur_b, cur_h);

		for (; cur_a < cur_b; cur_a += cur_h) {
			sum += 1.0 / 3.0 * ((*function) (cur_a) +
					    2 * (*function) (cur_a2)) * cur_h;;
			cur_a2 += cur_h;
		}

		MPI_Barrier(MPI_COMM_WORLD);

		if (!rank) {	//Это процесс-мастер
			//Собираем результаты расчетов:
			for (i = 1; i < size; i++) {
				MPI_Recv(&s1, 1, MPI_DOUBLE, i, 101,
					 MPI_COMM_WORLD, &status);
				sum += s1;
			}
			return sum;

		} else
			//Это подчиненный процесс, отправляем результаты 
			//мастеру:
			MPI_Send(&sum, 1, MPI_DOUBLE, 0, 101, MPI_COMM_WORLD);

		MPI_Finalize();
	}

	double simpson(double epsilon) {	//интегрирование с заданной точностью методом парабол
		double sum = 0, oldSum = 0, dx, midsum;
		int i, npoints = 10, rank, size;
		double cur_a, cur_a2, cur_b, d_ba, cur_h;
		MPI_Status status;
		dx = (xhi - xlo) / npoints;
		sum += 0.5 * ((*function) (xlo) + (*function) (xhi)) * dx;
		for (i = 1; i < npoints; i++)
			sum += (*function) (xlo + dx * i) * dx;

		MPI_Init(&argc, &argv);
		//Определяем свой номер в группе:
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		//Определяем размер группы:
		MPI_Comm_size(MPI_COMM_WORLD, &size);

		while (true) {
			midsum = 0;
			if (!rank) {	//Это процесс-мастер
				npoints *= 2;
				oldSum = sum;
				dx = (xhi - xlo) / npoints;
				sum = 0;
				//Определяем размер диапазона для каждого процесса:
				d_ba = (xhi - xlo - dx) / size;
				//Оставляем первый диапазон для мастера:
				cur_a = xlo + dx + d_ba;
				cur_h = (xhi - xlo - dx) / npoints;
				cur_a2 = cur_a + (dx / 2);
				//Рассылаем исходные данные подчиненным процессам:
				for (i = 1; i < size; i++) {
					cur_b = cur_a + d_ba;
					MPI_Send(&cur_a2, 1, MPI_DOUBLE, i, 97,
						 MPI_COMM_WORLD);
					MPI_Send(&cur_a, 1, MPI_DOUBLE, i, 98,
						 MPI_COMM_WORLD);
					MPI_Send(&cur_b, 1, MPI_DOUBLE, i, 99,
						 MPI_COMM_WORLD);
					MPI_Send(&cur_h, 1, MPI_DOUBLE, i, 100,
						 MPI_COMM_WORLD);
					cur_a += d_ba;
					cur_a2 += d_ba;
				}
				cur_a = xlo + dx;
				cur_b = xlo + d_ba + dx;
				cur_a2 = cur_a + (dx / 2);
				sum += 1.0 / 6.0 * ((*function) (xlo) +
						    4 * (*function) (xlo +
								     dx / 2.0) +
						    (*function) (xhi)) * dx;

			} else {	//Это один из подчиненных процессов
				//Получаем исходные данные:
				MPI_Recv(&cur_a2, 1, MPI_DOUBLE, 0, 97,
					 MPI_COMM_WORLD, &status);
				MPI_Recv(&cur_a, 1, MPI_DOUBLE, 0, 98,
					 MPI_COMM_WORLD, &status);
				MPI_Recv(&cur_b, 1, MPI_DOUBLE, 0, 99,
					 MPI_COMM_WORLD, &status);
				MPI_Recv(&cur_h, 1, MPI_DOUBLE, 0, 100,
					 MPI_COMM_WORLD, &status);
			}

			double s1;

			printf("Process %d. A=%.4f B=%.4f h=%.10f\n",
			       rank, cur_a, cur_b, cur_h);

			for (; cur_a < cur_b; cur_a += cur_h) {
				midsum += 1.0 / 3.0 * ((*function) (cur_a) +
						       2 *
						       (*function) (cur_a2)) *
				    cur_h;;
				cur_a2 += cur_h;
			}

			MPI_Barrier(MPI_COMM_WORLD);
			if (!rank) {	//Это процесс-мастер
				//Собираем результаты расчетов:
				for (i = 1; i < size; i++) {
					MPI_Recv(&s1, 1, MPI_DOUBLE, i, 101,
						 MPI_COMM_WORLD, &status);
					sum += s1;
				}
				sum += midsum;
				if (std::abs(sum - oldSum) < epsilon) {
					return sum;
				}
			} else
				//Это подчиненный процесс, отправляем результаты 
				//мастеру:
				MPI_Send(&midsum, 1, MPI_DOUBLE, 0, 101,
					 MPI_COMM_WORLD);

		}
		MPI_Finalize();
	}

};

double func(double x)
{
	return sin(x) * x;
}

int main(int argc, char **argv)
{
	integrator I;
	double (*f) (double) = &func;
	I.set(f, -15, 5, argc, argv);
	double testSum = I.rect(1000);
	std::cout << testSum << std::endl;
	return 0;
}
