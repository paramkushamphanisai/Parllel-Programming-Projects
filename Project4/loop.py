import os
for j in range(4,48,4):
    for i in [1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152]:
        os.system("g++ -DNUMT="+str(j)+" -DARRAYSIZE="+str(i)+" Project4Extra.c  -o simd  -lm  -fopenmp")
        os.system("./simd")