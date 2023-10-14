#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define num_rows 3
#define num_cols 3

int main(){
	srand(time(NULL));

	int **mat = malloc(num_rows*sizeof(int *));
	int **mat2 = malloc(num_rows*sizeof(int *));
	int **result = malloc(num_rows*sizeof(int *));
	for(int i = 0; i < num_rows; i++){
		mat[i] = malloc(num_cols*sizeof(int));
		mat2[i] = malloc(num_cols*sizeof(int));
		result[i] = malloc(num_cols*sizeof(int));
	}

	if(!mat || !mat2 || !result){
		printf("Memory allocation failed!\n");
	}

	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j++){
			mat[i][j] = rand()%100;
			mat2[i][j] = rand()%100;
			//result[i][j] = rand()%100;

		}
	}

	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_rows; j++){
			for(int k = 0; k < num_rows; k++){
				result[i][j]+=mat[i][k]*mat2[k][j];
			}
		}
	}

	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j++){
			printf("%d\t",mat[i][j]);
		}

		printf("\n");
	}

	printf("\n\n");

	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j++){
			printf("%d\t",mat2[i][j]);
		}

		printf("\n");
	}

	printf("\n\n");
	printf("Multiplication result:\n ");
	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j++){
			printf("%d\t",result[i][j]);
		}

		printf("\n");
	}

	return 0;
}