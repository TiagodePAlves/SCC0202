# TOOLS
CC ?= gcc
# OPTIONS
LEVEL ?= 3
DEBUG ?= -DNDEBUG
WARNINGS ?= -Wall -Werror -Wpedantic
CFLAGS ?= -std=c11 $(WARNINGS) $(DEBUG) -O$(LEVEL)

# SOURCE
SRC := main.c subst.c
# ARTIFACT
PROD := main

# GENERAL COMMANDS
.PHONY: all run clean debug zip

all: $(PROD)

run: all
	./$(PROD)

zip: 01.zip

debug: LEVEL = 0
debug: DEBUG = -DDEBUG
debug: CFLAGS += -ggdb3
debug: all

clean:
	rm -rf *.o $(PROD) *.zip

%.zip: makefile $(SRC) $(wildcard *.h)
	zip -J $@ $^


# BUILDING
$(PROD): $(SRC:.c=.o)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
