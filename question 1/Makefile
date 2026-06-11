CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = max_heap_scheduler

all: $(TARGET)

$(TARGET): max_heap_scheduler.c
	$(CC) $(CFLAGS) -o $(TARGET) max_heap_scheduler.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all run clean
