#include<stdio.h>
#include<stdlib.h>

int main(int argc,char** argv){

    int aValue = 12;

    int* ptr = (int*)malloc(4*sizeof(int));

    *ptr = aValue;

    return 0;
}
