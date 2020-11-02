# TOOLS
CC ?= gcc
# OPTIONS
LEVEL ?= 3
DEBUG ?= -DNDEBUG -D_FORTIFY_SOURCE=2
WARNINGS ?= -Wall -Werror -Wpedantic -Wunused-result
CFLAGS ?= -std=gnu11 $(WARNINGS) $(DEBUG) -O$(LEVEL)

# SOURCE
SRC := main.c dicio.c random.c
# ARTIFACT
PROG := main
LAB  := P1

# GENERAL COMMANDS
.PHONY: all run clean debug zip

all: $(PROG)

run: $(PROG)
	./$(PROG)

zip: $(LAB).zip

debug: LEVEL = 0
debug: DEBUG = -DDEBUG
debug: CFLAGS += -ggdb3
debug: clean all

clean:
	rm -rf *.o $(PROG) *.zip

$(LAB).zip: makefile $(SRC) $(wildcard *.h)
	zip -J $@ $^


# BUILDING
$(PROG): $(SRC:.c=.o)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
