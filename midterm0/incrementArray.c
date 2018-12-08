#include<stdio.h>
#include<stdlib.h>

int main(int argc,char** argv){

    int someArray[5] = {1,2,3,4,5};
    int incrementArray[5] = {0,0,0,0,0};

    int i =0;
    
    while(i < 5){

        incrementArray[i] = someArray[i]+1;
        printf("%d %d\n",incrementArray[i], someArray[i]);
        i++;

    }

    printf("%d\n",*(someArray+1));

    return 0;
}
