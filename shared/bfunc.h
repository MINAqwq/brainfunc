#include "types.h"

#ifndef BF_SHARED_BFUNC_H
#define BF_SHARED_BFUNC_H

#define BF_VER_MAJOR 0
#define BF_VER_MINOR 2

#define BF_SIZE_ADDRESS sizeof(uint)
#define BF_SIZE_OPCODE	sizeof(uint8)

#define BF_FUNCTION_NAME_LEN_MAX 47

#define BF_NAME_ENTRY "main"

/* BrainFunc Opcodes */
typedef enum {
	BF_BYTE_NONE = 0,
	BF_BYTE_ADD,
	BF_BYTE_SUB,
	BF_BYTE_LEFT,
	BF_BYTE_RIGHT,
	BF_BYTE_LOOP_START,
	BF_BYTE_LOOP_END,
	BF_BYTE_PRINT,
	BF_BYTE_READ,
	BF_BYTE_CALL,
	BF_BYTE_RETURN,
	BF_BYTE_COPY,
	BF_BYTE_SYSCALL
} BfByte;

/* TODO for next version: change magic bytes to 0x0B 'F' 'U' 'N'*/
typedef struct {
	char  magic[4]; /* ASCII "BFUN" */
	uint8 ver_maj;
	uint8 ver_min;
	uint  size;  /* code size after header */
	uint  entry; /* entry point address */
} __attribute__((packed)) BfExecHeader;

#endif
