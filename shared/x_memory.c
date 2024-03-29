#include "xmemory.h"

#include <stdio.h>
#include <stdlib.h>

#define OFFSET_NAME_PTR 0
#define OFFSET_DESC_PTR sizeof(const char *)
#define OFFSET_FILE_PTR (sizeof(const char *) + sizeof(const char *))
#define OFFSET_LINE                                                            \
	(sizeof(const char *) + sizeof(const char *) + sizeof(const char *))

void *
bf_xmalloc(uint size)
{
	void *ptr;

	ptr = malloc(size);
	if (!ptr) {
		fputs("xmalloc failed\n", stderr);
		exit(1);
	}

	return ptr;
}

void *
bf_xcalloc(uint n, uint size)
{
	void *ptr;

	ptr = calloc(n, size);
	if (!ptr) {
		fputs("xcalloc failed\n", stderr);
		exit(1);
	}

	return ptr;
}

void *
bf_xrealloc(void *ptr_old, uint size_new)
{

	ptr_old = realloc(ptr_old, size_new);
	if (!ptr_old) {
		fputs("xrealloc failed\n", stderr);
		exit(1);
	}

	return ptr_old;
}

void *
bf_xmalloc_debug(uint size, const char *name, const char *description,
		 const char *file, int line)
{
	void	    *ptr;
	BfDebugInfos dbg;

	fprintf(stderr, "%s:%03d\tMALLOC [%04d]\t%s (%s)\n", file, line, size,
		name, description);

	dbg.lstr_name = name;
	dbg.lstr_description = description;
	dbg.lstr_file = file;
	dbg.i_line = line;

	/* allocate size + space for debug stuff */
	ptr = bf_xmalloc(sizeof(dbg) + size);

	*((BfDebugInfos *)ptr) = dbg;

	return ptr + sizeof(dbg);
}

void *
bf_xcalloc_debug(uint n, uint size, const char *name, const char *description,
		 const char *file, int line)
{
	void	    *ptr;
	BfDebugInfos dbg;

	fprintf(stderr, "%s:%03d\tCALLOC [%04d]\t%s (%s)\n", file, line, size,
		name, description);

	dbg.lstr_name = name;
	dbg.lstr_description = description;
	dbg.lstr_file = file;
	dbg.i_line = line;

	/* allocate size + space for debug stuff */
	ptr = bf_xcalloc(n, sizeof(dbg) + size);

	*((BfDebugInfos *)ptr) = dbg;

	return ptr + sizeof(dbg);
}

void *
bf_xrealloc_debug(void *ptr_old, uint size_new, const char *name,
		  const char *description, const char *file, int line)
{
	void	     *ptr;
	BfDebugInfos *dbg;

	dbg = (ptr_old - sizeof(*dbg));

	fprintf(stderr, "%s:%03d\tREALLOC[%04d]\t%s (%s) [%s]\n", file, line,
		size_new, name, description, dbg->lstr_name);

	ptr = bf_xrealloc(dbg, size_new + sizeof(*dbg));

	return ptr + sizeof(*dbg);
}

void
bf_free_debug(void *ptr, const char *file, int line)
{
	BfDebugInfos *dbg;

	dbg = (ptr - sizeof(*dbg));

	fprintf(stderr, "%s:%03d\tFREE (%s)\n", file, line, dbg->lstr_name);

	free(ptr - sizeof(*dbg));
}
