#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// test value is 8
// Real value is 28
#define NUMOFHEADERS 8
// test value is 16 (including '\0')
// real value is 418
#define COMPLETEHEADERLENGTH 16



/*           WHAT IS THIS FILE???
 *This file is used in order to make sure that the headers
 *of the given list is in order (and their corresponding columns).
 *The order is defined as the order given by Professor in the 
 *project description.
 *ORDER: 
 * color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,
 * actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,
 * cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,
 * num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,
 * imdb_score,aspect_ratio,movie_facebook_likes
*/ 


// this function takes in two char*s. The first char* is the row
// of the CSV that we want to reorder and the char* is the first line
// (the line of the headers in order to allow us to reorder it
// returns the reordered string
char* orderCSV(char*,char*);

// returns the index that the header should be in
int headerTable(char*);

void writeArr(char*[],int);

int findNextIndex(int[],int);

char* betterStrtok(char*,char const*);

char* stringCat(char*[],int);

char* addCommas(char*);

char* addMissing(char*);
    
int main(int argc,char** argv){
    
    //orderCSV("test","color,duration,movie_title,director_name,num_critic_for_reviews,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,aspect_ratio,imdb_score,movie_facebook_likes");

    //printf("ordered: %s\n",orderCSV("1,7,,4,,5,0,","b,h,d,e,c,f,a,g")); 

    char* row = "1,3,5,6,7";
    char* header= "b,d,f,g,h";
    printf("pre-addMissing:\n");
    printf("%s\n",header);
    printf("pre-add commas:\n");
    printf("%s\n",row);
    printf("\n");
    
    row = addCommas(row);
    header = addMissing(header);
   
    printf("pre-order:\n"); 
    printf("%s\n",header);
    printf("%s\n",row);
    printf("\n");
    printf("post-order:\n"); 
    printf("%s\n","a,b,c,d,e,f,g,h");
    printf("%s\n",orderCSV(row,header));

    return 0;
}

