#include "bfc.h"

#include <stdio.h>
#include <string.h>

boolean
bf_function_internal_strcmp(const char *str1, const char *str2)
{
	while (*str1 && *str2 && *str1 == *str2) {
		str1++;
		str2++;
	}

	return (!(*str1) && !(*str2));
}

BfFunctionRegister *
bf_function_reg_create()
{
	BfFunctionRegister *reg;

	reg = bf_malloc(sizeof(reg->size), "BfFunctionRegister Create",
			"Init Allocation");

	reg->size = 0;

	return reg;
}

void
bf_function_reg_delete(BfFunctionRegister *func_reg)
{
	bf_free(func_reg);
}

boolean
bf_function_reg_add(BfFunctionRegister **reg, char *name, uint addr)
{
	BfFunction	   *fn;
	BfFunctionRegister *func_reg;

	func_reg = *reg;

	/* check if name is higher than allowed or if name already exists */
	if (strlen(name) >= sizeof(fn->name) ||
	    bf_function_reg_search(func_reg, name)) {
		return 1;
	}

	/* increment */
	func_reg->size++;

	/* resize */
	func_reg = bf_realloc(func_reg,
			      sizeof(func_reg->size) +
				  (sizeof(func_reg->functs) * func_reg->size),
			      "BfFuncReg Add Resize",
			      "Resize function register to new size");

	/* index */
	fn = &func_reg->functs[func_reg->size - 1];

	/* add entry */
	fn->addr = addr;
	strcpy(fn->name, name);

	/* update (hacky garbage) */
	*reg = func_reg;

	return 0;
}

const BfFunction *
bf_function_reg_search(const BfFunctionRegister *func_reg, char *name)
{
	uint i;

	const BfFunction *fn;

	for (i = 0; i < func_reg->size; i++) {
		fn = &func_reg->functs[i];

		if (!bf_function_internal_strcmp(fn->name, name)) {
			continue;
		}

		return fn;
	}

	return NULL;
}
