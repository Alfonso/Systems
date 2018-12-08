#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<stdbool.h>

#define NUMOFDIRECTORIES 4
#define BREADTHORDEPTH 0            //0 FOR BREADTH 1 FOR DEPTH

inline void swap(char* x,char* y){
    char t = *x; *x=*y;*y=t;
}

char* reverse(char* buffer,int i,int j){
    while(i<j){
        swap(&buffer[i++],&buffer[j--]);
    }

    return buffer;
}

// implementation of itoa
char* itoa(int value,char* buffer,int base){
    int n = abs(value);
    int i = 0;
    
    while(n){
        int r = n %base;
        if(r>=10)
            buffer[i++] = 65+(r-10);
        else buffer[i++] = 48+r;
        n = n/base;
    }    

    if (i == 0)
        buffer[i++] = '0';  

    if(value<0 && base == 10)
        buffer[i++] = '-';

    buffer[i] = '\0';

    return reverse(buffer,0,i-1);

}


int main(int argc,char** argv){

    int counter=0;
    char* path = (char*)malloc(sizeof(char)*1000);    
    char* scounter = (char*)malloc(sizeof(char)*4);

    if( BREADTHORDEPTH == 1 ){
        for(counter=0;counter<NUMOFDIRECTORIES;counter++){
            if(counter == 0)
                strcat(path,"d");
            else strcat(path,"/d");
            itoa(counter,scounter,10);
            strcat(path,scounter);
            mkdir(path,0777);
        }

    }else if( BREADTHORDEPTH == 0 ){
        mkdir("d0",0777);
        for(counter=0;counter<NUMOFDIRECTORIES;counter++){
            itoa(counter,scounter,10);
            strcpy(path,"d0/f");
            strcat(path,scounter);
            strcat(path,".csv");
            FILE* test = fopen(path,"w");
            fclose(test);
        }

    }

    return 1;
}
