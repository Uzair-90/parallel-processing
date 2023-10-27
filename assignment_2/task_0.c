#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<time.h>

#define NROWS  8
#define NCOLS  8
#define NUMTHREADS 8

int** mat_alloc(int nrows, int ncols) {

    int **mat = malloc (nrows * sizeof(int*));
    if (mat==NULL) {
        printf("ERROR allocating memory 1\n");
        exit(-1);
    }

    for (int i=0; i<nrows; i++) {
        mat[i] = malloc (ncols * sizeof(int));
        if (mat[i]==NULL) {
            printf("ERROR allocating memory 2\n");
            exit(-1);
        }
    }

    return mat;
}


void matmul (int **a, int **b, int **c, int r_a, int c_a, int r_b, int c_b) {
    // assert (c_a==r_b)
    
    for (int i = 0; i < r_a; i++) {
      for (int j = 0; j < c_b; j++) {
        c[i][j] = 0;
        for (int k = 0; k < c_a; k++) {
          c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
      }
    }
}

typedef struct threadarg {
   int **a;
   int **b;
   int **c;

   int start_row;
   int numrows_a;
   int numcols_a;
   int numcols_b;

} threadarg;

void * matmul_thread (void *arg) {
    threadarg *p = (threadarg *) arg;
    for(int i=p->start_row; i<p->start_row+p->numrows_a; i++)
        for (int j=0; j<p->numcols_b; j++)
            for (int k=0; k<p->numcols_a; k++)
                p->c[i][j] += p->a[i][k]*p->b[k][j];
    return NULL;
}

void mat_init (int **a, int rows, int cols){
    for (int i=0; i<rows; i++)
        for (int j=0; j<cols; j++)
            a[i][j] = 0;
}

void mat_show (int **a, int rows, int cols){
    printf("\n");
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++)
          printf("%d, ", a[i][j]);
        printf("\n");
    }

    printf("\n");
}

int main (int argc, char* argv[]) {

    //setting variable for time
    clock_t _starts,_ends;
    double _cpu_time_s = 0;
    clock_t _startd,_endd;
    double _cpu_time_d = 0;

    //checking arguments errors
    if(argc < 2){
        printf("Not enough arguments!\n");
        return 1;
    }
    
    //checking file error
    FILE *f1 = fopen(argv[1], "r");
    if (f1==NULL) {
        printf("ERROR opening file %s\n", argv[1]);
        exit(-1);
    }

    //initialiazing size variables.
    int numcases = 0;
    int numrows_a = 0;
    int numcols_a = 0;
    int numrows_b = 0;
    int numcols_b = 0;


    //reading size form file.
    fscanf(f1, "%d", &numcases);
    fscanf(f1, "%d %d", &numrows_a, &numcols_a);

    //allocating memory for matrix
    int **a = mat_alloc(numrows_a, numcols_a);

    //reading matrix values from file.
    for (int i=0; i<numrows_a; i++)
        for (int j=0; j<numcols_a; j++)
            fscanf(f1, "%d", &a[i][j]);

    //displaying matrix values.
    mat_show(a, numrows_a, numcols_a);
    printf("\n");

    //rading matrix b size
    fscanf(f1, "%d %d", &numrows_b, &numcols_b);

    //allocating memory for matrix b
    int **b = mat_alloc(numrows_b, numcols_b);

    //reading matrix b values from file.
    for (int i=0; i<numrows_b; i++)
        for (int j=0; j<numcols_b; j++)
            fscanf(f1, "%d", &b[i][j]);

    //displaying matrix b data
    mat_show(b, numrows_b, numcols_b);
    printf("\n");

    //allocating memory for matrix c (result mat).
    int **c = mat_alloc(numrows_a, numcols_b);

    _starts = clock();
    //performing matrix mult without any threads.
    matmul (a, b, c, numrows_a, numcols_a, numrows_b, numcols_b);

    //displaying matrix c
    mat_show(c, numrows_a, numcols_b);
    printf("\n");
    _ends = clock();

    _cpu_time_s = ((double)(_ends - _starts)/CLOCKS_PER_SEC);

    printf("Execution Time Single Thread: %lf (s)\n",_cpu_time_s);



    printf("\n");
    printf("\n");

    //creating NUMTHREADS ids
    /*pthread_t tids[NUMTHREADS];
    //creating thread args
    threadarg args[NUMTHREADS];

    //storing data in args
    for (int i=0; i<NUMTHREADS; i++) {
        args[i].a = a;
        args[i].b = b;
        args[i].c = c;

        args[i].start_row = i*(numrows_a/NUMTHREADS);
        if (i==NUMTHREADS-1)
            args[i].numrows_a = (numrows_a - (i*(numrows_a/NUMTHREADS)));
        else
            args[i].numrows_a = numrows_a/NUMTHREADS;

        args[i].numcols_a = numcols_a;
        args[i].numcols_b = numcols_b;
    }

    //displaying each thread assigned row and location
    for (int i=0; i<NUMTHREADS; i++) {
        printf("%d: %d, %d, %d\n", i, args[i].start_row, args[i].numrows_a, args[i].numcols_a);
    }

    //initializing mat c to 0.
    mat_init(c, numrows_a, numcols_b);

    _startd = clock();
    //creating threads
    for (int i=0; i<NUMTHREADS; i++)
        pthread_create(&tids[i], NULL, matmul_thread, &args[i]);
    //joining threads
    for (int i=0; i<NUMTHREADS; i++)
        pthread_join(tids[i], NULL);
    //displaying threads
    mat_show(c, numrows_a, numcols_b);

    _endd = clock();

    _cpu_time_d = ((double)(_endd - _startd)/CLOCKS_PER_SEC);

    printf("Execution Time %d Thread: %lf (s)\n",NUMTHREADS,_cpu_time_d);*/


    return 0;
}
