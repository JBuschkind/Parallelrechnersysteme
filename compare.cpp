#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <omp.h>
#include <cmath>

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


int main (int argc, char *argv[]){

  int size = argv[1];
  int A = new int[size];
  cout << "Array Elements: ";
    for (int i = 0; i < size; i++)
        cout << *A[i] << " ";
    cout << endl;
  numberGen(size, 100, A);
  
  int global_max = INT_MIN;
  
  #pragma omp parallel
    {
        int local_max = INT_MIN;

        #pragma omp for
        for (int i = 0; i < N; ++i) {
            if (arr[i] > local_max) {
                local_max = arr[i];
            }
        }

        #pragma omp critical
        {
            if (local_max > global_max) {
                global_max = local_max;
            }
        }
    }
	cout << "Der maximale Wert im Array ist: " << global_max << endl;
  return 0;
}