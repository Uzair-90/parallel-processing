#include <stdio.h>
#include <pthread.h>
#include<time.h>

#define NROWS  8
#define NCOLS  8
#define NUMTHREADS 4

int* mat_alloc(int nrows, int ncols) {
    return NULL;
}


void matmul (int a[][NCOLS], int b[][NCOLS], int c[][NCOLS]) {
    for (int i = 0; i < NROWS; i++) {
      for (int j = 0; j < NCOLS; j++) {
        c[i][j] = 0;
        for (int k = 0; k < NCOLS; k++) {
          c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
      }
    }
}

typedef struct threadarg {
   int (*a)[NCOLS];
   int (*b)[NCOLS];
   int (*c)[NCOLS];

   int start_row;
   int num_rows;
   int num_cols;

} threadarg;

void * matmul_thread (void *arg) {
    threadarg *p = (threadarg *) arg;
    for(int i=p->start_row; i<p->start_row+p->num_rows; i++)
        for (int j=0; j<p->num_cols; j++)
            for (int k=0; k<p->num_cols; k++)
                p->c[i][j] += p->a[i][k]*p->b[k][j];

    return NULL;
}

void mat_init (int a[][NCOLS], int rows, int cols){
    for (int i=0; i<rows; i++)
        for (int j=0; j<cols; j++)
            a[i][j] = 0;
}

void mat_show (int a[][NCOLS], int rows, int cols){

    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++)
          printf("%d, ", a[i][j]);
        printf("\n");
    }

    printf("\n\n");
}

int main (int argc, char* argv[]) {

    clock_t start,end;

    double cpu_time_used = 0;

    start = clock();
    
    int a[NROWS][NCOLS] = { {0, 0, 0, 0, 0, 0, 0, 0}, 
                    {1, 1, 1, 1, 1, 1, 1, 1},
                    {2, 2, 2, 2, 2, 2, 2, 2},
                    {3, 3, 3, 3, 3, 3, 3, 3},
                    {4, 4, 4, 4, 4, 4, 4, 4},
                    {5, 5, 5, 5, 5, 5, 5, 5},
                    {6, 6, 6, 6, 6, 6, 6, 6},
                    {7, 7, 7, 7, 7, 7, 7, 7}};

    int b[NROWS][NCOLS] = { {0, 0, 0, 0, 0, 0, 0, 0}, 
                    {1, 1, 1, 1, 1, 1, 1, 1},
                    {2, 2, 2, 2, 2, 2, 2, 2},
                    {3, 3, 3, 3, 3, 3, 3, 3},
                    {4, 4, 4, 4, 4, 4, 4, 4},
                    {5, 5, 5, 5, 5, 5, 5, 5},
                    {6, 6, 6, 6, 6, 6, 6, 6},
                    {7, 7, 7, 7, 7, 7, 7, 7}};

    int c[NROWS][NCOLS] = {0};

    mat_show(a, NROWS, NCOLS);
    mat_show(b, NROWS, NCOLS);
    matmul (a, b, c);
    mat_show(c, NROWS, NCOLS);


    pthread_t tids[NUMTHREADS];
    threadarg args[NUMTHREADS];
    for (int i=0; i<NUMTHREADS; i++) {
        args[i].a = a;
        args[i].b = b;
        args[i].c = c;

        args[i].start_row = i*2;
        args[i].num_rows = 2;
        args[i].num_cols = NCOLS;
    }

    mat_init(c, NROWS, NCOLS);

    for (int i=0; i<NUMTHREADS; i++)
        pthread_create(&tids[i], NULL, matmul_thread, &args[i]);

    for (int i=0; i<NUMTHREADS; i++)
        pthread_join(tids[i], NULL);

    mat_show(c, NROWS, NCOLS);

    end = clock();

    cpu_time_used = ((double) (end - start)/CLOCKS_PER_SEC);

    printf("Execution Time: %lf (s)\n ",cpu_time_used);

    return 0;
}
