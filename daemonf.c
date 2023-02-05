#include "daemonf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_READ 2000
char *execute(const char *command) {
    char buffer[1024];
    char *result = NULL;
    size_t result_len = 0;

    FILE *fp=popen(command, "r");
    if(fp==NULL) {
        return NULL;
    }

    while(fgets(buffer, sizeof(buffer), fp)!=NULL){
        size_t len=strlen(buffer);
        result=realloc(result, result_len + len + 1);
        if (result == NULL) {
            pclose(fp);
            return NULL;
        }
        memcpy(result+result_len, buffer, len);
        result_len+=len;
        result[result_len]='\0';
    }
    pclose(fp);
    return result;
}
int start_r(int pipe){
    while(1){
        char buffer[MAX_READ];
        int c;
        if((c=read(pipe,buffer,MAX_READ))==-1){
            return -1;
        }
        buffer[c]='\0';
        if(strcmp(buffer,"exit")==0){
            return 1;
        }
        char *output=execute(buffer);
        if((c=write(pipe,output,2147483647))==-1){
            return -1;
        }
    }
    return 1;
}