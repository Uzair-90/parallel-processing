# Assignment 2 Matrix Optimization

# TASK (0)

Matmul function in c without using any threads:

```c
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
```
Using the above logic for matrix multiplication time of execution for a 10x10 matrix:

Execution Time Single Thread: 0.000057 (s)

For a 100x100 matrix:

Execution Time Single Thread: 0.007827 (s)


# TASK(1)

Function matmul1() using 4 threads for testing excution time:

```c
void * matmul_thread (void *arg) {
    threadarg *p = (threadarg *) arg;
    for(int i=p->start_row; i<p->start_row+p->numrows_a; i++)
        for (int j=0; j<p->numcols_b; j++)
            for (int k=0; k<p->numcols_a; k++)
                p->c[i][j] += p->a[i][k]*p->b[k][j];
    return NULL;
}
```

## Using 4 threads

## Testing time for 10x10 Matrix

Using matmul()1 the programme is taking execution time of 0.000417 (s) using 4 threads. While doing the same task with single thread is: 0.000058 (s).

## Testing time for 100x100 Matrix

Using matmul() function execution time:

* Execution Time Single Thread: 0.007884 (s)
* Execution Time 4 Threads: 0.013317 (s)

## Using 2 threads

For a 10x10 matrix

* Execution Time Single Thread: 0.000042 (s)
* Execution Time 2 Thread: 0.000229 (s)

For a 100x100 matrix

* Execution Time Single Thread: 0.008831 (s)
* Execution Time 2 Thread: 0.009238 (s)

## Using 8 threads

For a 10x10 matrix

* Execution Time Single Thread: 0.000057 (s)
* Execution Time 8 Thread: 0.000642 (s)

For a 100x100 matrix

* Execution Time Single Thread: 0.008735 (s)
* Execution Time 8 Thread: 0.013135 (s)


## Discussion

As we can see that the single thread is performing better than multiple threads in some cases the reason behind this is that there is frequent context switch in multithreaded programms so CPU overhead addsup which increase the execution time of the programme overall.

# TASK(2)

Following is the function matmul2 () code using openMP:

```C
void matmul3(int **a, int **b, int **c, int r_a, int c_a, int r_b, int c_b, int num_threads) {
    // Initialize result matrix
    for (int i = 0; i < r_a; i++) {
        for (int j = 0; j < c_b; j++) {
            c[i][j] = 0;
        }
    }

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Perform matrix multiplication
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < r_a; i++) {
        for (int j = 0; j < c_b; j++) {
            for (int k = 0; k < c_a; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}
```

Using the above code on DA Lab PC's following is the time complexity:

## Results: Using 2 threads:
*   For a 10x10 matrix, the execution time was approximately 0.000 seconds.
*   For a 100x100 matrix, the execution time was approximately 0.008 seconds.
## Using 4 threads:
*   For a 10x10 matrix, the execution time was approximately 0.002 seconds.
*   For a 100x100 matrix, the execution time was approximately 0.005 seconds.
## Using 8 threads:
*   For a 10x10 matrix, the execution time was approximately 0.001 seconds.
*   For a 100x100 matrix, the execution time was approximately 0.01 seconds.



# TASK(3)

Following is the code for tiled algorithm:
```C
void matmul_tiled(int **a, int **b, int **c, int r_a, int c_a, int r_b, int c_b, int tile_size) {
    for (int i = 0; i < r_a; i += tile_size) {
        for (int j = 0; j < c_b; j += tile_size) {
            for (int k = 0; k < c_a; k += tile_size) {
                for (int ii = i; ii < i + tile_size && ii < r_a; ii++) {
                    for (int jj = j; jj < j + tile_size && jj < c_b; jj++) {
                        for (int kk = k; kk < k + tile_size && kk < c_a; kk++) {
                            c[ii][jj] += a[ii][kk] * b[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

```


## For a 10x10 matrix using tiled algorithm keeping the tile size 16:

* Execution Time Single Thread: 0.000122 (s)

## For a 100x100 matrix using tiled algorithm keeping the tile size 16:

* Execution Time Single Thread: 0.009242 (s)


# TASK(4)

Following is the function for multithreaded tiled function:

```C
void matmul5(int **a, int **b, int **c, int r_a, int c_a, int r_b, int c_b, int tile_size, int num_threads) {
    // Initialize result matrix
    for (int i = 0; i < r_a; i++) {
        for (int j = 0; j < c_b; j++) {
            c[i][j] = 0;
        }
    }

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Perform the tiled matrix multiplication
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < r_a; i += tile_size) {
        for (int j = 0; j < c_b; j += tile_size) {
            for (int k = 0; k < c_a; k += tile_size) {
                // Calculate end indices for this tile
                int endRow = i + tile_size > r_a ? r_a : i + tile_size;
                int endCol = j + tile_size > c_b ? c_b : j + tile_size;
                int endK = k + tile_size > c_a ? c_a : k + tile_size;

                // Perform multiplication for this tile
                for (int ii = i; ii < endRow; ii++) {
                    for (int jj = j; jj < endCol; jj++) {
                        int sum = 0;
                        for (int kk = k; kk < endK; kk++) {
                            sum += a[ii][kk] * b[kk][jj];
                        }
                        c[ii][jj] += sum;
                    }
                }
            }
        }
    }
}

```

## Results: Using 2 threads:
*  For a 10x10 matrix, the execution time was approximately 0.000 seconds.
*   For a 100x100 matrix, the execution time was approximately 0.006 seconds.
## Using 4 threads:
*   For a 10x10 matrix, the execution time was approximately 0.000 seconds.
*   For a 100x100 matrix, the execution time was approximately 0.004 seconds.
## Using 8 threads:
*   For a 10x10 matrix, the execution time was approximately 0.002 seconds.
*   For a 100x100 matrix, the execution time was approximately 0.008 seconds.

