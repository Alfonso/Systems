#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
 *          WHAT IS THIS FILE?????
 *This file basically puts together all of the other
 *files and functions i have written in order to put it
 *in one location. The file handler will be called when
 *a file is found during our "LS" call
 *
*/

void fileHandler(char*,char*,char*,char*);

int isCSV(char*);

int isValidCSV(char*,char*);

int colPresent(char*,char*);

int isSorted(char*);

int checkRows(char*);

int checkWord(char*[],char*);

int main(int argc,char**argv){
    
    // simple testing
    //fileHandler(argv[1],argv[2]);

    return 0;
}

// nameToSort is the name the source file we want to sort
// currentPath is the name of the path to the directory containing
// the source file
// column is the name of the column we want to sort by
// outputDirectory is the name of the directory that we want
// to put all of the sorted .csv files to. If it is equal to "" 
// then we just add it to the directory of the source file
void fileHandler(char* currentPath,char* nameToSort,char* column,char* outputDirectory){

    // check if it is a .csv
    if( isCSV(nameToSort) != 1 ){
        printf("file is not a .csv\n");
        return;
    }

    // check if it is a valid .csv
    if( isValidCSV(nameToSort,column) != 1){
        printf("File is not a valid .csv\n");
        return;
    }
    
    // check to see if the outputDirectory passed is an actual name
    // if not, then it is the path of the current name
    if( outputDirectory == "" ){
        // set output to the current path
        outputDirectory = currentPath;
    }

    // create the fileName
    char* outputFileName = strcat( outputDirectory, "/" );
    outputFileName = strcat( outputFileName, nameToSort );
    outputFileName = strcat( outputFileName, "-sorted-" ); 
    outputFileName = strcat( outputFileName, column);
    outputFileName = strcat( outputFileName, ".csv" );
    // create the actual file
    FILE* fp;
    fp = fopen(outputFileName,"w");
    fclose(fp);

    // run sorter and pass name of the file we are sorting
    // as well as passing the output file.
    
    return;
}

int isCSV(char* fileName){
    int nameLength = strlen(fileName) -1;
    if(fileName[nameLength] != 'v')
        return 0;
    if(fileName[nameLength-1] != 's')
        return 0;
    if(fileName[nameLength-2] != 'c')
        return 0;
    if(fileName[nameLength-3] != '.')
        return 0;

    return 1;
}

int isValidCSV(char* fileName,char* column){
    
    FILE* fp;
    fp = fopen(fileName,"r");
    if(fp == NULL){
        printf("File is not inputed correctly\n");
        return 0;
    }
char* array[]= {"color","director_name","num_critic_for_reviews","duration","director_facebook_likes","actor_3_facebook_likes","actor_2_name","actor_1_facebook_likes","gross","genres","actor_1_name","movie_title","num_voted_users","cast_total_facebook_likes","actor_3_name","facenumber_in_poster","plot_keywords","movie_imdb_link","num_user_for_reviews","language","country","content_rating","budget","title_year","actor_2_facebook_likes","imdb_score","aspect_ratio","movie_facebook_likes"};

    int counter;
    char line[300];
    fscanf(fp,"%s",&line);

    if( checkWord(array,line)==1 ){
        if( checkRows(fileName)){
            if( isSorted(fileName)==0 ){
                if( colPresent(fileName,column)==1){
                    return 1;
                }
            }
        }
    }

    fclose(fp);

    return 0;
}

// checks if the col is present in the header line
// returns 0 if it is not present
// returns 1 if it is present
int colPresent(char* fileName,char* column){

    FILE* fp;
    fp = fopen(fileName,"r");
    if( fp == NULL ){
        printf("File is not inputed correctly\n");
        return 0;
    }

    char line[300];
    fscanf(fp,"%s",&line);
    
    if( strstr(line,column) != NULL )
        return 1;

    return 0;
}

int isSorted(char* fileName){

    char* sorted = "-sorted-";

    if( strstr(fileName,sorted) != NULL ){
        // -sorted- exists in the fileName so return
        return 1;
    }
    
    return 0;
}

int checkRows(char* fileName){
    FILE* fp;
    fp = fopen(fileName,"r");
    if(fp == NULL){
        printf("File is not inputted correctly\n");
        return 0;
    }
    int commaCount=0;
    int count=0;
    int inQuotes = 0;
    int counter=0 ;
    char line[300];
    fscanf(fp,"%s",&line);

    for(counter=0;counter<strlen(line);counter++){
        if(inQuotes == 0){
            if( line[counter] == ',')
                commaCount++;
            else if(line[counter] == '"'){
                inQuotes = 1;
            }
        }else if(inQuotes == 1){
            if(line[counter] == '"')
                inQuotes =0;
        }
    }
    while( fscanf(fp,"%s",&line) != EOF ){
        inQuotes =0;
        count =0;
        for(counter=0;counter<strlen(line);counter++){
            if(inQuotes == 0){
                if( line[counter] == ',')
                    count++;
                else if(line[counter] == '"'){
                    inQuotes = 1;
                }
            }else if(inQuotes == 1){
                if(line[counter] == '"')
                    inQuotes =0;
            }
        }
        if(commaCount != count){
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

int checkWord(char* values[],char* line){
    int counter;
    int size = 28;
    const char s[2] = ",";
    int isIn = 0;
    char* token;
    token = strtok(line,s);
    if(token == NULL)
        return 0;


    while( token!=NULL ){
        isIn = 0;
        for(counter=0;counter<size;counter++){
            if( strcmp(values[counter],token) == 0 ){
                isIn=1;
            }
        }
        if( isIn == 0 ){
            return 0;
        }
        token = strtok(NULL,s);
    }

    return 1;
}
