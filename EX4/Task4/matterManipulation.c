#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

#define N 5  // Matrixgröße NxM
#define M 5  // Matrixgröße NxM

//------------------------------------
void initialize_matrices (int n, int m, int max, int **store)
{
    for (int i = 0; i < n; i++){
        for(int j = 0; j < m; i++){
            store[i][j] = rand () % max;
        }
    }
    
}

// Hauptfunktion zur Matrixmultiplikation
void matrix_multiply(int A[N][M], int B[M][N], int C[N][N], int start_row, int end_row) {
    #pragma omp parallel for collapse(2)
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < M; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void dump (int *x, int n, int m){
        cout << "----------------------------------------" << endl;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
                cout << x[i][j] << " ";
            cout << endl;
        }
        cout << "----------------------------------------" << endl;
}

int main(int argc, char** argv) {


    
    MPI_Init(&argc, &argv); // MPI-Umgebung initialisieren

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Prozess-Rang (ID)
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Anzahl der Prozesse

    int A[][], B[][], C[][];

    rows_per_process = N / size;  

    if (rank == 0) {
        srand (time (0));
        // Initialisiere Matrizen im Hauptprozess
        initialize_matrices(N, M, 1000, A);
        initialize_matrices(M, N, 1000, B);
    }

    MPI_Bcast(B, M * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, rows_per_process * M, MPI_INT, A[rank * rows_per_process], rows_per_process * M, MPI_INT, 0, MPI_COMM_WORLD);

    matrix_multiply(A, B, C, start_row, end_row);

    MPI_Gather(C[start_row], rows_per_process * M, MPI_INT, C, rows_per_process * M, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        dump(A, N, M);
        dump(B, M, N);
        dump(C, N, N);
    }
    

    MPI_Finalize();
    return 0;
}