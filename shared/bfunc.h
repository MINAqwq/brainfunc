#include "types.h"

#define BF_VER_MAJOR 0
#define BF_VER_MINOR 1

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
	BF_BYTE_RETURN
} BfByte;

typedef struct {
	char  magic[4]; /* ASCII "BFUN" */
	uint8 ver_maj;
	uint8 ver_min;
	uint  size;  /* code size after header */
	uint  entry; /* entry point address */
} __attribute__((packed)) BfExecHeader;
