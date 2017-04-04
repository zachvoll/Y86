/*
 * Author: Zachary Vollen
 * A Matrix Wavefront Transport Function
 * 
 * trans.c - Matrix Wavefront A[i][j] = A[i-1][j-1] + A[i-1][j] + A[i][j-1]
 *
 * Each matrix wavefront transport function must have a prototype of the form:
 * void matrix_wavefront(int M, int N, int A[M][N]);
 *
 * A matrix wavefront transport function is evaluated by counting the number of misses
 * on a 512B and 4KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_matrix_wavefront(int M, int N, int A[M][N], int s, int E, int b);


char matrix_wavefront_submit_desc[] = "Matrix Wavefront submission";
void matrix_wavefront_submit(int M, int N, int A[M][N], int s, int E, int b)
{

	//first case: 32 sets, two lines = 64, factorize 255 = 3*5*17, take largest product that's less than 64, so 51
	//second case: 256 sets, two lines, block size = 255

	int jj,ii,j,i;
	if(s==5){
	int bsize = 51;
	int NE = bsize * (N/bsize);
	int ME = bsize * (M/bsize);
	for(ii = 1; ii < ME; ii+=bsize){
		for(jj = 1; jj < NE; jj+=bsize){
			for(i = ii; i < (ii+bsize);i++){
				for(j = jj; j < (jj+bsize);j++){
					A[i][j] = A[i-1][j-1] + A[i-1][j] + A[i][j-1];
				}
			}
		}
	}
	}
	else{
	int bsize = 255;
	int NE = bsize * (N/bsize);
	int ME = bsize * (M/bsize);
	for(ii = 1; ii < ME; ii+=bsize){
		for(jj = 1; jj < NE; jj+=bsize){
			for(i = ii; i < (ii+bsize);i++){
				for(j = jj; j < (jj+bsize);j++){
					A[i][j] = A[i-1][j-1] + A[i-1][j] + A[i][j-1];
				}
			}
		}
	}
	}
}

//matrix_wavefront - A simple baseline matrix wavefront function, not optimized for the cache.
char matrix_wavefront_desc[] = "Simple column-wise matrix wavefront calculations";
void matrix_wavefront(int M, int N, int A[M][N], int s, int E, int b)
{
    int i, j;

    for (j = 1; j < N; j++) {
        for (i = 1; i < M; i++) {
            A[i][j] = A[i-1][j-1] + A[i-1][j] + A[i][j-1];
        }
    }    
}

/*
 *     registerFunctions - This function registers your matrix wavefront
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     matrix wavefront strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(matrix_wavefront_submit, matrix_wavefront_submit_desc); 

    /* Register any additional matrix wavefront functions */
    registerTransFunction(matrix_wavefront, matrix_wavefront_desc); 

}

/* 
 * is_matrix_wavefront - This helper function checks if matrix wavefront A is generated by above
 * definition.
 */
int is_matrix_wavefront(int M, int N, int A[M][N], int s, int E, int b)
{
    int i, j;

    for (i = 1; i < N; i++) {
        for (j = 1; j < M; ++j) {
            if (A[i][j] != (A[i-1][j-1] + A[i-1][j] + A[i][j-1]) ) {
                return 0;
            }
        }
    }
    return 1;
}

