#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*                          WHAT IS THIS FILE?????
 *This file is just to test the helper function that checks to see if there
 *are duplicate headers in the header row. This is done by tokenizing the 
 *row and checks to see if there are multiple ones in the same row. If there
 *are then it returns -1. 
 *
*/

int duplicateHeader(char*);

int main(int argc,char** argv){

    char* str = "bob,joe,katie,shyam,alfonso,ali";
    printf("str: %s\nduplicate: %d\n",str,duplicateHeader( str ));

    return 1;
}

int duplicateHeader(char* headers){
    char* cpy = (char*)malloc(sizeof(char)*(strlen(headers)+1));
    strcpy(cpy,headers);
    int counter=0;
    int quote=0;
    int commaCount=0;

    for(counter=0;counter<strlen(headers);counter++){
        if(headers[counter] == '"' && quote==0)
            quote = 1;
        if(headers[counter] == '"' && quote==1)
            quote =0;
        if( quote==0 ){
            if(headers[counter] == ',')
                commaCount++;
        }
    }
    char* header[commaCount+1];

    char* ptr = strtok(cpy,",");
    counter =0;

    while(ptr){
        header[counter] = ptr;
        counter++;
        ptr = strtok(NULL,",");
    }

    int counter2=0;
    for(counter=0;counter<commaCount+1;counter++){
        for(counter2=counter+1;counter2<commaCount+1;counter2++){
            if( strcmp( header[counter],header[counter2] ) == 0 )
                return 1;
        }
    }
    free(cpy);

    return 0;
}
