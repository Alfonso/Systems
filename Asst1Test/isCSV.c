#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
 *          WHAT IS THIS FILE????
 *This file is to test whether or not a given file
 *is a .csv file. It does this by going to the end of
 *the file name and checks to see if it has the .csv flag.
 *Could this potentially not work??
 *Assuming that the input for this specific file is the name of the
 *file we are testing to see if it is a .csv
 *
*/

int main(int argc,char** argv){
    if(argc!=2){
        printf("Argument # incorrect\n");
        return 0;
    }

    if( isCSV(argv[1]) == 1 ){
        printf("File is .csv\n");
    }else printf("File is not a .csv\n");

    return 0;
}

// return 0 if it is not a csv file
// return 1 if it is a csv file
int isCSV(char* fileName){
    int nameLength = strlen(fileName) -1;
    if( fileName[nameLength] != 'v' ){
        return 0;
    }
    if( fileName[nameLength-1] != 's' ){
        return 0;
    }
    if( fileName[nameLength-2] != 'c' ){
        return 0;
    }
    if( fileName[nameLength-3] != '.' ){
        return 0;
    }

    return 1;
}
