CC=cc
CFLAGS=-std=c89 -O2 -Wall -Wextra -Werror -ansi
OBJ_BFC=\
bfc/b_compiler.o \
bfc/b_func_reg.o \
bfc/b_main.o

OBJ_BFVM=\
bfvm/b_main.o\
bfvm/b_syscall.o\
bfvm/b_vm.o\
shared/vector.o

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

.PHONY:
install: install_bfc install_bfvm

.PHONY:
uninstall: uninstall_bfc uninstall_bfvm

.PHONY:
install_bfc: bfc
	strip bfc/bfc
	cp -v bfc/bfc /usr/bin/bfc

.PHONY:
uninstall_bfc:
	rm -v /usr/bin/bfc

.PHONY:
install_bfvm: bfvm
	strip bfvm/bfvm
	cp -v bfvm/bfvm /usr/bin/bfvm

.PHONY:
uninstall_bfvm:
	rm -v /usr/bin/bfvm
