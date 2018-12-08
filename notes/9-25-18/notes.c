#include<stdio.h>
#include<stdlib.h>

typedef struct _node{
    void* data;
    node* next;
} node;

int comparator_INT(void* data0,void* data1){
    return *((int*) data1) - *((int*)data0);
}

int comparator_STR(void* data0,void* data1){
    return strcmp(*((char*) data1) - *((char*)data0));
}

// return > 0, then data1 > data0
// return < 0, then data1 < data0
// return ==0, then data1 == data0
int mergesort(node* dataList,int (*comparatorFnPtr)(void*,void*)){
    int result = comparatorFnPtr(dataList->data,dataList->next->data);
    return 0;
}

int main(int argc,char** argv){
    
    mergesort(myBigStringList,comparator_STR);
    mergesort(myBigIntegerList,comparator_INT);

    return 0;
}
