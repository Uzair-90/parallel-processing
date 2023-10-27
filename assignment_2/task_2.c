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