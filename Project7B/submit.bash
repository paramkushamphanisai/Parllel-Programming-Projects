#!/bin/bash
#SBATCH -J AutoCorr
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH -N 4 # number of nodes
#SBATCH -n 4 # number of tasks
#SBATCH --constraint=ib
#SBATCH -o autocorr.out
#SBATCH -e autocorr.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=paramkuv@oregonstate.edu
module load openmpi
mpic++ Project7B.cpp -o autocorr -lm
for n in 1 2 3 4 5 6 7 8 
do
mpiexec -mca btl self,tcp -np $n ./autocorr
done