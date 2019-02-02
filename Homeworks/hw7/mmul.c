#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "matrix.h"

#define NUM_THREADS 2

struct thread_data
{
    int	num;
    TMatrix *m;
    TMatrix *n;
    TMatrix *mul;
    int row;
    int col;
    int col_2;
};

void* matrix_mul(void * threadarg){
    int i,j,k;

    struct thread_data* thread_data = (struct thread_data*) threadarg;

    for (i = thread_data->num; i < thread_data->row;i+=2)  {
        for (j = 0; j < thread_data->col_2; j++) {
            TElement sum = (TElement) 0;
            for (k = 0; k < thread_data->col; k++) {
                sum += thread_data->m->data[i][k] * thread_data->n->data[k][j];
            }
            thread_data->mul->data[i][j] = sum;
        }
    }

    pthread_exit(NULL);
    return NULL;
}

/* Return the factor of two matrices.
 * Return NULL if anything is wrong.
 *
 * Similar to mulMatrix, but with multi-threading.
 */
TMatrix * mulMatrix_thread(TMatrix *m, TMatrix *n)
{
    pthread_t thread[NUM_THREADS];
    struct thread_data thread_data[NUM_THREADS];
    int i, row, col, col_2;

    row = m->nrow;
    col = m->ncol;
    col_2 = n->ncol;

    TMatrix * mulMat = newMatrix(row,col_2);

    for (i = 0; i < NUM_THREADS; i++){
        thread_data[i].num = i;
        thread_data[i].m = m;
        thread_data[i].n = n;
        thread_data[i].mul = mulMat;
        thread_data[i].row = row;
        thread_data[i].col = col;
        thread_data[i].col_2 = col_2;
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&thread[i], NULL, matrix_mul, &thread_data[i]);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread[i], NULL);
    }

    return mulMat;
}
