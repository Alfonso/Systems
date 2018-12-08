#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>

int main(int argc,char** argv){
    
    struct dirent something = -1;

    if(dd == NULL){
        printf("KABOOM\n");
        return -1;
    }

    something = readdir(dd);

    if(something == NULL)
        printf("Kaboom #1\n");
        return -1;
    }
    printf("%s\n");

    return 0;
}
