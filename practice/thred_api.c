#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include<assert.h>

typedef struct {int a; int b;} myarg_t;
typedef struct {int x; int y;} myret_t;

void *mythread (void *arg) {

myarg_t *arg_c = (myarg_t *)arg;
myret_t *rvals = malloc(sizeof(myret_t));

rvals->x = arg_c->a;
rvals->y = arg_c->b;


return (void *) rvals;

}


int main(int argc, char *argv[]){

pthread_t p;

myret_t *rvals;

myarg_t args = {89,247};

pthread_create(&p,NULL,mythread,&args);
pthread_join(p,(void **) &rvals);

printf("Returened values: %d and %d\n",rvals->x, rvals->y);
free(rvals);



return 0;
}
