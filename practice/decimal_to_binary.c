#include<stdio.h>
#include<stdlib.h>

int* decimal_to_binary(int number, int *size){
    int temp = number;
    *size = 0;
    while(temp > 0){
        temp /= 2;
        (*size)++;
    }
    
    int *array = malloc((*size) * sizeof(int));
    int i = 0;
    while(number > 0){
        array[i] = number % 2;
        number /= 2;
        i++;
    }
    
    return array;
}

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);
    int size;
    int *array = decimal_to_binary(n, &size);
    for(int i = size - 1; i >= 0; i--){
        printf("%d", array[i]);
    }
    printf("\n");

    free(array); 
    return 0;
}

