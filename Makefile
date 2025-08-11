# Compiler and flags
CC := gcc
CFLAGS := -O3 -Wall -Iheader -mwindows -s
LDFLAGS := -lgdi32 -lmsimg32

# Source and object files
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,obj/%.o,$(SRC))

# Resources
RES := resources/resources.res
RC := resources/resources.rc

# Output executable
OUT := bin/snake_game.exe

.PHONY: all clean resources run

all: resources $(OUT)

resources: $(RES)

$(RES): $(RC)
	@windres $(RC) -O coff -o $(RES)

# Link the final executable
$(OUT): $(OBJ) $(RES)
	@$(CC) $(OBJ) $(RES) -o $(OUT) $(LDFLAGS)

# Compile source files to obj/ directory
obj/%.o: src/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f obj/*.o bin/*.exe resources/*.res

run: all
	@"$(OUT)"
