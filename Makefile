# Variables
COMPILER = gcc
C_FLAGS = -g -Wall
BINARY_NAME = shell_simulator
RM = rm
DEPS = shell.h

# 'make build'
build:
	$(COMPILER) -o $(BINARY_NAME) $(CFLAGS) $(DEPS) shell.c main.c

# 'make run'
run: build
	./$(BINARY_NAME)

# 'make clean'
clean: build
	$(RM) $(BINARY_NAME)