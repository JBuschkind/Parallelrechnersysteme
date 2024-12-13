#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // MPI-Umgebung initialisieren

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Prozess-Rang (ID)
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Anzahl der Prozesse

    // Case for Coordinator
    if (rank == 0) {

        //Message with Length
        char mess[] = "Hello World";
        int len = strlen(mess)+1;

        //Getting the Time at Send
        float time1 = MPI_Wtime();

        //Sending the Message
        for(i = 1; i < size; i++){
            MPI_Send(mess, len, MPI_CHAR, i, 2, MPI_COMM_WORLD);
        }

        //Preparing to recieve
        char recv[size][12];
        MPI_Status status;

        //Recieving Message
        for(i = 1; i < sizeM; i++ ){
            MPI_Recv(recv[i-1], 12, MPI_CHAR, i, 2, MPI_COMM_WORLD, &status);
        }

        //Gatting Time after Recieving
        float time2 = MPI_Wtime();

        //Calculating the time it took to PingPong
        float diff = time2 - time1;

        //Output
        printf("Recieved Message in (two Way) Time: %f \n", diff);

    //Case for PingPongPartner
    } else {

        //Preparing to recieve
        char recv[12];
        MPI_Status status;

        //Recieving Message
        MPI_Recv(recv, 12, MPI_CHAR, 0, 2, MPI_COMM_WORLD, &status);

        //Preparing to send
        int len = strlen(recv)+1;

        //Sending Message
        MPI_Send(recv, len, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize(); // MPI-Umgebung schließen
    return 0;
}
