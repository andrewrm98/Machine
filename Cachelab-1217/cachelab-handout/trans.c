/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{

    /* This transpose function uses a blocked approach
     * 2D array A will iterated through by blocks which allow it to be more efficient with respect to the cache
     */

    /* Determine block size based on size of 2D array */
    int bSize1 = 8;
    int bSize2 = 8;

    /* Determine the number of blocks */
    int blocks1 = bSize1 * (N/bSize1);
    int blocks2 = bSize2 * (M/bSize2);

    int temp;
    int temp_col;

    /* first two loops will go through each block of A */
    for(int i = 0; i<blocks1; i+=bSize1)
    {
        for(int j = 0; j<blocks2; j+=bSize2)
        {
            /* the inner two loops will iterate through the blocks in A and transpose the values to B */
            for(int k = i; k<i+bSize1 && k<M; k++)
            {
                for(int l = j; l<j+bSize2 && l<N; l++)
                {
                    /* check if it is on a diagonal: if so store in temp variable to deal with later */
                    if(k==l)
                    {
                        temp = A[k][l];
                        temp_col = l;
                    }
                    /* else tranpose to B */
                    else
                    {
                        B[l][k] = A[k][l];
                    }
                }
                /* if a diagonal is found then add it to B here */
                B[temp_col][k] = temp;
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

