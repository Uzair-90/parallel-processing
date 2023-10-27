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