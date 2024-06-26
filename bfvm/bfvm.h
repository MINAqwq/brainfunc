#include "../shared/types.h"

/* ================= Virtual Machine ================ */

typedef struct __attribute__((packed)) {
	uint8 memory[8096];    /* memory */
	uint  index_code;      /* instruction pointer */
	uint  index_memory;    /* current memory position */
	uint  index_callstack; /* index to current callstack position */
	uint  callstack[64];   /* callstack for address saving */
	uint  code_size;       /* size of program code */
	uint8 code[1];	       /* program code */
} BfVm;

/* allocate an instance of the brainfunc vm (will copy code) */
BfVm *bfvm_create();

/* free allocated memory */
void bfvm_delete(BfVm *vm);

/* load brainfunc code from stdin
 * TODO: only read code specified by the header and not by eof
 */
boolean bfvm_code_load_from_stdin(BfVm **vm);

/* load brainfunc code from a file directly into the code buffer */
boolean bfvm_code_load_from_file(BfVm **vm, const char *file_path);

/* run code from entry */
boolean bfvm_exec(BfVm *vm);

/* ==================== Syscalls ==================== */

typedef enum {
	BF_SYS_OPEN = 0,
	BF_SYS_CLOSE,
	BF_SYS_READ,
	BF_SYS_WRITE
} BfSyscall;

boolean bfvm_syscall_exec(BfVm *vm);
