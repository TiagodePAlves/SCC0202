# TOOLS
CC ?= gcc
# OPTIONS
LEVEL ?= 3
DEBUG ?= -DNDEBUG
WARNINGS ?= -Wall -Werror -Wpedantic
CFLAGS ?= -std=c11 $(WARNINGS) $(DEBUG) -O$(LEVEL)

# SOURCE
SRC := main.c stack.c
# ARTIFACT
PROD  := main
OPTIM := optimized

# GENERAL COMMANDS
.PHONY: all run clean debug zip

all: $(PROD)

run: $(PROD)
	./$(PROD)

zip: 02.zip

debug: LEVEL = 0
debug: DEBUG = -DDEBUG
debug: CFLAGS += -ggdb3
debug: clean all

clean:
	rm -rf *.o $(PROD) *.zip

%.zip: makefile $(SRC) $(wildcard *.h) $(OPTIM).c
	zip -J $@ $^


# BUILDING
$(PROD): $(SRC:.c=.o)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OPTIM): $(OPTIM).c
	$(CC) $(CFLAGS) $< -o $@
