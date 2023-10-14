#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define num_col 5
#define num_row 5
#define num_threads 10

typedef struct my_arg {
    int rows;
    int cols;
    int (*matrix_a)[num_col];
    int (*matrix_b)[num_col];
    int (*result_mat)[num_col];
} arg_t;

pthread_mutex_t lock;

void *mat_mul(void *arg) {
    arg_t *args = (arg_t *)arg;

    for (int i = 0; i < args->rows; i++) {
        for (int j = 0; j < args->cols; j++) {
            args->result_mat[i][j] = 0;
            for (int k = 0; k < args->cols; k++) {
                args->result_mat[i][j] += args->matrix_a[i][k] * args->matrix_b[k][j];
            }
        }
    }

    return NULL;
}

void mat_init(int rows, int cols, int (*matrix)[cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

void print_mat(int rows, int cols, int (*matrix)[cols]) {
    for (int i = 0; i < rows; i++) {
        printf("{");
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("}\n");
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    pthread_t threads[num_threads];
    arg_t *args = malloc(sizeof(arg_t));
    args->rows = num_row;
    args->cols = num_col;

    args->matrix_a = malloc(num_row * sizeof(int[num_col]));
    args->matrix_b = malloc(num_row * sizeof(int[num_col]));
    args->result_mat = malloc(num_row * sizeof(int[num_col]));

    mat_init(args->rows, args->cols, args->matrix_a);
    mat_init(args->rows, args->cols, args->matrix_b);

    print_mat(args->rows, args->cols, args->matrix_a);
    printf("\n");
    print_mat(args->rows, args->cols, args->matrix_b);

    pthread_mutex_init(&lock, NULL);

    time_t start, end;
    time(&start);

    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, mat_mul, (void *)args) != 0) {
            printf("Thread creation failed!\n");
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Failed to join threads!\n");
            return 1;
        }
    }

    time(&end);

    double timee = difftime(end, start);

    printf("\n");
    print_mat(args->rows, args->cols, args->result_mat);
    printf("\n");
    printf("Time taken to completion: %lf seconds\n", timee);

    pthread_mutex_destroy(&lock);
    free(args->matrix_a);
    free(args->matrix_b);
    free(args->result_mat);
    free(args);

    return 0;
}
