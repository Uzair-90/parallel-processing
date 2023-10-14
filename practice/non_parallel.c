#include<stdio.h>
#include<stdlib.h>
#include<time.h>

long long int global = 0;

int main(int argc, char *argv[]){
	time_t start,end;
	time(&start);
	for(long long int i = 0; i < 20000000000;i++){
		global++;
	}
	time(&end);
	printf("Value of global is: %lld\n",global);
	double exec_time = (double)(end - start);
	printf("Time taken: %lf\n", exec_time);


}