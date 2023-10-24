#include <stdio.h>
#include "/usr/local/opt/libomp/include/omp.h"


int main() {
    #pragma omp parallel
    {
        printf("Hello, World! Thread ID: %d\n", omp_get_thread_num());
    }
    return 0;
}

