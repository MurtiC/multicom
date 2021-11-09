#ifndef readFile_C
    #define readFile_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <limits.h>

#include <stdio.h>


#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#define lineReadLength 4096

#define CSVSeperator ';'

void createFolder(char *relPath) {
    int pathLength = strlen(relPath);
    char path[PATH_MAX];
    if (getcwd(path, sizeof(path)) == NULL)
    {
        printf("OpenConfigRead:getCSV:Faild");
         return;
    }
    strcat(path, relPath);
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path,0777);
    }
}

bool fileExist(char* relPath)
{
    int pathLength = strlen(relPath);
    char path[PATH_MAX];
    if (getcwd(path, sizeof(path)) == NULL)
    {
        printf("OpenConfigRead:getCSV:Faild");
         return false;
    }
    strcat(path, relPath);

    return access( path, F_OK ) == 0;

}

void createCSV(
    int rows,
    int colums,
    int length,
    char result[rows][colums][length]) 
{
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < colums; y++)
        {
            strcpy(result[x][y],"");
        }
    }
}

int getFiles(int filesLength,int fileLength,char files[filesLength][fileLength],char* relPath) {
    
    int pathLength = strlen(relPath);
    char path[PATH_MAX];
    if (getcwd(path, sizeof(path)) == NULL)
    {
        printf("OpenConfigRead:getCSV:Faild");
         return -1;
    }
    strcat(path, relPath);

    
    int currentDir = 0;

    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    char full_path[PATH_MAX];

    for (int a=0; a < filesLength;a++) {
        strcpy(files[a],"");
    }

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            //Condition to check regular file.
            if(dir->d_type==DT_REG){
                strcat(files[currentDir],dir->d_name);
                strcat(files[currentDir],"\0");
                currentDir++;
                if (filesLength == currentDir) {
                    break;
                }
            }
        }
        closedir(d);
    }
    return 1;
}


int getColumCount(char *text,
                  int start)
{
    int length = strlen(text);
    int count = 0;
    if (start < 0)
    {
        start = start * -1;
    }
    count = start;
    for (int a = 0; a < length; a++)
    {
        if (text[a] == CSVSeperator)
        {
            count++;
        }
        else if (text[a] == '\n' || text[a] == '\r')
        {
            count++;
            return count;
        }
        else if (text[a] == '\0')
        {
            count++;
            return count * -1;
        }
    }
    return count * -1;
}

bool getCSVEntry(
    char *text,
    char *result,
    int resultLen,
    int number)
{
    int length = strlen(text);
    int count = 0;
    int start = 0;
    int end = 0;
    for (int a = 0; a < length; a++)
    {
        if (text[a] == CSVSeperator || text[a] == '\n' || text[a] == '\r' || text[a] == '\0')
        {
            start = end;
            end = a;
            if (count == number)
            {
                if (count != 0)
                {
                    start++;
                }
                int len = end - start;
                if (resultLen < len)
                {
                    return false;
                }

                strncpy(result, text + start, len);
                result[len] = '\0';
               // printf("found:%d, %s\n", number, result);
                return true;
            }
            count++;
        }
        if (text[a] == '\n' || text[a] == '\r' || text[a] == '\0')
        {
            return false;
        }
    }
    return false;
}

int getCSV(
    char *relPath,
    int rows,
    int colums,
    int length,
    char result[rows][colums][length])
{
    int pathLength = strlen(relPath);
    char path[PATH_MAX];
    if (getcwd(path, sizeof(path)) == NULL)
    {
       
         UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "OpenConfigRead:getCSV:Faild");

        return -1;
    }
    strcat(path, relPath);
    if (access(path, R_OK) != 0)
    {

         UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               "OpenConfigRead:getCSV:File not readable/exist:%s",path);
        return -1;
    }


    
    FILE *file = fopen(path, "r");
    char line[lineReadLength];

    bool read = true;
    for (int x = 0; x < rows && read; x++)
    {
        read = fgets(line, lineReadLength, file) != NULL;
        if (read)
        {
           // printf("line:%s\n", line);
            for (int y = 0; y < colums; y++)
            {
                getCSVEntry(line, result[x][y], length, y);
            }
        }
    }

    fclose(file);
    return 1;
}

int getCSVColum(
    int rows,
    int colums,
    int length,
    char csv[rows][colums][length],
    char *header)
{
    int headerlength = strlen(header);
    for (int y = 0; y < colums; y++)
    {
        if (strcmp(csv[0][y], header) == 0)
        {
            return y;
        }
    }
    
    return -1;
}

int addCSVColum(int rows,
    int colums,
    int length,
    char csv[rows][colums][length],
    char *header) {
    // Add if not exist
    for (int y=0; y < colums;y++) {
       if (strlen(csv[0][y]) == 0)
        {
            strcpy(csv[0][y],header);
            return y;
        } 
    }
    return -1;
}

int addCSVRow(
    int rows,
    int colums,
    int length,
    char csv[rows][colums][length],
    int colum,
    char *content) {
    for (int a=1; a < rows;a++) {
         if (strlen(csv[a][colum]) == 0) {
              strcpy(csv[a][colum],content);
              return a;
         }
    }
    return -1;
}

int getCSVRow(
    int rows,
    int colums,
    int length,
    char csv[rows][colums][length],
    int colum,
    char *content) {
    int contentlength = strlen(content);
    for (int a=1; a < rows;a++) {
        if (strncmp(csv[a][colum],content,contentlength) == 0) {
            return a;
        }
        if (strlen(csv[a][colum]) == 0) {
            return a * (-1);
        }
    }
    return 0;
}

void printCSV(
    int rows,
    int colums,
    int length,
    char csv[rows][colums][length])
{
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < colums; y++)
        {
            printf("%s;", csv[x][y]);
        }
        printf("\n");
    }
}

int writeCSV(
    char *relPath,
    int rows,
    int colums,
    int length,
    char csv[rows][colums][length])
{
    int pathLength = strlen(relPath);
    char path[PATH_MAX];
    if (getcwd(path, sizeof(path)) == NULL)
    {
        printf("OpenConfigRead:writeCSV:Faild");
        return -1;
    }
    strcat(path, relPath);

    FILE *file = fopen(path, "w");

     for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < colums; y++)
        {
            fputs(csv[x][y],file);
            if (y < colums-1) {
                fputc(CSVSeperator,file);
            }
        }
        fputc('\r',file);
        fputc('\n',file);

    }
    fclose(file);
    return 1;
}


void concatCells(
    int rows,
    int colums,
    int length,
    char csv[rows][colums][length],
    int count,
    int cellRows[count],
    int cellColumes[count],char * text) {
    for (int a=0; a < count;a++) {
        strcat(text,csv[cellRows[a]][cellColumes[a]]);
    }
}
#endif