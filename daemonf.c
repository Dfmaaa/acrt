#include "daemonf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#define MAX_BUF_SIZE 100
char *execute(char *command) {
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

int start_r(int read_fd) {
  while(1){
  fd_set read_fds;
  int ret;
  // Initialize the file descriptor set for the select function
  FD_ZERO(&read_fds);
  FD_SET(read_fd, &read_fds);
  // Wait for data to be available on the pipe
  ret = select(read_fd + 1, &read_fds, NULL, NULL, NULL);
  if (ret == -1) {
    // Error occurred
    return -1;
  } else if (ret == 0) {
    // Timeout occurred
    return 0;
  } else {
    // Data is available to be read from the pipe
    char buf[MAX_BUF_SIZE];
    int num_read = read(read_fd, buf, MAX_BUF_SIZE);
    if (num_read == -1) {
      // Error occurred
      return -1;
    }
    buf[num_read]='\0';
        if(strcmp(buf,"exit")==0){
            return 1;
        }
        char *output=execute(buf);
        if(write(read_fd,output,2147483647)==-1){
            return -1;
        }
    }
  }
  return 0;
 }
