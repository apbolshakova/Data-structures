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

void pushIntoOpList(operation_t** root, operation_t* operation)
{
	operation_t* last = getLastOperation(root); 
	if (!last) *root = operation;
	else last->next = operation;
}

operation_t* getLastOperation(operation_t** root)
{
	operation_t* result = *root;
	if (result) while (result->next) result = result->next;
	return result;
}