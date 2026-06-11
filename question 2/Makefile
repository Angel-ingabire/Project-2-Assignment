CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = max_heap_security

all: $(TARGET)

$(TARGET): max_heap_security.c
	$(CC) $(CFLAGS) -o $(TARGET) max_heap_security.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all run clean
