#include<stdio.h>
#include<stdlib.h>

/*  
 *                   WHAT IS THIS FILE?????
 *THIS IS A TEST FILE TO SEE IF WE CAN STORE THE PIDS IN A FILE
 *IN ORDER TO FIGURE OUT HOW TO "COMMUNICATE" BETWEEN PROCESSES
 *AND THEN TESTING HOW WE CAN PRINT OUT THE METADATA TO STDOUT
*/

void printPIDS(int);

int main(int argc,char** argv){

    FILE* fp;
    fp = fopen("PIDHOLDER.txt","w");
    int digitLength;

    int initial = getpid();
/*
    printf("parent pid: %d\n",initial);
    
    if( getpid() == initial){
        fprintf(fp,"first: %d\n",getpid());
        fflush(fp);
        printf("Printing initial\n");
    }    
*/
    int f1 = fork();
    int f2 = fork();
    int f3 = fork();

    if(f2 == 0 || f3 == 0){
        f1 = 0;
        f2 =0;
    }
    
    if( getpid() != initial ){
        //printf("Printing not initial\n");
        fprintf(fp,"%d\n",getpid());
        fflush(fp);
    }
   
    fclose(fp);


    if( f1 != 0){
        wait();
    }

    if( f2 != 0){
        wait();
    }

    if( f3 != 0){
        wait();
    }
   
    if( getpid() == initial)
        printPIDS(initial);
 
    return 0;

}

void printPIDS(int initial){
    int counter=0;
    int buffer;
    FILE* fp;


    fp = fopen("PIDHOLDER.txt","r");

    printf("Initial PID: %d\n",initial);
    printf("PIDS of all child processes: ");
    while( fscanf(fp,"%d\n",&buffer)!= EOF ){
        printf("%d,",buffer);
        counter++;
    }
    printf("\n");
    printf("Total number of processes(including initial): %d\n",(counter+1));

    return;
}
