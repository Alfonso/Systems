#include<stdio.h>
#include<stdlib.h>
#include<errno.h>


int main(int argc,char** argv){

    int fd = open("./test.c",O_RDWR);
    
    printf("%d\n",fd);

    if( fd > 0 ){
        close(fd);
    }else{
        printf("FATAL Error in file %s on line %d:\n%s\n", __FILE__,__LINE__, strerror(errno));
    }

    return 0;
}
