#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_WR 2000
#define OUTPUT_LENM 10000000
int main(int argc, char **argv){
    if(argc!=2){
        printf("Usage: %s [named pipe location]\n",argv[0]);
        return 0;
    }
    int pipe=open(argv[1],O_RDONLY | O_WRONLY);
    if(pipe==-1){
        printf("Can't open pipe.\n");
        return 1;
    }
    while(1){
        char command[MAX_WR];
        printf("Enter command.\n");
        scanf("%s",command);
        if(write(pipe,command,strlen(command))==-1){
            printf("Can't write to pipe.\n");
            return 1;
        }
        if(strcmp("exit",command)==0){
            break;
        }
        char output[OUTPUT_LENM];
        int len;
        if((len=read(pipe,output,OUTPUT_LENM))==-1){
            printf("Can't read pipe.\n");
            return 1;
        }
        output[len]='\0';
        printf("%s\n",output);
    }
    close(pipe);
    return 0;
}