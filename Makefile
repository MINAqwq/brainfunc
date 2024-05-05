CC=cc
CFLAGS=-std=c89 -O2 -Wall -Wextra -Werror -ansi

OBJ_BFC=\
bfc/b_compiler.o \
bfc/b_func_reg.o \
bfc/b_main.o

OBJ_BFVM=\
bfvm/b_main.o\
bfvm/b_vm.o

OBJ_SHARED=\
shared/x_memory.o

all: bfc bfvm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bfc: $(OBJ_BFC) $(OBJ_SHARED)
	$(CC) $(OBJ_BFC) $(OBJ_SHARED) -o $@/$@

bfvm: $(OBJ_BFVM) $(OBJ_SHARED)
	$(CC) $(OBJ_BFVM) $(OBJ_SHARED) -o $@/$@

.PHONY:
clean:
	rm $(OBJ_BFC) $(OBJ_BFVM) $(OBJ_SHARED) bfc/bfc bfvm/bfvm
