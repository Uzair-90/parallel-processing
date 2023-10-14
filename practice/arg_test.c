#include <stdio.h>
#include <stdlib.h>

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for(int i = 0; i < rows; i++){
    	for(int j = 0; j < cols; j++){
    		printf("%d\t",matrix[i][j]);
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

    int rows, cols;

    fscanf(file, "%d %d", &rows, &cols);

    printf("The number of provided rows and cols on first line: %d, %d\n", rows, cols);

    int **matrix = malloc(rows * sizeof(int *));
    if (matrix == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return 1;
    }

    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            printf("Memory allocation failed!\n");
            // Free allocated memory before exiting
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            fclose(file);
            return 1;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    // Call function to print or process the matrix
    print_matrix(rows, cols, matrix);

    // Free allocated memory
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
    
    fclose(file);

    return 0;
}
