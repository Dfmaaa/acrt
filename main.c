#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <grp.h>
#include "daemonf.h"

#define DEFAULT_PIPE "acrt_p"
#define PIPE_PERMS 066
#define MAX_PIPE_N_S 256

int main(int argc, char **argv){
    if(argc<2){
        printf("usage: %s [group] [optional, location of named pipe].\n",argv[0]);
        return 0;
    }
    char p_n[MAX_PIPE_N_S];
    if(argc==3){
        strncpy(p_n,argv[2],MAX_PIPE_N_S);
    }
    else{
        strncpy(p_n,DEFAULT_PIPE,MAX_PIPE_N_S);   
    }
    printf("Name of pipe will be %s\n",p_n);
    int acrt_p;
    if((acrt_p=mkfifo(p_n,PIPE_PERMS))==-1){
        printf("Pipe can't be created. This can happen if the pipe already exists. If this isn't the case, create an issue on Github.\n");
        printf("strerror output: %s.\n",strerror(errno));
        return 1;
    }
    printf("Pipe with name %s created.\n",p_n);
    struct group *aug=getgrnam(argv[1]);
    if(!aug){
        printf("%s is not a group. Use the group_helper executable to create groups and add users to them.\n");
        close(acrt_p);
        printf("Deleting pipe named %s\n",p_n);
        remove(p_n);
        return 1;
    }
    gid_t aug_id = aug->gr_gid;
    if (chown(p_n, -1, aug_id)==-1) {
        printf("%s can't become the group owner of %s\n",argv[1],p_n);
        printf("strerror output: %s\n",strerror(errno));
        close(acrt_p);
        printf("Deleting pipe named %s\n",p_n);
        remove(p_n);
        return 1;
    }
    if (chmod(p_n, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH) == -1) {
        printf("Can't change file permissions of %s\n",p_n);
        close(acrt_p);
        printf("Deleting pipe named %s\n",p_n);
        remove(p_n);
        return 1;
    }
    printf("Users in %s can now read/write to %s\n",argv[1],p_n);
    printf("Assigning new SID to process. This is Goodbye :(. Use the client executable to write to %s. Keep in mind that only users in %s can write to it.\n",p_n,argv[1]);
    int f=fork();
    if(f==-1){
        printf("Can't create child process.\n");
        close(acrt_p);
        printf("Deleting pipe named %s\n",p_n);
        remove(p_n);
        return 1;
    }
    if(f==0){
        int ns=setsid();
        start_r(acrt_p);
        close(acrt_p);
        printf("Deleting pipe named %s\n",p_n);
        remove(p_n);
        return 0;
    }
    else{
        return 0;
    }
    return 0;
}