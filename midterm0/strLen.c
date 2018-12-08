#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char** argv){

    char* str = "ThisIsALongWord";
    char str1[] = "ThisIsALongWord";

    printf("Strlen: %d\n",strlen(str1));
    printf("sizeof: %d\n",sizeof(str1));


    return 0;
}
