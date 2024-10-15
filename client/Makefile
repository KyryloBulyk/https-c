CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SOURCES = src/client.c src/rsa.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = build/client

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
