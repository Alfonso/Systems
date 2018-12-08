#include<stdio.h>
#include<stdlib.h>
#define MAXBUFF 1000

int main(int argc,char** argv){
    
    char* str = malloc(sizeof(char));
    while(fgets(str,MAXBUFF,stdin) != NULL ){
        printf("The input is: %s\n",str);
    }

    return 0;
}
