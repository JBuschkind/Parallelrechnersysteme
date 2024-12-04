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

//--------------------------------------------------------
int compare (int *A, int size){

	if(size == 1){
		return A[0];
	}
	int Out = new int[(int)ceil(size/2)];
	if(size%2 == 1){
		Out[(int)ceil(size/2)-1] = *A[size-1];
	}
	for( i = 0; i < floor(size/2); i++){
		Out[i*2] = A[i*2] > A[(i*2)+1] ? A[i*2] : A[(i*2)+1];
	}
	
	return compare(*Out,ceil(size/2));
}

int main (int argc, char *argv[]){

  int size = argv[1];
  int A = new int[size];
  cout << "Array Elements: ";
    for (int i = 0; i < size; i++)
        cout << *A[i] << " ";
    cout << endl;
  numberGen(size, 100, A);
  int max = compare(*A, size);
  return 0;
}