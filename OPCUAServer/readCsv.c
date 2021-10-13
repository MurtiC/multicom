#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <unistd.h>
#include <limits.h>

#include <stdio.h>


int readConfigFile(char* fileText,int length,char* path) {
    int pathLength = strlen(path);
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
         printf("OpenConfigRead:getcwd:Faild");
        return -1;
    }
    strcat(cwd,path);
    if (access( cwd, R_OK ) != 0) {
        return -1;
    }
    
    
    FILE* file = fopen(cwd,"r");
    fgets(fileText,length,file);
    fclose(file);
    return 1;
}

int writeConfigFile(char* fileText,char* path) {
    int pathLength = strlen(path);
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
         printf("OpenConfigRead:getcwd:Faild");
        return -1;
    }
    strcat(cwd,path);

    FILE* file = fopen(cwd,"w");
    fputs(fileText,file);
    fclose(file);
    return 1;
}