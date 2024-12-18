
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <omp.h>

using namespace std;

#define MAXVALUE 100
#define min(A,B) ((A<B) ? A : B)
#define max(A,B) ((A>B) ? A : B)

//------------------------------------
void numberGen (int N, int max, int *store)
{
  int i;
  for (i = 0; i < N; i++)
    store[i] = rand () % max;
}

//--------------------------------------------------------
void dump (double *x, int N, int M)
{
  for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < M; j++)
        cout << x[i * M + j] << " ";
      cout << endl;
    }
  cout << "----------------------------------------" << endl;
}


//--------------------------------------------------------
void mmult (double *A, double *B, double *C, int N, int K, int M)
{ 
    #pragma omp parallel for collapse(2)
    for(int x = 0; x < N; x++){
      for(int y = 0; y < M; y++){
        double temp = 0;      
        for(int z = 0; z < K; z++){
          temp += A[z*N + x] * B[y*M + z];
        }
        C[y*N + x] = temp;
      }
    }
}

//--------------------------------------------------------
int main (int argc, char *argv[])
{
 if (argc != 4)
    {
      cout << "Use : " << argv[0] << " N K M" <<endl;
      exit (1);
    }

  srand (time (0));
  int N = atoi (argv[1]);
  int K = atoi (argv[2]);
  int M = atoi (argv[3]);
  double *A = new double[N * K];
  double *B = new double[K * M];
  double *C = new double[N * M];

  numberGen (N * K, MAXVALUE, A);
  numberGen (K * M, MAXVALUE, B);

  double t = omp_get_wtime ();
;

  mmult (A, B, C, N, K, M);

  cout << omp_get_wtime () - t << endl;

  //dump (A, N, K);
  //dump (B, K, M);
  //dump (C, N, M);


  delete[]A;
  delete[]B;
  delete[]C;
  return 0;
}

