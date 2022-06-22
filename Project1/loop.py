import os
for i in [1,2,4,8,12,16,20]:
    for n in [1,10, 100, 1000, 10000,50000, 100000,500000, 1000000]:
        os.system("g++ -O3 -DNUMT="+str(i)+" -DNUMTRIALS="+str(n)+" Project1.cpp  -o montecarlo  -lm  -fopenmp")
        os.system("./montecarlo")