#include "types.h"

/* ==================== xMemory ==================== */

/* debug informations */
typedef struct {
	const char *lstr_name;	      /* allocation name */
	const char *lstr_description; /* allocation description */
	const char *lstr_file;	      /* __FILE__ */
	int	    i_line;	      /* __LINE__ */
} __attribute__((packed)) BfDebugInfos;

/* malloc wrapper with error checking */
void *bf_xmalloc(uint size);

/* calloc wrapper with error checking */
void *bf_xcalloc(uint n, uint size);

/* realloc wrapper with error checking */
void *bf_xrealloc(void *ptr_old, uint size_new);

/* malloc wrapper with error checking and debug tracing */
void *bf_xmalloc_debug(uint size, const char *name, const char *description,
		       const char *file, int line);

/* calloc wrapper with error checking and debug tracing */
void *bf_xcalloc_debug(uint n, uint size, const char *name,
		       const char *description, const char *file, int line);

/* realloc wrapper with error checking and debug tracing */
void *bf_xrealloc_debug(void *ptr_old, uint size_new, const char *name,
			const char *description, const char *file, int line);

/* free wrapper with debug tracing*/
void bf_free_debug(void *ptr, const char *file, int line);

#ifndef BF_DEBUG
#define bf_malloc(size, name, description)                                     \
	bf_xmalloc(size);                                                      \
	(void)name;                                                            \
	(void)description

#define bf_calloc(n, size, name, description)                                  \
	bf_xcalloc(n, size);                                                   \
	(void)name;                                                            \
	(void)description

#define bf_realloc(ptr_old, size_new, name, description)                       \
	bf_xrealloc(ptr_old, size_new);                                        \
	(void)name;                                                            \
	(void)description

#define bf_free(ptr) free(ptr)

#else
#define bf_malloc(size, name, description)                                     \
	bf_xmalloc_debug(size, name, description, __FILE__, __LINE__)

#define bf_calloc(n, size, name, description)                                  \
	bf_xcalloc_debug(n, size, name, description, __FILE__, __LINE__)

#define bf_realloc(ptr_old, size_new, name, description)                       \
	bf_xrealloc_debug(ptr_old, size_new, name, description, __FILE__,      \
			  __LINE__)

#define bf_free(ptr) bf_free_debug(ptr, __FILE__, __LINE__)

#endif
