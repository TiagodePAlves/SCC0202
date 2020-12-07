# TOOLS
CC ?= gcc
# OPTIONS
LEVEL ?= 3
OPTFL := -march=native -mtune=native -pipe -fivopts
OPTFL += -fmodulo-sched -flto -fwhole-program -fno-plt
DEBUG ?= -DNDEBUG -D_FORTIFY_SOURCE=1
WARNINGS ?= -Wall -Werror -Wpedantic -Wunused-result
CFLAGS ?= -std=gnu11 $(WARNINGS) $(DEBUG) -O$(LEVEL) $(OPTFL)

# ARTIFACT
PROG := main
LAB  := P3
# SOURCE
SRC := main.c treap.c
HEADERS := $(wildcard *.h)

# GENERAL COMMANDS
.PHONY: all run clean debug zip

all: $(PROG)

run: $(PROG)
	@./$(PROG)

zip: $(LAB).zip

debug: LEVEL = 0
debug: DEBUG = -DDEBUG
debug: OPTFL = -ggdb3
debug: clean all

clean:
	rm -rf *.o $(PROG) *.zip

$(LAB).zip: makefile $(SRC) $(HEADERS)
	zip -J $@ $^

# BUILDING
$(PROG): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@
