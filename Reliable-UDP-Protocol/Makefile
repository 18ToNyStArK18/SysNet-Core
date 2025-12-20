#AI GENERATED
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 

# Target executables
CLIENT = client
SERVER = server

# Source files
CLIENT_SRC = client.c
SERVER_SRC = server.c
HEADERS = sham.h

# Object files
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)

# Default target - build both
all: $(CLIENT) $(SERVER)

# Build client executable
$(CLIENT): $(CLIENT_OBJ)
	$(CC) $(CLIENT_OBJ) -o $(CLIENT) $(LDFLAGS)

# Build server executable
$(SERVER): $(SERVER_OBJ)
	$(CC) $(SERVER_OBJ) -o $(SERVER) $(LDFLAGS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Build only client
client-only: $(CLIENT)

# Build only server
server-only: $(SERVER)

# Clean build artifacts
clean:
	rm -f $(CLIENT_OBJ) $(SERVER_OBJ) $(CLIENT) $(SERVER)

# Rebuild everything from scratch
rebuild: clean all

# Install both executables (optional - customize as needed)
install: $(CLIENT) $(SERVER)
	cp $(CLIENT) /usr/local/bin/
	cp $(SERVER) /usr/local/bin/

# Uninstall both executables (optional)
uninstall:
	rm -f /usr/local/bin/$(CLIENT)
	rm -f /usr/local/bin/$(SERVER)

# Run client
run-client: $(CLIENT)
	./$(CLIENT)

# Run server
run-server: $(SERVER)
	./$(SERVER)

# Debug client with gdb
debug-client: $(CLIENT)
	gdb ./$(CLIENT)

# Debug server with gdb
debug-server: $(SERVER)
	gdb ./$(SERVER)

# Check client for memory leaks with valgrind
memcheck-client: $(CLIENT)
	valgrind --leak-check=full --show-leak-kinds=all ./$(CLIENT)

# Check server for memory leaks with valgrind
memcheck-server: $(SERVER)
	valgrind --leak-check=full --show-leak-kinds=all ./$(SERVER)

# Print variables (for debugging the Makefile)
print-%:
	@echo '$*=$($*)'

# Phony targets (not actual files)
.PHONY: all client-only server-only clean rebuild install uninstall run-client run-server debug-client debug-server memcheck-client memcheck-server print-%
