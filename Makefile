CC=gcc
CFLAGS= -W -Wall -Werror -g

all: chirp chirp_listener

chirp_listener: chirp_listener.c
	$(CC) $(CFLAGS) chirp_listener.c -o chirp_listener

chirp: chirp.c
	$(CC) $(CFLAGS) chirp.c -o chirp
