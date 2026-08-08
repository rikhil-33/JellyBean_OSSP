CC = gcc
CFLAGS = -Wall
TARGET = shell

all:
	gcc -Wall src/main.c -o shell

clean:
	rm -f shell