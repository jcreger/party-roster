CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g

main.exe: main.c
	$(CC) $(CFLAGS) main.c -o main.exe
