#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
 *                     WHAT IS THIS FILE??????
 *   This file is to test if given a .csv file that it is a valid one
 *   as in it matches the heading as the expected heading. They all
 *   should follow the same data (or a subset). We also are going to
 *   check if each subsequent row has the same number of cols as the
 *   header row.
 *   This file will also check if the name of the .csv contains the word
 *   -sorted- as if so, then we dont need to sort it as it is already sorted
 *   The input for the this is just the name of the csv
 *     
*/

//this accepted headings:
// color, director_name, num_critic_for_reviews, duration, director_facebook_likes,
// actor_3_facebook_likes, actor_2_name, actor_1_facebook_likes, gross, genres, actor_1_name, 
// movie_title, num_voted_users, cast_total_facebook_likes, actor_3_name, facenumber_in_poster, 
// plot_keywords, movie_imdb_link, num_user_for_reviews, language, country, content_rating, 
// budget, title_year, actor_2_facebook_likes, imdb_score, aspect_ratio, movie_facebook_likes

int main(int argc,char** argv){
   
    FILE* fp;
    fp = fopen(argv[1],"r");
    if(fp == NULL){
        printf("File is not inputted correctly\n");
        return 0;
    }

    char* array[]= {"color","director_name","num_critic_for_reviews","duration","director_facebook_likes","actor_3_facebook_likes","actor_2_name","actor_1_facebook_likes","gross","genres","actor_1_name","movie_title","num_voted_users","cast_total_facebook_likes","actor_3_name","facenumber_in_poster","plot_keywords","movie_imdb_link","num_user_for_reviews","language","country","content_rating","budget","title_year","actor_2_facebook_likes","imdb_score","aspect_ratio","movie_facebook_likes"};    

    int counter;
    char line[300];
    fscanf(fp,"%s",&line);
                                        
    if( checkWord(array,line)==1 ){
        if( checkRows(argv[1])){
            if( isSorted(argv[1])==0 ){
                printf("File is valid\n");
            }else printf("File is already sorted\n");
        }else printf("Wrong number of commas in a row\n");
    }else printf("Header col names do not match\n");
    
    fclose(fp);
                                                                    
    return 0;
}

// checks if the .csv is already sorted
// if it is, then its filename would contain
// -sorted-
// return 0 if it is not sorted
// return 1 if it is sorted
int isSorted(char* fileName){

    char* sorted = "-sorted-";

    if( strstr(fileName,sorted) != NULL ){
        // -sorted- exists in the fileName so return 
        return 1;
    }

    return 0;
}

// checks if all of the rows under the header row have
// the same number of cols as the header row (if they do not
// then the file is not valid)
// given the file. 
// return 0 if it is not a valid file
// return 1 if it is a valid file
int checkRows(char* fileName){
    FILE* fp;
    fp = fopen(fileName,"r");
    if(fp == NULL){
        printf("File is not inputted correctly\n");
        return 0;
    }
    // number of commas in the first row
    int commaCount=0;
    // keeps track of the number of commas in the current row
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

    // commaCount now contains the number of commas in the first/header row
    
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

        if( commaCount != count ){
            // the number of commas in this row does not match the number in the header
            return 0;
        }
    }
    
    fclose(fp);
    return 1;
}

// checks if all of the cols in the header row
// matches all of the possible things in the 
// given potential names
// return 0 if it does not belong
// return 1 if it does belong
int checkWord(char* values[],char* line){
    int counter;
    int size = 28;
    const char s[2] = ",";
    int isIn = 0;
    // have to tokenize the line by the commas
    char* token;
    token = strtok(line,s);
    // check if it is empty
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

