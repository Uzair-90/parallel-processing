#include<stdio.h>
#include<stdlib.h>
#include "omp.h"


void hello(void){
    printf("Hello world!\n");
}

int main(int argc, char *argv[]){
    int thread_count = 4;

    #pragma parallel num_threads(thread_count)

    hello();

    return 0;
}