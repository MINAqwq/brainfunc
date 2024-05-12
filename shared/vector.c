#include "vector.h"
#include "xmemory.h"

#include <stdio.h>

void
bf_vector_create(BfVector *v)
{
	v->allocated = 16;
	v->size = 0;
	v->data = bf_malloc(16, "Vector", "Vector data memory");
}

void
bf_vector_delete(BfVector *v)
{
	if (!v || !v->allocated) {
		return;
	}

	bf_free(v->data);
	v->allocated = 0;
	v->size = 0;
}

boolean
bf_vector_insert(BfVector *v, const void *data, uint64 data_size, uint64 pos)
{
	uint64 i;

	if (!v || !data || !data_size) {
		return 1;
	}

	/* exponentially allocate memory if not enough is allocated */
	while (v->allocated <= ((pos + 1) * data_size)) {
		v->allocated *= 2;
		v->data = bf_realloc(v->data, v->allocated, "Vector Resize",
				     "Exponential vector resize");
	}

	/* copy bytes */
	for (i = 0; i < data_size; i++) {
		*((uint8 *)v->data + ((pos * data_size) + i)) =
		    *(((uint8 *)data) + i);
	}

	if (pos <= v->size) {
		v->size = (pos == v->size) ? v->size + 1 : v->size;
	} else {
		v->size = pos;
	}

	return 0;
}

boolean
bf_vector_append(BfVector *v, const void *data, uint64 data_size)
{
	return bf_vector_insert(v, data, data_size, v->size);
}

boolean
bf_vector_get(const BfVector *v, void *data, uint64 data_size, uint64 pos)
{
	uint64 i;

	if (!v || !data || !data_size) {
		return 1;
	}

	/* copy bytes */
	for (i = 0; i < data_size; i++) {
		*(((uint8 *)data) + i) =
		    *((uint8 *)v->data + ((pos * data_size) + i));
	}

	return 0;
}

boolean
bf_vector_back(const BfVector *v, void *data, uint64 data_size)
{
	return bf_vector_get(v, data, data_size, v->size);
}