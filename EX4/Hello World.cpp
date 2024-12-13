#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // MPI-Umgebung initialisieren

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // Prozess-Rang (ID)
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // Anzahl der Prozesse

    printf("Hello from process %d out of %d\n", world_rank, world_size);

    MPI_Finalize(); // MPI-Umgebung schließen
    return 0;
}
