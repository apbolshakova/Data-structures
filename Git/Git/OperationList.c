#include "Header.h"

void deleteOperationList(operation_t** root)
{
	operation_t* temp = *root;
	while (*root)
	{
		*root = (*root)->next;
		free(temp);
		temp = NULL;
		temp = *root;
	}
}

func_result_t pushIntoOpList(operation_t* opBuf)
{
	//TODO
}