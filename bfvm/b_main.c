#include "bfvm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
runtime_error(BfVm *vm)
{
	fputs("=== YOUR CODE IS BUGGY BTW ====\n", stderr);
	while (vm->index_callstack) {
		fprintf(stderr, "[%08X]\n",
			vm->callstack[vm->index_callstack--]);
	}

	fprintf(stderr,
		"======== RUNTIME ERROR ========\nIP: 0x%08X\nOP: 0x%02X\nMI: "
		"0x%04X\nCI: %d\n",
		vm->index_code, vm->code[vm->index_code], vm->index_memory,
		vm->index_callstack);
	exit(2);
}

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
		runtime_error(vm);
	}

	bfvm_delete(vm);
	return 0;
}
