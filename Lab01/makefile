# TOOLS
CC ?= gcc
# OPTIONS
LEVEL ?= 3
WARNINGS ?= -Wall -Werror -Wpedantic
CFLAGS ?= -std=c11 $(WARNINGS) -O$(LEVEL)

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
debug: CFLAGS += -ggdb3 -DDEBUG
debug: all

clean:
	rm -rf *.o $(PROD)


# BUILDING
$(PROD): $(SRC:.c=.o)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
