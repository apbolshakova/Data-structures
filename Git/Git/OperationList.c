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

func_res_t pushIntoOpList(operation_t** root, operation_t* operation)
{
	//добавить новый элемент 
	return SUCCESS;
}