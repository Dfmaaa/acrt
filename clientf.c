#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
int main(int argc, char **argv){
    if(argc!=2){
        printf("Usage: %s [named pipe location]\n",argv[0]);
        return 0;
    }
}