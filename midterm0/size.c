#include<stdio.h>
#include<stdlib.h>

struct test{

    char a;
    int val;
    float flot;

};

struct test1{
    char a;
    double dub;
    int in;

};

struct test2{

    double dub;
    int in;
    char c;

};

struct test3{

    char c;
    int in;
    double dub;

};

enum day{monday,tuesday,wednesday,thursday,friday,saturday,sunday};

int main(int argc,char** argv){

    printf("%d\n",sizeof(struct test));
    printf("enum: %d\n",sizeof(enum day));
    printf("test1: %d\n",sizeof(struct test1));
    printf("test2: %d\n",sizeof(struct test2));
    printf("test3: %d\n",sizeof(struct test3));



    return 0;
}
