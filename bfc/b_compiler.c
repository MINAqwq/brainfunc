#include "bfc.h"

#include <memory.h>
#include <stdio.h>

boolean
bf_comp_symbol_is_ignored(char symbol)
{
	return (symbol == ' ' || symbol == '\n' || symbol == '\r' ||
		symbol == '\t' || symbol == 0);
}

char *
bf_comp_resolve_addr_get(const char *code, uint size, uint *i)
{
	uint  j;
	char *buffer_name;

	buffer_name = bf_malloc(BF_FUNCTION_NAME_LEN_MAX + 1, "Name Buffer",
				"Fixed size buffer for a brainfunc label");

	for (j = 0; j < BF_FUNCTION_NAME_LEN_MAX; j++) {

		/* prevent going out of buffer */
		if (*i == size) {
			break;
		}

		/* name terminator */
		if (code[j] == '#') {
			buffer_name[j] = 0;
			return buffer_name;
		}

		/* copy into name buffer */
		buffer_name[j] = code[j];

		/* skip bytes in main loop */
		(*i)++;
	}

	bf_free(buffer_name);
	fputs("error: failed to register a function\n", stderr);
	exit(1);
}

void
bf_comp_resolve_addr(BfFunctionRegister **reg, const char *code, uint size,
		     uint *buffer_size)
{
	uint  i;
	uint  addr;
	char *buffer_name;

	addr = 0;

	for (i = 0; i < size; i++) {
		/* fprintf(stderr, "%02d: %c\n", i, code[i]); */

		if (bf_comp_symbol_is_ignored(code[i])) {
			continue;
		}

		if (code[i] == '$') {
			/* skip $ */
			i++;

			/* get function name */
			buffer_name =
			    bf_comp_resolve_addr_get(code + i, size, &i);

			/* add function */
			bf_function_reg_add(reg, buffer_name, addr);

			/* cleanup */
			bf_free(buffer_name);
			buffer_name = NULL;

			continue;
		}

		/* jump */
		if (code[i] == '@') {
			/* skip label */
			while (code[++i] != '#') {
			}

			/* correct address */
			addr += (BF_SIZE_ADDRESS + BF_SIZE_OPCODE);
			continue;
		}

		addr++;
	}

	/* highest address is also the number of bytes needed for the compiled
	 * code hihi */
	*buffer_size = addr;
}

void
bf_comp_header_build(const BfFunctionRegister *reg, uint size_code,
		     BfExecHeader *header)
{
	const BfFunction *fn;

	fn = bf_function_reg_search(reg, BF_NAME_ENTRY);
	if (!fn) {
		fprintf(stderr, "error: label %s is not defined\n",
			BF_NAME_ENTRY);
		exit(1);
	}

	header->magic[0] = 'B';
	header->magic[1] = 'F';
	header->magic[2] = 'U';
	header->magic[3] = 'N';

	header->ver_maj = BF_VER_MAJOR;
	header->ver_min = BF_VER_MINOR;

	header->size = size_code;

	header->entry = fn->addr;
}

uint8
bf_comp_map_get(char opcode)
{
	switch (opcode) {
	case '+':
		return BF_BYTE_ADD;
	case '-':
		return BF_BYTE_SUB;
	case '<':
		return BF_BYTE_LEFT;
	case '>':
		return BF_BYTE_RIGHT;
	case '[':
		return BF_BYTE_LOOP_START;
	case ']':
		return BF_BYTE_LOOP_END;
	case '.':
		return BF_BYTE_PRINT;
	case ',':
		return BF_BYTE_READ;
	case '@':
		return BF_BYTE_CALL;
	case '!':
		return BF_BYTE_RETURN;
	default:
		return BF_BYTE_NONE;
	}
}

uint8 *
bf_comp_map(const char *ascii_code, uint size, uint8 *buffer_compiled,
	    const BfFunctionRegister *reg)
{
	uint i;
	char tmp_op;

	char *buffer_name;

	const BfFunction *fn;

	/* iterate through ascii code */
	for (i = 0; i < size; i++) {
		if (bf_comp_symbol_is_ignored(ascii_code[i])) {
			continue;
		}

		/* skip label definitions */
		if (ascii_code[i] == '$') {
			/* skip label */
			while (ascii_code[i++] != '#') {
			}

			continue;
		}

		/* map ascii to opcode */
		tmp_op = bf_comp_map_get(ascii_code[i]);

		/* error handling */
		if (tmp_op == BF_BYTE_NONE) {
			fprintf(stderr, "error: failed to map -> 0x%02X\n",
				ascii_code[i]);
			exit(1);
		}

		/* add opcode */
		*buffer_compiled = tmp_op;
		buffer_compiled++;

		if (tmp_op != BF_BYTE_CALL) {
			continue;
		}

		i++;
		/* get name */
		buffer_name =
		    bf_comp_resolve_addr_get(ascii_code + i, size, &i);

		/* get address by name */
		fn = bf_function_reg_search(reg, buffer_name);
		if (!fn) {
			fprintf(stderr, "error: label %s does not exist\n",
				buffer_name);
			exit(1);
		}

		bf_free(buffer_name);

		/* append address */
		*((uint *)buffer_compiled) = fn->addr;
		buffer_compiled += BF_SIZE_ADDRESS;

		printf("buffer = %08X\n", fn->addr);
	}

	return buffer_compiled;
}

uint8 *
bf_comp_compile(const char *code, uint size)
{
	BfFunctionRegister *reg;
	BfExecHeader	    header;
	uint8	      *buffer_compiled;
	uint		    code_size;

	reg = bf_function_reg_create();

	/* STEP 1: map all label to their addresses */
	bf_comp_resolve_addr(&reg, code, size, &code_size);

	/* STEP 2: build header */
	bf_comp_header_build(reg, code_size, &header);

	/* allocate space for compiled code */
	buffer_compiled = bf_malloc(
	    sizeof(header) + (code_size), "Header + Compiled code buffer",
	    "buffer for the compiled Brainfunc code and its header");

	/* write header to start of buffer */
	*((BfExecHeader *)buffer_compiled) = header;

	/* STEP 3: map all valid ascii chars to bfunc opcodes */
	bf_comp_map(code, size, buffer_compiled + sizeof(header), reg);

	bf_function_reg_delete(reg);

	return buffer_compiled;
}
