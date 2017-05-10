#!/bin/bash

#PBS -l walltime=00:01:00, nodes = 1:ppn=3
#PBS -N new_job(2)
#PBS -q batch

cd $PBS_O_WORKDIR
mpirun --hostfile ./vdinodes -np 20 ./a.out
