/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 

//Name:Lilin    ID:5130379091

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

/*For record:
I took adventages of some codes from the Internet for the (M = 64 && N = 64),
but I worte my code after understanding it instead of just copying it.
And the rest two conditions are solved on my own.*/

char transpose_submit_desc[] = "Transpose submission";
//the basic way to sovle the problem is to divide the whole matrix into blocks
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int bsize = 8;       //block size is 8 * 8

    //deal with 32 * 32
    //just divide it into 16 8*8 blocks and consider the diag elements speaclily
    //misses can reach to 287
    if (M == 32)
    {
        for (int i = 0; i < M; i += bsize) 
        {
            for (int j = 0; j < N; j += bsize) 
            {
                for (int ii = i; ii < i + bsize; ii++) 
                {
                    //here for the elements at the diag
                    //and if not use (int*) ptr,the misses will be up to 340
                    int diag = 0;
                    int *temp = A[ii];
                    if(i == j)
                    { 
                        diag = temp[ii];
                    }
                    //if the elements are not at the diag
                    for (int jj = j; jj < j + bsize; jj++)
                    {
                        if (jj != ii)
                        {
                            B[jj][ii] = A[ii][jj];
                        }
                    }
                    //this positon can't be modified
                    if(i == j)
                    {
                        B[ii][ii] = diag;
                    }
                }
            }
        }
    }

    //deal with the 64 * 64
    //this solution is to set the inside of each block first
    //then set the border of each block at last
    if (M == 64)
    {
        for(int i = 0; i < M; i += bsize)
        {
            for(int j = 0; j < N; j += bsize)
            {
                int k,p0,p1,p2,p3,p4,p5,p6,p7;
                int *temp;
                for(k = 0; k < bsize; k++)
                {
                    //temp[0-3] is the first half of the block
                    temp = &A[j + k][i];
                    p0 = temp[0];
                    p1 = temp[1];
                    p2 = temp[2];
                    p3 = temp[3];
                    //if k != 0,here consider the borders speaclily
                    if(!k)
                    {
                        p4 = temp[4];
                        p5 = temp[5];
                        p6 = temp[6];
                        p7 = temp[7];
                    }
                    //set the elements in B,except the border
                    temp = &B[i][j + k];
                    temp[0] = p0;
                    temp[M] = p1;
                    temp[M*2] = p2;
                    temp[M*3] = p3;
                }
                //just like above,here is the second half of the block
                for(k = bsize-1; k > 0; k--)
                {
                    temp = &A[j+k][i+4];
                    p0 = temp[0];
                    p1 = temp[1];
                    p2 = temp[2];
                    p3 = temp[3];
                    temp = &B[i+4][j+k];
                    temp[0] = p0;
                    temp[M] = p1;
                    temp[M*2] = p2;
                    temp[M*3] = p3;
                }

                //here set border elements of each block
                temp = &B[i + 4][j];
                temp[0] = p4;
                temp[M] = p5;
                temp[M*2] = p6;
                temp[M*3] = p7;
            }
        }
    }

    //deal with the 61 * 67
    //this is like the first situation
    //just divide it into as many 8*8 blocks as we can
    //and consider the rest elements speaclily,misses can reach to 1913
    if (M == 61)
    {
       for(int i = 0; i < M; i += bsize)
       {
            for(int j = 0; j < N; j += bsize)
            {
                for(int jj = j; jj < j + bsize && jj < N; jj++)
                {
                    for(int ii = i; ii < i + bsize && ii < M; ii++)
                    {
                        B[ii][jj] = A[jj][ii];
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

