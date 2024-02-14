#include <stdlib.h>

#include "../shared/xmemory.h"

typedef unsigned char  boolean;
typedef unsigned char  uint8;
typedef unsigned short uint16;

#define BF_VER_MAJOR 0
#define BF_VER_MINOR 1

#define BF_SIZE_ADDRESS sizeof(uint)
#define BF_SIZE_OPCODE	sizeof(uint8)

#define BF_FUNCTION_NAME_LEN_MAX 47

#define BF_NAME_ENTRY "main"

/* ==================== Argparse ==================== */

typedef struct {
	uint   size;
	char **start;
} BfArgsStrings;

/* parse arguments */
boolean bf_arg_parse(int argc, char **argv);

/* returns files to compile or NULL if not used */
BfArgsStrings *bf_arg_flag_compile();

/* return output file path or NULL if not used */
const char *bf_arg_flag_output();

/* ==================== Compiler ==================== */

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

/* compile ascii/utf-8 code to byte code the vm can execute.
 * byte code starts with header.
 */
uint8 *bf_comp_compile(const char *code, uint size);

/* ================ Function Register ================ */

/* A BrainFunc function holds a name and an address */
typedef struct {
	char name[BF_FUNCTION_NAME_LEN_MAX + 1]; /* null delimited */
	uint addr;				 /* address */
} BfFunction;

/* dynamic array holding all registered functions */
typedef struct {
	uint	   size;      /* size of array */
	BfFunction functs[1]; /* offset for dynamic array */
} BfFunctionRegister;

/* create a new BfFunctionRegister */
BfFunctionRegister *bf_function_reg_create();

/* delete a BfFunctionRegister */
void bf_function_reg_delete(BfFunctionRegister *func_reg);

/* add a function to the register (returns true on error) */
boolean bf_function_reg_add(BfFunctionRegister **func_reg, char *name,
			    uint addr);

/* search a function in the function register */
const BfFunction *bf_function_reg_search(const BfFunctionRegister *func_reg,
					 char		      *name);
