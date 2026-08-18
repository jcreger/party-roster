CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g -MMD -MP -Iinclude
LDFLAGS = -g

SRC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

main.exe: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o main.exe

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) main.exe

-include $(DEPS)

.PHONY: clean
