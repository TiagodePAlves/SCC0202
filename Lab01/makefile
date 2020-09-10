# TOOLS
CC = gcc
CFLAGS = -std=c11

# SOURCE
SRC = main.c
# ARTIFACT
PROD = main

# GENERAL COMMANDS
.PHONY: all run clean

all: $(PROD)

run: all
	./$(PROD)

clean:
	rm -rf *.o $(PROD)

# BUILDING
$(PROD): $(SRC:.c=.o)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
