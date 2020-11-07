# TOOLS
CC ?= gcc
# OPTIONS
LEVEL ?= 3
DEBUG ?= -DNDEBUG -D_FORTIFY_SOURCE=2
WARNINGS ?= -Wall -Werror -Wpedantic -Wunused-result
CFLAGS ?= -std=gnu11 $(WARNINGS) $(DEBUG) -O$(LEVEL)

# ARTIFACT
PROG := main
LAB  := 07
# SOURCE
SRC := main.c
HEADERS := $(wildcard *.h)

# SUBMAKES
export CC CFLAGS LIB

# GENERAL COMMANDS
.PHONY: all run clean debug zip

all: $(PROG)

run: $(PROG)
	@./$(PROG)

zip: $(LAB).zip

debug: LEVEL = 0
debug: DEBUG = -DDEBUG -ggdb3
debug: all

clean:
	rm -rf *.o $(PROG) *.zip

$(LAB).zip: makefile $(SRC) $(HEADERS)
	zip -J $@ $^

# BUILDING
$(PROG): $(SRC:.c=.o)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
