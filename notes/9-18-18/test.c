#include<stdio.h>
#include<stdlib.h>
#include "test.h"
#include<unistd.h>

int main(int argc, char * argv[]){



    char* buffer = (char*) malloc(sizeof(char)*20);

    read(STDIN, buffer, 20);
    write(STDOUT,buffer, 20);

    free(buffer);    

    return 0;


/*

   // fprintf(STDOUT,"Stuff");
    char  output[] = "stuff!\n"; //length 9 bytes long because of null terminator
    write(STDOUT,output,9);


    return 0;
*/
/*
    int a = 2;
    int b = 200;
    const int* ptr = &a; // cant change the thing that the ptr is point to because you are declaring it as a constant
                        // however, you can change what the ptr is pointing to
    int * const ptr2 = &a; // for ptr2, you can change what is in the memory address, but you cant change the 
                           // memory address
    const int * const ptr3 = &a; //basically just declaring it as a variable (without either const)

    *ptr;
    *ptr = 3; // this is not allowed because we declared the ptr as constant so cant change what is at the ptr
    
    ptr = &b; // this is allowed as you are just changing what the pointer is pointing to

    return 0;
*/

/*
    if( argc < 3 ){

        printf("FATAL Error in file %s on line %d\n",__FILE__,OUTPUTSTUFF);
        printf("Code must be called with at least 3 arguments, found %d\n", argc);
        return -1;

    }

    return 0;
*/
}
