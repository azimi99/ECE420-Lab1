#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "lab1_IO.h"

int **A;
int **B;
int **C;
int n;
int p;
// https://eclass.srv.ualberta.ca/pluginfile.php/7906242/mod_resource/content/0/Chapter%201.pdf
void *threadfunc(void *rank)
{
    int my_rank = (int)rank;
    int i, j;
    int x = my_rank / sqrt(p);
    int my_first_row = x * (n / sqrt(p));
    int my_last_row = (x + 1) * (n / sqrt(p)) - 1;
    int y = (my_rank % (int)sqrt(p));
    int my_first_col = y * (n / sqrt(p));
    int my_last_col = (y + 1) * (n / sqrt(p)) - 1;

    for (i = my_first_row; i <= my_last_row; i++)
    {
        for(j = my_first_col; j<=my_last_col; j++){
            C[i][j] = 0;

            // do calcs
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
    C = malloc(n * sizeof(int *));
    for (i = 0; i < n; i++)
        C[i] = malloc(n * sizeof(int));
    // partition data
    p = (int)strtol(argv[1], (char **)NULL, 10);

    printf("Performing matrix multiplication with %d threads\n", p);
    pthread_t thread_array[p];

    

    struct timespec start, end;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);
    

    for (k = 0; k < p; k++)
    {
        pthread_create(&thread_array[k], NULL, threadfunc, (void *)k);
    }

    for (k = 0; k < p; k++)
    {
        pthread_join(thread_array[k], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec);
    elapsed += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

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