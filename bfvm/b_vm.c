#include "bfvm.h"

#include <stdio.h>

BfVm *
bfvm_create()
{
	BfVm *vm;

	vm = bf_calloc(1, sizeof(*vm) - sizeof(*vm->code), "VM Allocation",
		       "Whole Brainfunc VM including memory, indexes and code");

	return vm;
}

void
bfvm_delete(BfVm *vm)
{
	bf_free(vm);
}

boolean
bfvm_code_load_from_file(BfVm **vm, const char *file_path)
{
	FILE	    *fp;
	uint	     file_size;
	BfExecHeader header;

	/* open file */
	fp = fopen(file_path, "rb");
	if (!fp) {
		return 1;
	}

	/* get file size */
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);

	/* fail if file is too small to be executable */
	if (file_size <= sizeof(BfExecHeader)) {
		fclose(fp);
		return 1;
	}

	/* read header */
	fread(&header, sizeof(BfExecHeader), 1, fp);

	/* resize vm */
	*vm = bf_realloc(*vm, sizeof(**vm) - sizeof(char) + file_size,
			 "VM Resize", "realloc for code from file");

	/* popo */
	fprintf(stderr,
		"BfExec Version %d.%d\n=> Entry: 0x%08X\n=> Size:  0x%08X\n\n",
		header.ver_maj, header.ver_min, header.entry, header.size);

	/* set size and entrypoint */
	(*vm)->index_code = header.entry;
	(*vm)->code_size = header.size;

	/* read code into memory */
	fread((*vm)->code, sizeof(char), header.size, fp);

	fclose(fp);
	return 0;
}

void
bfvm_loop_start(BfVm *vm)
{
	if (!(vm->memory[vm->index_memory])) {
		while (vm->code[vm->index_code++] != BF_BYTE_LOOP_END) {
			fputc('.', stderr);
			fflush(stderr);
		}
		fputc('\n', stderr);

		return;
	}

#ifdef BF_DEBUG
	fprintf(stderr, "[%d] PUSH %d\n", vm->index_callstack, vm->index_code);
#endif

	vm->callstack[++vm->index_callstack] = vm->index_code;
}

void
bfvm_loop_exit(BfVm *vm)
{
	if (!(vm->memory[vm->index_memory])) {
#ifdef BF_DEBUG
		fprintf(stderr, "[%d] TRASH %d\n", vm->index_callstack,
			vm->callstack[vm->index_callstack]);
#endif
		vm->index_callstack--;
		return;
	}
#ifdef BF_DEBUG
	fprintf(stderr, "[%d] BACK %d\n", vm->index_callstack,
		vm->callstack[vm->index_callstack]);
#endif
	vm->index_code = vm->callstack[vm->index_callstack];
	return;
}

void
bfvm_call(BfVm *vm)
{
	vm->callstack[++vm->index_callstack] =
	    vm->code[vm->index_code + BF_SIZE_ADDRESS + 1];
	vm->index_code = ((uint)vm->code[vm->index_code + 1]);
}

void
bfvm_return(BfVm *vm)
{
	vm->index_code = vm->callstack[vm->index_callstack--];
}

boolean
bfvm_exec(BfVm *vm)
{
	BfByte opcode;
	while (1) {
		opcode = vm->code[vm->index_code];
		/* fprintf(stderr, "[%04Xe]opcode: %X\n", vm->index_code,
		 * opcode); */
		switch (opcode) {
		case BF_BYTE_NONE:
			return 1;
		case BF_BYTE_ADD:
			vm->memory[vm->index_memory]++;
			break;
		case BF_BYTE_SUB:
			vm->memory[vm->index_memory]--;
			break;
		case BF_BYTE_LEFT:
			vm->index_memory--;
			break;
		case BF_BYTE_RIGHT:
			vm->index_memory++;
			break;
		case BF_BYTE_LOOP_START:
			bfvm_loop_start(vm);
			break;
		case BF_BYTE_LOOP_END:
			bfvm_loop_exit(vm);
			break;
		case BF_BYTE_PRINT:
			fputc(vm->memory[vm->index_memory], stdout);
			break;
		case BF_BYTE_READ:
			break;
		case BF_BYTE_CALL:
			bfvm_call(vm);
			break;
		case BF_BYTE_RETURN:
			bfvm_return(vm);
			break;
		default:
			return 1;
		}

		vm->index_code++;
	}
	return 0;
}
