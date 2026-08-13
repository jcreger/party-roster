CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g

main.exe: main.c input.c character.c render.c
	$(CC) $(CFLAGS) main.c input.c character.c render.c -o main.exe