// adds the missing headers
char* addMissing(char* row){
    char* result;
    int counter=0;

    char* buff = (char*)malloc(sizeof(char)*(1+strlen(row)));
    // copy over row to buff
    // use the buffer in order to be able to use strcat
    strcpy(buff,row);

    // make first token
    char* ptr= strtok(buff,",");

    //test one with the 8 headers of a b c d e f g h
    if(NUMOFHEADERS == 8){

        char* testHeaders[] = {"a","b","c","d","e","f","g","h"};
        int testHeadersCheck[] = {0,0,0,0,0,0,0,0};
        
        // check which ones are currently present
        while(ptr != NULL){
            testHeadersCheck[ headerTable( ptr ) ] = 1;
            ptr = strtok(NULL,",");
        }

        // initialize result
        result = (char*)malloc(sizeof(char)*COMPLETEHEADERLENGTH);

        // copy old data to result
        strcat(result,row);

        // tack on the missing ones to the end of the thing
        for(counter=0;counter<NUMOFHEADERS;counter++){
            // check to see if the value is there
            if( testHeadersCheck[counter] == 0 ){
                strcat(result,",");
                strcat(result, testHeaders[counter]);
            }
        }
        

    }else if(NUMOFHEADERS == 28){

        char* headers[] = {"color","director_name","duration","director_facebook_likes","actor_3_facebook_likes","actor_2_name","actor_1_facebook_likes","gross","genres","actor_1_name","movie_title","num_voted_users","cast_total_facebook_likes","actor_3_name","facenumber_in_poster","plot_keywords","movie_imdb_link","num_user_for_reviews","language","country","content_rating","budget","title_year","actor_2_facebook_likes","imdb_score","aspect_ratio","movie_facebook_likes"};
        int headersCheck[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        // check which ones are currently present
        while(ptr != NULL){
            headersCheck[ headerTable( ptr ) ] = 1;
            ptr = strtok(NULL,",");
        }

        // initialize result
        result = (char*)malloc(sizeof(char)*COMPLETEHEADERLENGTH);

        // copy old data to result
        strcat(result,row);

        // tack on the missing ones to the end of the result
        for(counter=0;counter<NUMOFHEADERS;counter++){
            // check to see if the value is there
            if( headersCheck[counter] == 0 ){
                strcat(result,",");
                strcat(result, headers[counter]);
            }
        }

    }
    
    return result;
}

// adds the correct number of commas in the row
// returns the updated row
char* addCommas(char* row){
    char* result;
    int counter=0;
    int commaCount = 0;
    // account for commas in quotations
    int quote=0;

    // go through all of the chars and count the commas
    for(counter=0;counter<strlen(row);counter++){
        if(row[counter] == '"' && quote==0)
            quote = 1;
        if(row[counter] == '"' && quote==1)
            quote =0;
        
        if( quote==0 ){
            if(row[counter] == ',')
                commaCount++;
        }

    }

    // check to see if the correct number of commas is there
    // if so, its fine so return initial thing
    if( commaCount == NUMOFHEADERS-1 )
        return row;

    // make the length of the new string (with the additional commas)
    int newLen = strlen(row)+1+( NUMOFHEADERS-1 - commaCount );

    result = (char*)malloc(sizeof(char)*newLen);

    // copy over old row data to new one
    strcat(result,row);
    
    // add the extra commas
    for(counter=0;counter<(NUMOFHEADERS-1-commaCount);counter++){
        strcat(result,",");
    }

    // add the null terminator
    result[newLen]='\0';

    return result;
}

char* orderCSV(char* row,char* headers){
    int counter=0;
    // Have to find the order to reorder the headers
    char* headerArr[NUMOFHEADERS];
    // array of the columns
    char* arr[NUMOFHEADERS];

    // turning the given char* into a char[] in order to use the better tokenizer
    char data[ strlen(row)+1 ];
    for(counter=0;counter<strlen(row);counter++){
        data[counter] = row[counter];
    }
    data[strlen(row)] = '\0';    
    char delim[] = ",";
    counter=0;
    char* test = betterStrtok(data,delim);
    // store all of the tokens into arr
    while(test){
        arr[counter] = test;
        test = betterStrtok(NULL,delim);
        counter++;
    }
    // check the last string in the array. If it is null then change it to ""
    if( counter < NUMOFHEADERS){
        arr[NUMOFHEADERS-1] = ""; 
    }           

    // initializing the buffers
    char* temp = (char*)malloc(sizeof(char)*(strlen(headers)+1));
    char* temp2 = (char*) malloc(sizeof(char)*(strlen(row)+1));

    counter=0;
    // create an arry to keep track of which indices have been visited
    int reached[NUMOFHEADERS];
    for(counter=0;counter<NUMOFHEADERS;counter++){
        reached[counter] = 0;
    }


    // tokenize string
    strcpy(temp,headers);
    char* ptr = strtok(temp,",");
    counter =0;
    // traverse throught the tokens and set them in the array
    while( ptr != NULL ){ 
        headerArr[counter] = ptr;
        ptr = strtok(NULL,",");
        counter++;
    }
/*
    printf("Pre-sort: \n");
    writeArr(headerArr,NUMOFHEADERS);
    printf("\n");
    writeArr(arr,NUMOFHEADERS);    
    printf("\n");
*/
    int index=0;
    temp = headerArr[ 0 ];
    temp2 = arr[ 0 ];
    // time to sort the headerArray. Start with the first one see where it goes
    while( index<NUMOFHEADERS ){
        if( index == headerTable( temp )){
            // we are at the point in the array where the thing should be
            // we need to keep track of which are "done"
            // mark this as finished
            reached[ index ] = 1;
            index = findNextIndex( reached,NUMOFHEADERS );
            temp = headerArr[ index ];
            temp2 = arr[ index ];
            if(index == -1)
                break;
        }else{
            // we are not. So swap the data
            headerArr[ index ] = headerArr[ headerTable( temp ) ];
            arr[ index ] = arr[ headerTable( temp ) ];
            headerArr[ headerTable( temp ) ] = temp;
            arr[ headerTable( temp ) ] = temp2;
            //mark as the spot has been visited
            reached[ headerTable( temp ) ] = 1;
            //make temp the data we just swapped
            temp = headerArr[ index ];
            temp2 = arr[ index ];
        }

 
    }
/*
    printf("\n");
    printf("Post-sort: \n");
    writeArr(headerArr,NUMOFHEADERS);
    printf("\n");
    writeArr(arr,NUMOFHEADERS);    
*/  
    char* result = stringCat(arr,NUMOFHEADERS);
  
    return result;
}

// give an array of strings
// return a string with all of the strings
// concated onto it
char* stringCat(char* arr[],int size){
    // account for number of commas
    int length=size-1;
    int counter;
    for(counter=0;counter<size;counter++){
        length += strlen(arr[counter]);
    }
    // account for '\0'
    length++;
    char* result = (char*)malloc(sizeof(char)*(length));
    for(counter=0;counter<size-1;counter++){
        strcat(result,arr[counter]);
        strcat(result,",");
    }
    strcat(result,arr[counter]);
    
    return result;
}

// a version of strtok that also returns an empty string
// if there is nothing between the delims
char* betterStrtok(char* str,char const* delims){

    static char* src = NULL;
    char* p,* ret = 0;
    
    if(str != NULL)
        src = str;

    if(src == NULL || *src == '\0')
        return NULL;

    ret = src;

    if((p=strpbrk(src, delims)) != NULL){
        *p = 0;
        src = ++p;
    }else if(*src){
        ret = src;
        src = NULL;
    }

    return ret;
}

// returns the index of the next index that has not
// been touched
// returns -1 if done
int findNextIndex(int reached[],int size){
    int counter=0;
    for(counter=0;counter<size;counter++){
        if(reached[counter] == 0)
            return counter;
    }
    return -1;
}

void writeArr(char* arr[],int size){
    int x;
    for(x=0;x<size;x++){
        if(x<size-1)
            printf("%s,",arr[x]);
        else printf("%s",arr[x]);
    }
    printf("\n");

    return;
}

int headerTable(char* header){
if(NUMOFHEADERS == 8){
    if(strcmp(header, "a")==0)
        return 0;
    else if(strcmp(header, "b")==0)
        return 1;
    else if(strcmp(header, "c")==0)
        return 2;
    else if(strcmp(header, "d")==0)
        return 3;
    else if(strcmp(header, "e")==0)
        return 4;
    else if(strcmp(header, "f")==0)
        return 5;
    else if(strcmp(header, "g")==0)
        return 6;
    else if(strcmp(header, "h")==0)
        return 7;
}else if(NUMOFHEADERS == 8){


        if(strcmp(header,"color")==0)
            return 0;
        else if(strcmp(header, "director_name")==0)
            return 1;
        else if(strcmp(header, "num_critic_for_reviews")==0)
            return 2;
        else if(strcmp(header, "duration")==0)
            return 3;
        else if(strcmp(header, "director_facebook_likes")==0)
            return 4;
        else if(strcmp(header, "actor_3_facebook_likes")==0)
            return 5;
        else if(strcmp(header, "actor_2_name")==0)
            return 6;
        else if(strcmp(header, "actor_1_facebook_likes")==0)
            return 7;
        else if(strcmp(header, "gross")==0)
            return 8;
        else if(strcmp(header, "genres")==0)
            return 9;
        else if(strcmp(header, "actor_1_name")==0)
            return 10;
        else if(strcmp(header, "movie_title")==0)
            return 11;
        else if(strcmp(header, "num_voted_users")==0)
            return 12;
        else if(strcmp(header, "cast_total_facebook_likes")==0)
            return 13;
        else if(strcmp(header, "actor_3_name")==0)
            return 14;
        else if(strcmp(header, "facenumber_in_poster")==0)
            return 15;
        else if(strcmp(header, "plot_keywords")==0)
            return 16;
        else if(strcmp(header, "movie_imdb_link")==0)
            return 17;
        else if(strcmp(header, "num_user_for_reviews")==0)
            return 18;
        else if(strcmp(header, "language")==0)
            return 19;
        else if(strcmp(header, "country")==0)
            return 20;
        else if(strcmp(header, "content_rating")==0)
            return 21;
        else if(strcmp(header, "budget")==0)
            return 22;
        else if(strcmp(header, "title_year")==0)
            return 23;
        else if(strcmp(header, "actor_2_facebook_likes")==0)
            return 24;
        else if(strcmp(header, "imdb_score")==0)
            return 25;
        else if(strcmp(header, "aspect_ratio")==0)
            return 26;
        else if(strcmp(header, "movie_facebook_likes")==0)
            return 27;
    }

    // error
    return -1;
}
