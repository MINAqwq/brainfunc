#include <stdlib.h>

#include "../shared/bfunc.h"
#include "../shared/xmemory.h"

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
					 char			  *name);
