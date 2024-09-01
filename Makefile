# Variables
CC = gcc
CFLAGS = -Wall
SERVER_TARGET = server

# source files
SERVER_SRC = src/server.c

# Compile both by default
all: $(SERVER_TARGET)

# Rule to compile the server
$(SERVER_TARGET): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) $(SERVER_SRC)


# Run the server
run_server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

# Clean
clean:
	rm -f $(SERVER_TARGET)
