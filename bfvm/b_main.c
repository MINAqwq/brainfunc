#include "bfvm.h"

int
main(int argc, char **argv)
{
	BfVm *vm;

	if (argc != 2) {
		return 1;
	}

	vm = bfvm_create();

	bfvm_code_load_from_file(&vm, argv[1]);
	bfvm_exec(vm);

	bfvm_delete(vm);
	return 0;
}
