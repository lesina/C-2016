#include "mpi.h"
#include "stdio.h"
#include  <time.h>

const double a=0.0;//Нижний предел
const double b=100.0;//Верхний предел
const double h=0.000001;//Шаг интегрирования

double fnc(double x)//Интегрируемая функция
{
  return x*x;
}

int main(int argc, char *argv[])
{
	int myrank, ranksize,i;
	clock_t start,finish;
  MPI_Status status;
  MPI_Init(&argc, &argv);//Инициализация MPI
  //Определяем свой номер в группе:
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  //Определяем размер группы:
  MPI_Comm_size(MPI_COMM_WORLD, &ranksize);
  double cur_a,cur_b,d_ba,cur_h;
  if(!myrank)
  {//Это процесс-мастер
    //Определяем размер диапазона для каждого процесса:
    d_ba=(b-a)/ranksize;
    //Оставляем первый диапазон для мастера:
    cur_a=a+d_ba;
    cur_h=h;
start=clock();
    //Рассылаем исходные данные подчиненным процессам:
    for(i=1;i<ranksize;i++)
    {
      cur_b=cur_a+d_ba-h;
      MPI_Send(&cur_a, 1, MPI_DOUBLE, i, 98, 
               MPI_COMM_WORLD);
      MPI_Send(&cur_b, 1, MPI_DOUBLE, i, 99, 
               MPI_COMM_WORLD);
      MPI_Send(&cur_h, 1, MPI_DOUBLE, i, 100, 
               MPI_COMM_WORLD);
      cur_a+=d_ba;
    }
    cur_a=a;cur_b=a+d_ba-h;
  }
  else
  {//Это один из подчиненных процессов
    //Получаем исходные данные:
    MPI_Recv(&cur_a, 1, MPI_DOUBLE, 0, 98, 
             MPI_COMM_WORLD,&status);
    MPI_Recv(&cur_b, 1, MPI_DOUBLE, 0, 99, 
             MPI_COMM_WORLD,&status);
    MPI_Recv(&cur_h, 1, MPI_DOUBLE, 0, 100, 
             MPI_COMM_WORLD,&status);
  }
  //Расчет интеграла в своем диапазоне, выполняют все 
  //процессы:
  double s=0,s1;
  printf("Process %d. A=%.4f B=%.4f h=%.10f\n", 
         myrank,cur_a,cur_b,cur_h);

   for(cur_a+=cur_h;cur_a<=cur_b;cur_a+=cur_h)
    s+=cur_h*fnc(cur_a);

  if(!myrank)
  {//Это процесс-мастер
    //Собираем результаты расчетов:
    for(i=1;i<ranksize;i++)
    {
      MPI_Recv(&s1, 1, MPI_DOUBLE, i, 101, 
               MPI_COMM_WORLD,&status);
      s+=s1;
    }
finish=clock();
    //Печать результата:
    printf("Integral value: %.4f\n",s);
	printf("Time: %.4f\n",(double)(finish-start)/CLOCKS_PER_SEC);
  }
  else
  //Это подчиненный процесс, отправляем результаты 
  //мастеру:
    MPI_Send(&s, 1, MPI_DOUBLE, 0, 101, 
             MPI_COMM_WORLD);
  MPI_Finalize();//Завершение работы с MPI
  return 0;
}
