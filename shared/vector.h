#ifndef SHARED_VECTOR_H
#define SHARED_VECTOR_H

#include "types.h"

typedef struct {
	uint64 size;
	uint64 allocated;
	void  *data;
} BfVector;

void bf_vector_create(BfVector *);

void bf_vector_delete(BfVector *v);

boolean bf_vector_insert(BfVector *v, const void *data, uint64 data_size,
			 uint64 pos);

boolean bf_vector_append(BfVector *v, const void *data, uint64 data_size);

boolean bf_vector_get(const BfVector *v, void *data, uint64 data_size,
		      uint64 pos);

boolean bf_vector_back(const BfVector *v, void *data, uint64 data_size);

#endif
