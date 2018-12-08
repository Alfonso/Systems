#include<stdio.h>
#include<stdlib.h>

/*
 *                  WHAT IS THIS FILE?????
 *This file is here to test flag formatting and to make sure
 *that we can handle the command input line correctly.
 *It checks if the -c flag is present. If it is not then the 
 *program will turn off as we need it. If it is present then
 *that is the name of the col we are sorting by. It checks if the
 *-d flag is present. If it is not then the program will default
 *to starting the search in the directory the program is in. If is
 *then it will start the sorting in the directory that is flagged.
 *It also checks if the -o flag is present. If it is not then the
 *program will default to putting the output files in the directory
 *of the source file. If it is present then it will put the files
 *in the specified directory.
 *
*/

char* colNameFlag(char**,int);

char* directoryEnd(char**,int);

char* directoryStart(char**,int);

int main(int argc,char** argv){

    int counter;
    char* colName;
    char* directoryName=""; // default to the directory the file is in
    char* outputDirectory=""; // default to the directory of the source file
    int cisPresent=0;

    if( (argc!=3) && (argc!=5) && (argc!=7) ){
        printf("Incorrect number of arguments\n");
        return 0;
    }

    colName = colNameFlag(argv,argc);

    if( strcmp(colName,"") == 0 ){
        printf("-c flag is not present\n");
        return 0;
    }

    directoryName = directoryStart(argv,argc);
    outputDirectory = directoryEnd(argv,argc);

    printf("-c: %s\n",colName);
    printf("-d: %s\n",directoryName);
    printf("-o: %s\n",outputDirectory);

    return 0;
}

char* colNameFlag(char** argv,int argc){
    char* colName="";
    int counter;

    for(counter=0;counter<argc;counter++){
        if( strcmp(argv[counter],"-c") == 0 ){
            // -c is present
            colName = argv[counter+1];
        }
    }

    return colName;
}
char* directoryStart(char** argv,int argc){
    char* directoryName = "./";
    int counter;

    for(counter=0;counter<argc;counter++){
        if(strcmp(argv[counter],"-d") == 0){
            directoryName = argv[counter+1];
        }
    }
    return directoryName;
}

char* directoryEnd(char** argv,int argc){
    char* outputDirectory ="";// needs to default to where the source file is
    int counter;

    for(counter=0;counter<argc;counter++){
        if(strcmp(argv[counter],"-o") ==0){
            outputDirectory = argv[counter+1];
        }
    }

    return outputDirectory;
}
