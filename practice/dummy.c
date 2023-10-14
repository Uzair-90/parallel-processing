#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows;
    int cols;
    int **mat_a;
    int **mat_b;
    int **result;
} myarg_t;


void print_matrix(int rows, int cols, int **matrix) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Error opening file!\n");
        return 1;
    } else {
        printf("File opened successfully! Reading data from file.\n");
    }

    myarg_t *arg = malloc(sizeof(myarg_t));
    if (arg == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return 1;
    }

    while (!feof(file)) {
        fscanf(file, "%d %d", &(arg->rows), &(arg->cols));
        // Allocate memory for matrices
        arg->mat_a = malloc(arg->rows * sizeof(int *));
        arg->mat_b = malloc(arg->rows * sizeof(int *));
        arg->result = malloc(arg->rows * sizeof(int *));

        for (int i = 0; i < arg->rows; i++) {
            arg->mat_a[i] = malloc(arg->cols * sizeof(int));
            arg->mat_b[i] = malloc(arg->cols * sizeof(int));
            arg->result[i] = malloc(arg->cols * sizeof(int));
        }

        // Read matrix values from the file for mat_a
        for (int i = 0; i < arg->rows; i++) {
            for (int j = 0; j < arg->cols; j++) {
                fscanf(file, "%d", &(arg->mat_a[i][j]));
            }
        }

        // Read matrix values from the file for mat_b
        for (int i = 0; i < arg->rows; i++) {
            for (int j = 0; j < arg->cols; j++) {
                fscanf(file, "%d", &(arg->mat_b[i][j]));
            }
        }

        // Call function to print or process the matrices
        print_matrix(arg->rows, arg->cols, arg->mat_a);
        printf("\n\n");
        print_matrix(arg->rows, arg->cols, arg->mat_b);

    }

    free(arg);
    fclose(file);

    return 0;
}
