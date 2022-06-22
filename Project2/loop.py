import os
for i in [1,2,4,8,16,20,25,30]:
    for n in [10,40,75,100,200,250,275,300]:
        os.system("g++ -O3 -DNUMT="+str(i)+" -DNUMNODES="+str(n)+" Project3.cpp  -o montecarlo  -lm  -fopenmp")
        os.system("./montecarlo")
