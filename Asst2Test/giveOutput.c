#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

/*              WHAT IS THIS FILE?????
 * This file is to test searching for a given directory. Whether
 * it be a child or any sort of grandchild of the starting directory.
*/ 



char* recurseFiles(char* basePath, char* givenDir){ // recurse through current dir and try to find given directory
    char* path = (char*)malloc(1000 * sizeof(char));
    DIR *dir = opendir(basePath);
    struct dirent *dp;


    if(!dir) return;

    while((dp = readdir(dir)) != NULL){
        if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
            if(dp->d_type == 4){
                // reset path and do rec call
                // strcpy and strcat
				strcpy(path, basePath);
                if( strcmp(path,"./")!=0)
                    strcat(path, "/");
                strcat(path, dp->d_name);
				if(strcmp(dp->d_name, givenDir) == 0){
					return path;
				}
                
				return recurseFiles(path, givenDir);
            }
        }
    }
    return "fail"; // given dir does not exist; return error
}

int main(int argc, char** argv){	
	char* basePath = "./";
	char* givenDir = "dir1"; // e.g. "dir1"
	printf("%s\n", recurseFiles(basePath, givenDir));
	return 0;
}
