#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // MPI-Umgebung initialisieren

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Prozess-Rang (ID)
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // Anzahl der Prozesse

    if (rank == 0) {
        char mess[] = "Hello World";
        int len = strlen(mess)+1;
        float time1 = MPI_Wtime();
        MPI_Send(mess, len, MPI_CHAR, 1, 2, MPI_COMM_WORLD);
        char recv[12];
        MPI_Status status;
        MPI_Recv(recv, 12, MPI_CHAR, 1, 2, MPI_COMM_WORLD, &status);
        float time2 = MPI_Wtime();
        float diff = time2 - time1;
        printf("Recieved Message in (two Way) Time: %f \n", diff);
    } else {
        char recv[12];
        MPI_Status status;
        MPI_Recv(recv, 12, MPI_CHAR, 0, 2, MPI_COMM_WORLD, &status);
        int len = strlen(recv)+1;
        MPI_Send(recv, len, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize(); // MPI-Umgebung schließen
    return 0;
}
