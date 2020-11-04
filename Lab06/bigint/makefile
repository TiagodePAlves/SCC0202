# ARTIFACT
LIB ?= bigint
OUT := ../$(LIB).o
# SOURCE
SRC := $(wildcard *.c)


# GENERAL COMMANDS
.PHONY: all run clean debug zip

all: $(OUT)

echo:
	@echo $(CC) $(CFLAGS) $(LIB) $(SRC)

# BUILDING
$(OUT): $(SRC)
	$(CC) $(CFLAGS) -c $^ -o $@
