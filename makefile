CC = gcc
EXEC = build/a
CFLAGS = -Wall -Wextra
SRCS = $(wildcard src/*.c)

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(EXEC)