#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include<semaphore.h>

pthread_mutex_t buffermutex;
sem_t semEmpty;
sem_t semFull;


#define numthreads 2
#define buffer_size 10

int buffer[buffer_size];
int count = 0;

void *producer(void *arg){
	while(1){
		int x = rand()%100;
		sem_wait(&semEmpty);
		if(count<10){
			pthread_mutex_lock(&buffermutex);
			//producing number between 0 and 99
			//Adding number to the buffer
			buffer[count] = x;
			count++;
			pthread_mutex_unlock(&buffermutex);
		}else{
			printf("Skipped %d\n",x);
		}
		sem_post(&semFull);
	}
	return NULL;
}

void *consumer(void *arg){
	while(1){
		int y = 0; 
		sem_wait(&semFull);
		if(count > 0){
			pthread_mutex_lock(&buffermutex);
			y = buffer[count-1];
			buffer[count-1] = 0;
			count--;
			printf("%d consumed.\n",y);
			//sleep(1);
			pthread_mutex_unlock(&buffermutex);
		}
		sem_post(&semEmpty);
	}

	return NULL;
}

int main(int argc, char *argv[]){
	pthread_mutex_init(&buffermutex,NULL);
	sem_init(&semEmpty,0,10);
	sem_init(&semFull,0,0);
	srand(time(NULL));
	pthread_t threads[numthreads];

	for(int i = 0; i < numthreads; i++){
		if(i == (numthreads-1)){
			if(pthread_create(&threads[i],NULL,&producer,NULL)!=0){
				printf("Thread creating failed!\n");
			}
		} else {
			if(pthread_create(&threads[i],NULL,&consumer,NULL)!=0){
				printf("Thread creation Failed!\n");
			}
		}
	}

	for(int i = 0; i < numthreads; i++){
		if(i == (numthreads-1)){
			if(pthread_join(threads[i],NULL)!=0){
				printf("Thread joining Failed!\n");
			}
		}
	}
	sem_destroy(&semEmpty);
	sem_destroy(&semFull);
	pthread_mutex_destroy(&buffermutex);
	return 0;
}