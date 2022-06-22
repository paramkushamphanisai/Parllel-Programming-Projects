import os
for i in [1024, 4096, 16384, 65536, 262144, 1048576, 2097152, 4194304]:
    for n in [8,32,128]:
        os.system("/usr/local/apps/cuda/cuda-10.1/bin/nvcc -DNUMTRIALS="+str(i)+" -DBLOCKSIZE="+str(n)+" -o proj05  proj05.cu")
        os.system("./proj05")