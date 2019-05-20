#include "Common.h"

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

status_t printOperations(FILE* file)
{
	operation_t* op = buf->operation;
	while (op)
	{
		if (op->type == '+') fprintf(file, "+ %i %s\n", op->beginIndex, op->data);
		else if (op->type == '-') fprintf(file, "- %i %i\n", op->beginIndex, op->endIndex);
		else return FAIL;
		op = op->next;
	}
	return SUCCESS;
}

status_t getOperationList(operation_t** root, FILE* file)
{
	if (!file) return FAIL;
    char buf[TEMP_LEN] = { 0 };
	
	operation_t* prev = NULL;
	while (fscanf_s(file, "%s", buf, TEMP_LEN) != EOF)
	{
		operation_t* op = (operation_t*)malloc(sizeof(operation_t));
		if (!op)
		{
			printf("ERROR: memory allocation error.\n");
			return FAIL;
		}
		op->next = NULL;
		if (*buf == '+')
		{
			op->type = '+';
			fscanf_s(file, "%s", buf, TEMP_LEN);
			op->beginIndex = atoi(buf);
			op->endIndex = INVALID_INDEX;
			if (getDataFromFile(&(op->data), file) == FAIL)
			{
				free(op);
				deleteOperationList(root);
				printf("ERROR: unable to load operations.\n");
				return FAIL;
			}
		}
		else if (*buf == '-')
		{
			op->type = '-';
			fscanf_s(file, "%i %i", &(op->beginIndex), &(op->endIndex));
			op->data = NULL;
		}
		else deleteOperationList(root);
		if (!prev) *root = op;
		else prev->next = op;
		prev = op;
	}
	return SUCCESS;
}

status_t copyOpList(operation_t* opListRoot, operation_t* sourceOpList)
{

}
