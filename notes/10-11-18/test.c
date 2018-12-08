#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include<dirent.h>
#include<signal.h>
#include<pthread.h>

void* threadTime(void* args){
    printf("I'm a thread! Look at me!\n");
    
    // since this is a thread, instead of using return (which is for functions)
    // we use this:
    pthread_exit(NULL);
    // we dont really care about returning anyhting back, So return null
}

int main(int argc, char** argv){
    
    pthread_t newThread;
    pthread_attr_t threadAttrStruct;

    pthread_attr_init(&threadAttrStruct);
    
    // since in this case, we do not use the arguments so we are
    // just passing NULL in order to satisfy the parameters
    pthread_create(&newThread, &threadAttrStruct, threadTime, NULL);
    // make sure that the thread completes before returning (and thus getting rid of the thread)
    // SO we use the threads version of wait
    pthread_join(newThread, NULL);

    return 0;
}
