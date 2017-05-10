#!/bin/bash

#PBS -l walltime=00:01:00,nodes=3:ppn=3
#PBS -N ITWORKS!
#PBS -q batch

cd $PBS_O_WORKDIR
mpirun --hostfile $PBS_NODEFILE -np 2 ./life gosperglidergungliderdestruction.cfg
