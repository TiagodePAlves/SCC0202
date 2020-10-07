# TOOLS
CC ?= gcc
# OPTIONS
LEVEL ?= 3
DEBUG ?= -DNDEBUG
WARNINGS ?= -Wall -Werror -Wpedantic
CFLAGS ?= -std=gnu11 $(WARNINGS) $(DEBUG) -O$(LEVEL)

# SOURCE
SRC := main.c lista.c
# ARTIFACT
PROD := main
LAB  := 04

# GENERAL COMMANDS
.PHONY: all run clean debug zip

all: $(PROD)

run: $(PROD)
	./$(PROD)

zip: $(LAB).zip

debug: LEVEL = 0
debug: DEBUG = -DDEBUG
debug: CFLAGS += -ggdb3
debug: clean all

clean:
	rm -rf *.o $(PROD) *.zip

$(LAB).zip: makefile $(SRC) $(wildcard *.h)
	zip -J $@ $^


# BUILDING
$(PROD): $(SRC:.c=.o)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
