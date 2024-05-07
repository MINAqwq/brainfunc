#include "bfvm.h"

#include <stdio.h>

boolean
bfvm_syscall_open(BfVm *vm)
{
	uint  index;
	char *arg1;
	char *arg2;
	FILE *fp;
	uint8 ret;

	if ((vm->index_memory + 4) >= sizeof(vm->memory)) {
		return 1;
	}

	index = vm->index_memory;

	arg1 = (char *)&vm->memory[++index];

	/* skip next string */
	while (vm->memory[++index] != 0) {
		if (index + 1 >= sizeof(vm->memory)) {
			return 1;
		}
	}

	arg2 = (char *)&vm->memory[++index];

	/* skip this string as sanity check */
	while (vm->memory[++index] != 0) {
		if (index + 1 >= sizeof(vm->memory)) {
			return 1;
		}
	}

	fp = fopen(arg1, arg2);
	ret = !fp ? -1 : 5;

	fprintf(stderr, "%d\n", ret);

	/* save return value in bf memory */
	vm->memory[vm->index_memory] = ret;

	return 0;
}

boolean
bfvm_syscall_close(BfVm *vm)
{
	(void)vm;
	return 1;
}

boolean
bfvm_syscall_read(BfVm *vm)
{
	(void)vm;
	return 1;
}

boolean
bfvm_syscall_write(BfVm *vm)
{
	(void)vm;
	return 1;
}

boolean
bfvm_syscall_exec(BfVm *vm)
{
	switch (vm->memory[vm->index_memory]) {
	case BF_SYS_OPEN:
		return bfvm_syscall_open(vm);
	case BF_SYS_CLOSE:
		return bfvm_syscall_close(vm);
	case BF_SYS_READ:
		return bfvm_syscall_read(vm);
	case BF_SYS_WRITE:
		return bfvm_syscall_write(vm);
	default:
		return 1;
	}
}