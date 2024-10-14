CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -I/opt/homebrew/opt/openssl/include
LDFLAGS = -L/opt/homebrew/opt/openssl/lib -lssl -lcrypto
SOURCES = src/server.c src/socket.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = build/https_server

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
