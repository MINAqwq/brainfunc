#include "bfvm.h"

#include "../shared/bfunc.h"
#include "../shared/vector.h"
#include "../shared/xmemory.h"

#include <stdio.h>
#include <string.h>

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
bfvm_header_validate(BfVm **vm, BfExecHeader *header)
{
	if (header->magic[0] != 'B' || header->magic[1] != 'F' ||
	    header->magic[2] != 'U' || header->magic[3] != 'N' ||
	    header->entry >= header->size || header->ver_maj != BF_VER_MAJOR ||
	    header->ver_min > BF_VER_MINOR) {
		return 1;
	}

	/* set size and entrypoint */
	(*vm)->index_code = header->entry;
	(*vm)->code_size = header->size;

	fprintf(stderr,
		"BfExec Version %d.%d\n=> Entry: 0x%08X\n=> Size:  0x%08X\n\n",
		header->ver_maj, header->ver_min, header->entry, header->size);

	return 0;
}

boolean
bfvm_code_load(BfVm **vm, uint8 *code, uint size)
{
	if (!code || !size) {
		return 1;
	}

	/* resize vm */
	*vm = bf_realloc(*vm, sizeof(**vm) - sizeof(char) + size, "VM Resize",
			 "realloc for code from memory");

	memcpy((*vm)->code, code, size);

	return 0;
}

boolean
bfvm_code_load_from_fileptr(BfVm **vm, FILE *fp, uint file_size)
{
	BfExecHeader header;

	/* fail if file is too small to be executable */
	if (file_size <= sizeof(BfExecHeader)) {
		fclose(fp);
		return 1;
	}

	/* read header */
	fread(&header, sizeof(BfExecHeader), 1, fp);

	if (bfvm_header_validate(vm, &header)) {
		return 1;
	}

	/* resize vm */
	*vm = bf_realloc(*vm, sizeof(**vm) - sizeof(char) + file_size,
			 "VM Resize", "realloc for code from file");

	/* read code into memory */
	fread((*vm)->code, sizeof(char), header.size, fp);
	return 0;
}

boolean
bfvm_code_load_from_stdin(BfVm **vm)
{
	char	      ch;
	BfVector      vec;
	boolean	      ret;
	BfExecHeader *header;

	bf_vector_create(&vec);

	/* append byte by byte */
	while ((ch = fgetc(stdin)) != EOF) {
		if (bf_vector_append(&vec, &ch, sizeof(ch))) {
			return 1;
		}
	}

	header = vec.data;

	if (vec.size < sizeof(*header)) {
		return 1;
	}

	ret = (bfvm_header_validate(vm, header) ||
	       bfvm_code_load(vm, vec.data + sizeof(*header), header->size));

	bf_vector_delete(&vec);

	return ret;
}

boolean
bfvm_code_load_from_file(BfVm **vm, const char *file_path)
{
	FILE *fp;
	uint  file_size;

	/* open file */
	fp = fopen(file_path, "rb");
	if (!fp) {
		return 1;
	}

	/* get file size */
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);

	bfvm_code_load_from_fileptr(vm, fp, file_size);

	fclose(fp);
	return 0;
}

void
bfvm_loop_start(BfVm *vm)
{
	if (!(vm->memory[vm->index_memory])) {
		while (vm->code[vm->index_code++] != BF_BYTE_LOOP_END) {
		}
		return;
	}

	vm->callstack[++vm->index_callstack] = vm->index_code;
}

void
bfvm_loop_exit(BfVm *vm)
{
	if (!(vm->memory[vm->index_memory])) {
		vm->index_callstack--;
		return;
	}
	vm->index_code = vm->callstack[vm->index_callstack];
	return;
}

void
bfvm_call(BfVm *vm)
{
#ifdef BF_DEBUG
	fprintf(stderr, "CALL %08X\n", ((uint)vm->code[vm->index_code + 1]));
#endif

	vm->callstack[++vm->index_callstack] = vm->index_code + BF_SIZE_ADDRESS;
	vm->index_code = (*((uint *)(vm->code + vm->index_code + 1))) - 1;
}

void
bfvm_return(BfVm *vm)
{
#ifdef BF_DEBUG
	fprintf(stderr, "RET  %08X\n", vm->callstack[vm->index_callstack]);
#endif
	vm->index_code = vm->callstack[vm->index_callstack--];
}

boolean
bfvm_exec(BfVm *vm)
{
	BfByte opcode;
	while (1) {
		opcode = vm->code[vm->index_code];
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
			if (vm->index_memory > sizeof(vm->memory)) {
				return 1;
			}
			break;
		case BF_BYTE_RIGHT:
			vm->index_memory++;
			if (vm->index_memory > sizeof(vm->memory)) {
				return 1;
			}
			break;
		case BF_BYTE_LOOP_START:
			if ((vm->index_callstack + 1) >
			    (sizeof(vm->callstack) / sizeof(uint))) {
				return 1;
			}

			bfvm_loop_start(vm);
			break;
		case BF_BYTE_LOOP_END:
			if ((vm->index_callstack + 1) >
			    (sizeof(vm->callstack) / sizeof(uint))) {
				return 1;
			}

			bfvm_loop_exit(vm);
			break;
		case BF_BYTE_PRINT:
			fputc(vm->memory[vm->index_memory], stdout);
			break;
		case BF_BYTE_READ:
			vm->memory[vm->index_memory] = fgetc(stdin);
			break;
		case BF_BYTE_CALL:
			bfvm_call(vm);
			break;
		case BF_BYTE_RETURN:
			if (!vm->index_callstack) {
				return 0;
			}

			bfvm_return(vm);
			break;
		default:
			return 1;
		}

		vm->index_code++;
	}
	return 0;
}
