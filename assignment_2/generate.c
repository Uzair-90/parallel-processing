#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL)); // Seed for random number generation

    FILE *f = fopen("input.txt", "w"); // Open the file for writing

    // Write the number of test cases
    fprintf(f, "1\n");

    // First set of 100x100 matrices
    fprintf(f, "100 100\n");
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            fprintf(f, "%d ", rand() % 20); // Generates random numbers between 0 and 19
        }
        fprintf(f, "\n");
    }

    fprintf(f, "100 100\n");
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            fprintf(f, "%d ", rand() % 20); // Generates random numbers between 0 and 19
        }
        fprintf(f, "\n");
    }

    fclose(f); // Close the file

    return 0;
}

