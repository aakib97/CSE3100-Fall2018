#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "matrix.h"

#define NUM_THREADS 2

// TODO
// Add any needed auxiliary data types and/or functions here
struct thread_data
{
    int	thread_num;
    int row;
    int col;
    TMatrix *m;
    TMatrix *n;
    TMatrix *sum;
};

void* matrix_add(void * threadarg){
    int i,j;

    struct thread_data* thread_data = (struct thread_data*) threadarg;

    for(i = thread_data->thread_num; i < thread_data->row; i+=2){
        for(j = 0; j < thread_data->col; j++){
            thread_data->sum->data[i][j] = thread_data->m->data[i][j] + thread_data->n->data[i][j];
        }
    }

    pthread_exit(NULL);
    return NULL;
}

/* Return the sum of two matrices.
 * Return NULL if anything is wrong.
 *
 * Similar to addMatrix, but with multi-threading.
 */
TMatrix * addMatrix_thread(TMatrix *m, TMatrix *n)
{
   // TODO
   pthread_t thread[NUM_THREADS];
   struct thread_data thread_data[NUM_THREADS];
   int i;

   TMatrix * sumMat = newMatrix(m->nrow,m->ncol);

   for (i = 0; i < NUM_THREADS; i++){
       thread_data[i].thread_num = i;
       thread_data[i].row = m->nrow;
       thread_data[i].col = m->ncol;
       thread_data[i].m = m;
       thread_data[i].n = n;
       thread_data[i].sum = sumMat;
   }

   for (i = 0; i < NUM_THREADS; i++) {
       pthread_create(&thread[i], NULL, matrix_add, &thread_data[i]);
   }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread[i], NULL);
    }

   return sumMat;
}
