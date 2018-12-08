#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* trim(char*);

int countNonWhite(char*);

int indexOfLastNon(char*);

int main(int argc,char** argv){
    
    char* str = "   test string   ";
    printf("Before trim: %s",str);
    printf(".\n");
    str = trim(str);
    printf(" After trim: %s",str);
    printf(".\n");
   
    return 0;
}

char* trim(char* strPtr){
    char* str = strPtr;
    // keep track of the last non-white character
    int lastNonWhite = indexOfLastNon(str);
    int charCount = countNonWhite(str);
    char* temp = (char*) malloc((charCount+1)*sizeof(char));
    int index = 0; // keep track of curr index
    int oldCount = 0; // keep track of index in old string
    // trim leading white-spaces
    while(*str == ' '){
        str++;
        oldCount++;
    }

    while( oldCount < lastNonWhite ){
        temp[index] = *str;
        index++;
        oldCount++;
        str++;
    }
    temp[index] = '\0';
    
    return temp;
}

int indexOfLastNon(char* str){
    int counter = strlen(str)-1;
    char* end = str + strlen(str) - 1;
    while( *end == ' '){
        counter--;
        end--;
    }
    
    return counter+1;
}

int countNonWhite(char* str){
    int counter=0;    
    char* ptr = str;        
    while( *ptr != '\0' ){
        if( *ptr != ' ' )
            counter++;
        ptr++;
    }

    return counter;
}
