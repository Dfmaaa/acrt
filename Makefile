CC = gcc
CFLAGS = -Wall -Wextra

all: main clientf group_helper

main: main.c daemonf.c
	$(CC) $(CFLAGS) -o acrt main.c daemonf.c

clientf: clientf.c
	$(CC) $(CFLAGS) -o clientf clientf.c

group_helper: group_helper.c
	$(CC) $(CFLAGS) -o group_helper group_helper.c

clean:
	rm -f acrt clientf group_helper
