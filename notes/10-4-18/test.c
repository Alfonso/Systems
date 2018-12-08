#include<limits.h>
#include<dirent.j>
#include "libtest.h"

int main(int argc, char** argv){

    int pid = fork();
    int status = 0;

    if( pid == 0 ){
        execv("./testchild", NULL);
    }

    wait(&status);
    
    return 0;
}
