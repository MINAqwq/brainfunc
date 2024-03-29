#include "types.h"

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
