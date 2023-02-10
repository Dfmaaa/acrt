#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s [create/add] [option input] [additional inputs (for add option only)]\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[1], "create") == 0) {
    struct group grp;
    struct group *result;
    char *buf;
    size_t bufsize;
    int s;

    bufsize = sysconf(_SC_GETGR_R_SIZE_MAX);
    if (bufsize == -1) {
      bufsize = 16384;
    }

    buf = malloc(bufsize);
    if (buf == NULL) {
      perror("malloc");
      exit(1);
    }

    s = getgrnam_r(argv[2], &grp, buf, bufsize, &result);
    if (result == NULL) {
      if (s == 0) {
        printf("Group '%s' already exists\n", argv[2]);
      } else {
        errno = s;
        perror("getgrnam_r");
      }
      exit(1);
    }

    if (result == NULL) {
      printf("Creating group '%s'\n", argv[2]);
      if (system(strcat("groupadd ",argv[2])) != 0) {
        perror("system");
        exit(1);
      }
    }
  } else if (strcmp(argv[1], "add") == 0) {
    if (argc < 4) {
      printf("Usage: %s add [group name] [user(s) to add to the group]\n", argv[0]);
      exit(1);
    }

    int i;
    char command[1024];
    snprintf(command, sizeof(command), "usermod -a -G %s", argv[2]);

    for (i = 3; i < argc; i++) {
      strncat(command, " ", sizeof(command) - strlen(command) - 1);
      strncat(command, argv[i], sizeof(command) - strlen(command) - 1);
    }

    printf("Adding users to group '%s'\n", argv[2]);
    if (system(command) != 0) {
      perror("system");
      exit(1);
    }
  } else {
    printf("Invalid option '%s'\n", argv[1]);
    printf("Usage: %s [create/add] [option input] [additional inputs (for add option only)]\n", argv[0]);
    exit(1);
  }

  return 0;
}
