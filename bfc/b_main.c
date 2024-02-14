#include "bfc.h"

#include <stdio.h>
#include <stdlib.h>

char *
bf_file_read(const char *path, uint *size)
{
	FILE *fp;
	char *buffer;

	/* open file */
	fp = fopen(path, "rb");
	if (!fp) {
		return NULL;
	}

	/* get file size */
	fseek(fp, 0, SEEK_END);
	*size = ftell(fp);
	rewind(fp);

	if (*size < 1) {
		return NULL;
	}

	/* allocate memory for read buffer */
	buffer = bf_malloc(*size, "File Read Buffer",
			   "Buffer used to store file content");

	/* read file content into buffer */
	if (fread(buffer, sizeof(char), *size, fp) != (*size) * sizeof(char)) {
		bf_free(buffer);
		return NULL;
	}

	/* close file handle */
	fclose(fp);

	return buffer;
}

char *
bf_buffer_append(char *dst, char *src, uint size_dst, uint size_src)
{
	uint i;

	/* resize */
	dst = bf_realloc(dst, size_dst + size_src, "Code Array Resize",
			 "Resize code array to append new code");

	/* append */
	for (i = 0; i < size_src; i++) {
		*(dst + size_dst + i) = *(src + i);
	}

	return dst;
}

void
bf_bin_out(uint8 *out)
{
	BfExecHeader *header;

	/* obtain bfe header */
	header = ((BfExecHeader *)out);

	/* write to output */
	fwrite(out, sizeof(uint8), sizeof(*header) + header->size, stdout);

	bf_free(out);
}

int
main(int argc, char **argv)
{
	/* temporary read buffer*/
	char *buffer_tmp;
	uint  size_tmp;

	/* array containing code */
	char *code;
	uint  size_code;

	if (argc < 2) {
		fputs("error: at least one argument is required\n", stderr);
		return 1;
	}

	size_code = 0;

	code = bf_malloc(1, "Code Array", "Contains the whole code");

	/* iterate through arguments */
	while (--argc) {
		/* read file */
		buffer_tmp = bf_file_read(argv[argc], &size_tmp);
		if (!buffer_tmp) {
			fprintf(stderr, "error: failed reading from %s\n",
				argv[argc]);
			return 1;
		}

		/* append file content to buffer */
		code = bf_buffer_append(code, buffer_tmp, size_code, size_tmp);

		/* calc new size */
		size_code += size_tmp;

		/* free tmp buffer */
		bf_free(buffer_tmp);
	}

	/* output compiled bfe file */
	bf_bin_out(bf_comp_compile(code, size_code));

	bf_free(code);

	return 0;
}