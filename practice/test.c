#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int count = 0;

int main(){
	srand(time(NULL));
	int buffer[10];
	for(int i = 0; i < 10; i++){
		buffer[count] = rand()%100;
		count++;
	} 

	for(int i = 0; i < 10; i++){
		printf("%d\t",buffer[i]);
	}
	printf("\n");
	int z = buffer[count -1];
	printf("%d is buffer-1 value.\n", z);
	printf("Current value of count: %d\n",count);
}