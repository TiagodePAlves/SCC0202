# TOOLS
CC ?= gcc
# OPTIONS
LEVEL ?= 3
DEBUG ?= -DNDEBUG
WARNINGS ?= -Wall -Werror -Wpedantic
CFLAGS ?= -std=c11 $(WARNINGS) $(DEBUG) -O$(LEVEL)

# SOURCE
SRC := main.c
# ARTIFACT
PROD := main

# GENERAL COMMANDS
.PHONY: all run clean

all: $(PROD)

run: all
	./$(PROD)

debug: LEVEL = 0
debug: DEBUG = -DDEBUG
debug: CFLAGS += -ggdb3
debug: all

clean:
	rm -rf *.o $(PROD)


# BUILDING
$(PROD): $(SRC:.c=.o)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
