mpicc -fopenmp -o matterManipulation matterManipulation.c
mpirun -np 4 ./matterManipulation
