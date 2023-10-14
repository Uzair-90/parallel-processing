#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<time.h>

time_t start,end;

pthread_mutex_t lock;
long long int my_int = 0;

void *my_func(void *arg) {
	
    for(long long int i = 0; i < 10000000000; i++) {
     pthread_mutex_lock(&lock);    
        my_int++;
     pthread_mutex_unlock(&lock);   
    }
    return NULL;
}

int main(int argc, char *argv[]) {
	time(&start);
	pthread_mutex_init(&lock,NULL);
    pthread_t p1, p2;
    pthread_create(&p1, NULL, &my_func, NULL);
    pthread_create(&p2, NULL, &my_func, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    //pthread_mutex_lock(&lock);
    printf("The value of my global is: %lld\n", my_int);
    //pthread_mutex_unlock(&lock);
    pthread_mutex_destroy(&lock);
    time(&end);

    double time_taken = (double)(end - start);
    printf("Execution time: %lf\n",time_taken);

    return 0;
}
