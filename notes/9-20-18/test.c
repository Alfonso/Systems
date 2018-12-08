#include<stdio.h>
#include<stdlib.h>

int main(int argc,char** argv){

    const int a=2;
    int* b = &a;
    *b = 3;
    printf("%d\n",a);



    return 0;
}
