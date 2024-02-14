CC=cc
CFLAGS=-std=c89 -O2 -Wall -Wextra -Werror -pedantic -ansi

OBJ_BFC=\
bfc/b_compiler.o \
bfc/b_func_reg.o \
bfc/b_main.o

OBJ_SHARED=\
shared/x_memory.o

.PHONY: clean
.DEFAULT: all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bfc: $(OBJ_BFC) $(OBJ_SHARED)
	$(CC) $(OBJ_BFC) $(OBJ_SHARED) -o bfc/$@

all: bfc

clean:
	rm $(OBJ_BFC) $(OBJ_SHARED) bfc/bfc
