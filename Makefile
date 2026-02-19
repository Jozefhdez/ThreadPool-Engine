CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -g -Iinclude -fsanitize=address
TARGET = build/main
SRCS = main.c task.c lockfree_queue.c
OBJS = $(addprefix build/, $(SRCS:.c=.o))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: %.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build