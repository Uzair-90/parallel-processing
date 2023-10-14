//matrix multiplication
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

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


void *mat_mul(void *arg){
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
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 100; 
        }
    }
}


void print_mat(int rows, int cols, int (*matrix)[cols]){
	for(int i = 0; i < rows; i++){
		printf("{");
		for(int j = 0; j < cols; j++){
			printf("%d ",matrix[i][j]);
		}
		printf("},\n");
	}
}

int main(int argc, char *argv[]){

	srand(time(NULL));

	time_t start,end;

	pthread_t threads[num_threads];
	arg_t *arg1 = malloc(sizeof(arg_t));
    arg1->rows = num_row;
    arg1->cols = num_col;

    // Allocate memory for matrices
    arg1->matrix_a = malloc(num_row * sizeof(int[num_col]));
    arg1->matrix_b = malloc(num_row * sizeof(int[num_col]));
    arg1->result_mat = malloc(num_row * sizeof(int[num_col]));

    // Initialize matrices using mat_init function
    mat_init(arg1->rows, arg1->cols, arg1->matrix_a);
    mat_init(arg1->rows, arg1->cols, arg1->matrix_b);

    print_mat(arg1->rows, arg1->cols, arg1->matrix_a);
    printf("\n");
    print_mat(arg1->rows, arg1->cols, arg1->matrix_b);



    time(&start);
	for(int i = 0; i < num_threads; i++ ){
		if(i == (num_threads-1)){
			if(pthread_create(&threads[i],NULL,mat_mul,(void *)arg1)!= 0){
				printf("Thread creation failed!\n");
			}
		}
	}

	for(int i = 0; i < num_threads; i++){
		if(i == (num_threads-1)){
			if(pthread_join(threads[i],NULL)!=0){
				printf("Failed to join threads!\n");
			}
		}
	}
	time(&end);

	double timee = end - start;

	printf("\n");
	print_mat(arg1->rows, arg1->cols,arg1->result_mat);
	printf("\n");
	printf("Time taken to completion %lf\n",timee);



	return 0;
}