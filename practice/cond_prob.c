#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>

int fuel = 0;
pthread_mutex_t mutex_fuel;



void *fuel_fill(void *arg){
	for(int i = 0; i < 5; i++){

		pthread_mutex_lock(&mutex_fuel);
		fuel+=15;
		printf("Filling Fuel: %d\n",fuel);
		pthread_mutex_unlock(&mutex_fuel);

	}
	return NULL;
}

void *car(void *arg){
	
	pthread_mutex_lock(&mutex_fuel);

	//this while loop inside the mutex lock will create a deadlock means it will be in
	//waiting state forever as the other thread will wait for this thread to release
	//the lock but this thread will never release the lock which will be running for ever
	//so this is the point where we need to use conditional variable which will signal
	//this loop to wakeup and terminate.
	while(fuel<40){
		printf("No Fuel! Waiting...\n");
		sleep(1);
	}
	fuel-=40;
	printf("Fuel consumption: %d\n",fuel);
	pthread_mutex_unlock(&mutex_fuel);
		
	
	return NULL;
}


int main(int argc, char *argv[]){
	pthread_t threads[2];
	pthread_mutex_init(&mutex_fuel,NULL);
	for(int i = 0; i < 2; i++){
		if(i == 1){
			if(pthread_create(&threads[i],NULL,&fuel_fill,NULL)!=0){
				printf("Failed to create thread...\n");
			}
			else{
				if(pthread_create(&threads[i],NULL,&car,NULL)!=0){
					printf("Failed to create thread...\n");
				}
			}
		}
	}

	for(int i = 0; i < 2; i++){
		if(i == 1){
			if(pthread_join(threads[i],NULL) != 0){
			printf("Failed to join threads...\n");
		}
		}
		
	}
	pthread_mutex_destroy(&mutex_fuel);
	return 0;
}