#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define N 200  // ArraySize N

void initialize_array (int n, int max, int *store)
{
    for (int i = 0; i < n; i++){
        store[i] = rand () % max;    
    }
    
}

void search(int I[N], int O[N], int target, int start_elem, int end_elem){
    for(int i = start_elem; i < end_elem; i++){
        if(I[i] == target){
            O[i] = 1;
        }
    }
}

void dump (int *x, int n){
        printf("----------------------------------------\n");
        for (int j = 0; j < n; j++)
            printf("%d ", x[j]);
        printf("\n");
}

int main(int argc, char** argv) {


    int target = 10;

    MPI_Init(&argc, &argv); // MPI-Umgebung initialisieren

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Prozess-Rang (ID)
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Anzahl der Prozesse

    int I[N], O[N];

    if(rank == 0){
        initialize_array(N, 10, I);
    }
    
    for(int i = 0; i < N; i ++){
            O[i] = 0;
    }   

    int elems_per_process = N / size;  
    int start_elem = rank * elems_per_process;
    int end_elem = start_elem + elems_per_process;

    if(rank == 0){
        MPI_Status status;
        //Sending the Message
        for(int i = 1; i < size; i++){
            MPI_Send(I[rank * elems_per_process], elems_per_process, MPI_INT, i, 2, MPI_COMM_WORLD);
        }

        search(I, O, target, start_elem, end_elem);
        
        //Recieving Message
        for(int i = 1; i < size; i++ ){
            MPI_Recv(O[rank * elems_per_process], elems_per_process, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
        }
    }else{
        MPI_Status status;
        MPI_Recv(I[rank * elems_per_process], elems_per_process, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

        search(I, O, target, start_elem, end_elem);

        MPI_Send(O[rank * elems_per_process], elems_per_process, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }

    if(rank == 0){
        dump(I,N);
        dump(O,N);
    }

}