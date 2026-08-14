CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -MMD -MP
LDFLAGS = -g

SRCS = main.c input.c character.c render.c item.c
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

main.exe: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o main.exe

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)
