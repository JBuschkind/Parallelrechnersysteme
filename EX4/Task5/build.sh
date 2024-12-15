mpicc -fopenmp -o distributedSearch distributedSearch.c
mpirun -np 4 ./distributedSearch
