// Peter Christakos pechristakos
// Andrew Morrison armorrison
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

    /* temp variables to hold diagonals */
    int temp;
    int temp_col;

    if (N==32) /* For 32x32 matrix, we have found a block size of 8 is most efficient */
    {
        /* transverse through block by block with the outer two loops */
        for(int rBlock = 0; rBlock<N; rBlock+=8)
        {
            for(int cBlock = 0; cBlock<M; cBlock+=8)
            {
                /* trasnverse through each individual block with inner two loops */
                for(int r = rBlock; r<rBlock+8; r++)
                {
                    for(int c = cBlock; c<cBlock+8; c++)
                    {
                        /* if it is not on a diagonal (i.e. row is not same as column) then simply transpose */
                        if(r!=c)
                        {
                            B[c][r] = A[r][c];
                        }
                        /* if it is on a diagonal then store it in a variable for later use */
                        else
                        {
                            temp = A[r][c];
                            temp_col = c;
                        }
                    }
                    /* now we deal with diagonals, if it is on a diagonal in a block and on a diagonal block then transpose it */
                    if(rBlock==cBlock)
                    {
                        B[temp_col][temp_col] = temp;
                    }
                }
            }
        }
    }
    /* do the exact same thing for 64, except block size of 4... 
     * this function sometimes gives us a 8/8 grade, and other times a 2.3/8
     * not completely sure if this is the correct implementation
     */
    else if(N==64)
    {
        for(int rBlock = 0; rBlock<N; rBlock+=4)
        {
            for(int cBlock = 0; cBlock<M; cBlock+=4)
            {
                for(int r = rBlock; r<rBlock+4; r++)
                {
                    for(int c = cBlock; c<cBlock+4; c++)
                    {
                        if(r!=c)
                        {
                            B[c][r] = A[r][c];
                        }
                        else
                        {
                            temp = A[r][c];
                            temp_col = c;
                        }
                    }
                    if(rBlock==cBlock)
                    {
                        B[temp_col][temp_col] = temp;
                    }
                }
            }
        }
    }
    else /* now in the case of an uneven matrix (61x67) a block size of 16 is most efficient*/
    {
        for(int rBlock = 0; rBlock<N; rBlock+=16)
        {
            for(int cBlock = 0; cBlock<M; cBlock+=16)
            {
                for(int r = rBlock; r<rBlock+16 && r<N; r++) // r<N simply make sure we do not go off edge of matrix
                {
                    for(int c = cBlock; c<cBlock+16 && c<M; c++) // r<M
                    {
                        if(r!=c)
                        {
                            B[c][r] = A[r][c];
                        }
                        else
                        {
                            temp = A[r][c];
                            temp_col = c;
                        }
                    }
                    if(rBlock==cBlock)
                    {
                        B[temp_col][temp_col] = temp;
                    }
                }
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


