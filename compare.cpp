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
int compare (int *A, size){

	if(size == 1){
		return *A[0];
	}
	int *Out = new int[ceil(size/2)];
	if(length%2 == 1){
		*Out[ceil(size/2)-1] = *A[size-1];
	}
	for( i = 0; i < floor(length/2); i++){
		*Out[i*2] = *A[i*2] > *A[(i*2)+1] ? *A[i*2] : *A[(i*2)+1];
	}
	
	return compare(*Out,ceil(size/2));
}

int main (int argc, char *argv[]){

  int size = argv[1];
  int *A = new int[size];
  cout << "Array Elements: ";
    for (int i = 0; i < size; i++)
        cout << *A[i] << " ";
    cout << endl;
  numberGen (size)
  int max = compare(*A, size);
  return 0;
}