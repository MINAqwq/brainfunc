CC=cc
CFLAGS=-std=c89 -O2 -Wall -Wextra -Werror -ansi -Wno-unused-result
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

OUT_DIR=bin

all: bfc bfvm

out_dir:
	mkdir -p $(OUT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bfc: $(OBJ_BFC) $(OBJ_SHARED) out_dir
	$(CC) $(OBJ_BFC) $(OBJ_SHARED) -o $(OUT_DIR)/$@

bfvm: $(OBJ_BFVM) $(OBJ_SHARED) out_dir
	$(CC) $(OBJ_BFVM) $(OBJ_SHARED) -o $(OUT_DIR)/$@

.PHONY:
clean:
	rm $(OBJ_BFC) $(OBJ_BFVM) $(OBJ_SHARED) $(OUT_DIR)/bfc $(OUT_DIR)/bfvm

.PHONY:
install: install_bfc install_bfvm

.PHONY:
uninstall: uninstall_bfc uninstall_bfvm

.PHONY:
install_bfc: bfc
	strip $(OUT_DIR)/bfc
	cp -v $(OUT_DIR)/bfc /usr/bin/bfc

.PHONY:
uninstall_bfc:
	rm -v /usr/bin/bfc

.PHONY:
install_bfvm: bfvm
	strip $(OUT_DIR)/bfvm
	cp -v $(OUT_DIR)/bfvm /usr/bin/bfvm

.PHONY:
uninstall_bfvm:
	rm -v /usr/bin/bfvm
