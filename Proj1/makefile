# TOOLS
CC ?= gcc
# OPTIONS
LEVEL ?= 3
DEBUG ?= -DNDEBUG -D_FORTIFY_SOURCE=2
WARNINGS ?= -Wall -Werror -Wpedantic -Wunused-result
CFLAGS ?= -std=gnu11 $(WARNINGS) $(DEBUG) -O$(LEVEL) $(EXTRA)

# SOURCE
SRC := main.c random.c dicio.c
HEADERS := $(wildcard *.h)
DICIO := no.c
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
debug: DEBUG = -DDEBUG -ggdb3
debug: all

clean:
	rm -rf *.o $(PROG) *.zip

$(LAB).zip: makefile $(SRC) $(DICIO) $(wildcard *.h)
	zip -J $@ $^


# BUILDING
$(PROG): $(SRC:.c=.o)
	$(CC) $(CFLAGS) $^ -o $@

dicio.o: dicio.c $(DICIO) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

extra:
	echo $(EXTRA)
