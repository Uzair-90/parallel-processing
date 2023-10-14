#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define n_threads 10


typedef struct{
	float a;
	float b;
	float result;
} arg_t;



void *my_func(void *arg){

	arg_t *arg1 = (arg_t *)arg;

	arg1->result = (arg1->a+arg1->b)/2;

	//printf("The average of provided numbers is: %f\n",arg1->result);
	
	return (void *)arg1;
}


int main(int argc, char *argv[]){
	pthread_t threads[n_threads];

	arg_t *arg1;
	arg1 = malloc(sizeof(arg_t));

	arg1->a = 10;
	arg1->b = 5;


	for(int i = 0; i < n_threads; i++){
			if(pthread_create(&threads[i],NULL,my_func,(void *)arg1)!=0){
				printf("Thread creation failed.\n");
			}
	}

	for(int i = 0; i < n_threads; i++){
			if(pthread_join(threads[i],(void **)&arg1)!=0){
				printf("Failed to join threads.\n");
			}
			printf("The average is: %f\n",arg1->result);
	}

	return 0;
}