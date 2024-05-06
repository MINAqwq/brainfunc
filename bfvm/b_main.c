#include "bfvm.h"

#include <stdio.h>
#include <string.h>

int
main(int argc, char **argv)
{
	BfVm *vm;

	if (argc != 2) {
		return 1;
	}

	vm = bfvm_create();

	if (!strcmp(argv[1], "-")) {
		if (!bfvm_code_load_from_stdin(&vm)) {
			goto exec;
		}

		fputs("failed reading from stdin\n", stderr);
		bfvm_delete(vm);
		return 1;
	}

	if (bfvm_code_load_from_file(&vm, argv[1])) {
		fprintf(stderr, "failed loading %s\n", argv[1]);
		bfvm_delete(vm);
		return 1;
	}

exec:
	if (bfvm_exec(vm)) {
		fprintf(stderr,
			"==== RUNTIME ERROR ====\nIP: 0x%08X\nOP: 0x%02X\nMI: "
			"0x%04X\nCI: %d\n",
			vm->index_code, vm->code[vm->index_code],
			vm->index_memory, vm->index_callstack);
	}

	bfvm_delete(vm);
	return 0;
}
