#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include "lab1_IO.h"


//Global variables
//Initialize input array, output array, size of input and number of threads
int **A;
int **B;
int **C;
int n;
int p;
// https://eclass.srv.ualberta.ca/pluginfile.php/7906242/mod_resource/content/0/Chapter%201.pdf
void *threadfunc(void *rank)
{
    //cast rank from void to int
    int my_rank = (int)rank;
    int i, j, k;
    //Map rank to block row in the matrix
    int x = my_rank / sqrt(p);
    //First row in the respective block
    int my_first_row = x * (n / sqrt(p));
    int my_last_row = (x + 1) * (n / sqrt(p)) - 1;
    //Map rank to block column in the matrix
    int y = (my_rank % (int)sqrt(p));
    //First column in the respective block
    int my_first_col = y * (n / sqrt(p));
    int my_last_col = (y + 1) * (n / sqrt(p)) - 1;

    //Traverse through the block respective to the rank
    for (i = my_first_row; i <= my_last_row; i++)
    {
        for(j = my_first_col; j<=my_last_col; j++){
            C[i][j] = 0;

            // do matrix calculation for determinants 
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
        }

    }
    // fflush(stdout);
    return NULL;
}

int main(int argc, char *argv[])
{
    // load data into program

    int i, j, k;
    FILE *fp;
    int temp, flag = 1;

    Lab1_loadinput(&A, &B, &n);

    //assign memory for output matrix respective to the size of the input matrix
    C = malloc(n * sizeof(int *));
    for (i = 0; i < n; i++)
        C[i] = malloc(n * sizeof(int));
    
    // number of threads from the comnmand line arguments
    p = (int)strtol(argv[1], (char **)NULL, 10);

    printf("Performing matrix multiplication with %d threads\n", p);
    
    //create p number of thread handles
    pthread_t thread_array[p];

    double start, end;
    double elapsed;

    GET_TIME(start);

    for (k = 0; k < p; k++)
    {
        pthread_create(&thread_array[k], NULL, threadfunc, (void *)k);
    }

    //wait for all threads to complete the matrix calculations
    for (k = 0; k < p; k++)
    {
        pthread_join(thread_array[k], NULL);
    }
    GET_TIME(end);
    elapsed = (end - start);


    // save output
    Lab1_saveoutput(C, &n, elapsed);
    // Handle memory leak

    for (i = 0; i < n; i++)
    {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}