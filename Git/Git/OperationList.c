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

void shiftIntoOpList(operation_t** root, operation_t* operation)
{
	if (!(*root)) operation->next = NULL;
	else operation->next = *root;
	*root = operation;
	return SUCCESS;
}

operation_t* getLastOperation(operation_t** root)
{
	operation_t* result = *root;
	if (result) while (result->next) result = result->next;
	return result;
}

status_t printOperations(FILE* file, operation_t* opListFromVer)
{
	operation_t* op = opListFromVer;
	if (!opListFromVer) op = buf->operation;
	while (op)
	{
		if (op->type == '+') fprintf(file, "+ %i %s\n", op->beginIndex, op->data);
		else if (op->type == '-') fprintf(file, "- %i %i %s\n", op->beginIndex, op->endIndex, op->data);
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
			fscanf(file, "%i %i ", &(op->beginIndex), &(op->endIndex));
			op->data = (char*)calloc(op->endIndex - op->beginIndex + 1, sizeof(char));
			fscanf(file, "%s", op->data);
		}
		else deleteOperationList(root);
		if (!prev) *root = op;
		else prev->next = op;
		prev = op;
	}
	return SUCCESS;
}

status_t appendOpList(operation_t** opListRoot, operation_t* appendOpList)
{
	if (!appendOpList) return SUCCESS;
	operation_t* opEl = getLastOperation(opListRoot);
	while (appendOpList)
	{
		operation_t* nextOpEl = (operation_t*)malloc(sizeof(operation_t));
		if (!nextOpEl)
		{
			printf("ERROR: memory allocation problem.\n");
			return FAIL;
		}
		memcpy(nextOpEl, appendOpList, sizeof(operation_t));
		if (!opEl)
		{
			*opListRoot = nextOpEl;
			opEl = *opListRoot;
		}
		else
		{
			opEl->next = nextOpEl;
			opEl = opEl->next;
		}
		appendOpList = appendOpList->next;
	}
	return SUCCESS;
}

status_t reverseVerOperations(version_t* newRoot)
{
	//делать реверс в версии, менять местами реверс версии и temp 
	operation_t* temp = NULL;
	if (appendOpList(&temp, newRoot->operation) == FAIL)
	{
		printf("ERROR: unable to copy version's operation list.\n");
		return FAIL;
	}
	if (reverseOpList(&(temp)) == FAIL) //Реверс операций
	{
		printf("ERROR: unable to reverse operations.\n");
		return FAIL;
	}
	version_t* el = newRoot;
	int stringLen = getMaxTextLen(newRoot);
	char* text = (char*)calloc(stringLen + 1, sizeof(char));
	if (getCurText(text, stringLen, newRoot, NULL) == FAIL)
	{
		free(text);
		return FAIL;
	}
	deleteOperationList(&(newRoot->operation));
	if (add(0, text, newRoot) == FAIL)
	{
		free(text);
		return FAIL;
	}

	el = newRoot->parentPtr;
	while (el)
	{
		if (reverseOpList(&(el->operation)) == FAIL) //Реверс операций
		{
			printf("ERROR: unable to reverse operations.\n");
			return FAIL;
		}
		swapOpLists(&temp, &(el->operation));
		el = el->parentPtr;
	}
	return SUCCESS;
}

status_t reverseOpList(operation_t** opList)
{
	operation_t* reversedListRoot = NULL;
	operation_t* op = *opList;
	while (op)
	{
		operation_t* reversedOp = NULL;
		if (getReversedOperation(&reversedOp, op) == FAIL)
		{
			printf("ERROR: unable to reverse operation.\n");
			deleteOperationList(&reversedListRoot);
		}
		shiftIntoOpList(&reversedListRoot, reversedOp);
		op = op->next;
	}
	deleteOperationList(opList);
	*opList = reversedListRoot;
	return SUCCESS;
}

status_t getReversedOperation(operation_t** result, operation_t* src)
{
	operation_t* reversed = (operation_t*)malloc(sizeof(operation_t));
	if (!reversed)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	if (src->type == '+')
	{
		reversed->type = '-';
		reversed->beginIndex = src->beginIndex;
		reversed->endIndex = src->beginIndex + strlen(src->data);
	}
	else if (src->type == '-')
	{
		reversed->type = '+';
		reversed->beginIndex = src->beginIndex;
		reversed->endIndex = INVALID_INDEX;
	}
	reversed->data = (char*)calloc(strlen(src->data) + 1, sizeof(char));
	strcpy(reversed->data, src->data);
	reversed->next = NULL;
	*result = reversed;
	return SUCCESS;
}

void swapOpLists(operation_t** opList1, operation_t** opList2)
{
	operation_t* temp = *opList1;
	*opList1 = *opList2;
	*opList2 = temp;
}