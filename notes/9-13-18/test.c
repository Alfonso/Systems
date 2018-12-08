#include<stdlib.h>
#include <unistd.h>
#include<stdio.h>
#include "test.h"

int printStuff(int a){

	printf("%d\n",a);
	return 0;

}

int main(int argc, char* argv[]){

	int (*functionPtr) (int) = printStuff;	
	
	functionPtr(5);

	return 0;

/*

	int array[] = {1, 2, 3, 4, 5};

	int* ptr = array+2;

	printf("%d\n",ptr[0]);

	return 0;


	if(0 ISZERO){

		printf("%d\n", SQUARE(5+5));

	}

	return 0;


	//char array[5];
	

	int* someInts = (int*) malloc(sizeof(int)*10);

	int* newSpace = (int*)malloc(sieof(int)*12);
	memcpy(newSpace. someInts, sizeof(int)*10);
	free(someInts);

	//someInts[3] = 300;
	int* newSpace = (int*)realloc(someInts,sizeof(int)*12);
	
	//while(reading in chars....)
	char* new = (char*) -1;
	bufferLength +=1;
	char* new = (char*)realloc(buffer,bufferLength*sizeof(char));
	if(new == -1 || new == NULL){

		Oh Noes!

	}

	free(newSpace);
	if(someInts == NULL){
		printf("ERROR: malloc broke - oh noes! \n");
		return -1;
	}
*/
	
}
