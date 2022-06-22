#!/bin/bash
#SBATCH -J ArrayMul
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1
#SBATCH -o arraymul.out
#SBATCH -e arraymul.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=paramkuv@oregonstate.edu
echo "NMB, LOCAL_SIZE, GigaMultsPerSecond, NUM_WORK_GROUPS"
for nmb in 1024  2048 4096 16384 65536 262144 1048576 2097152 4194304 8388608
do
for localS in 32 128 256
                        do
g++ /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -DNMB=$nmb -DLOCAL_SIZE=$localS -o first first.cpp -lm -fopenmp
                                                            ./first
                                                                    done
                                                                done
