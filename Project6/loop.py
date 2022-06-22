import os
for i in [1024,2048,4096,8192,16384,32768,65536,131072,131072*2,131072*2,131072*2*2,131072*2*2*2*2,131072*2*2*2*2*2,131072*2*2*2*2*2*2,131072*2*2*2*2*2*2*2 ]:
    for n in [32,64,128,256]:
        os.system("g++ first.cpp -DLOCAL_SIZE="+str(n)+" -DNMB="+str(i)+" /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -o printinfo -lm -fopenmp")
        os.system("./printinfo")