#include "Header.h"

func_result_t add(int i, char* data)
{
	if (!indexIsCorrect(i)) 
	{
		printf("ERROR: Invalid index for starting position.\n");
		return FAIL;
	}

	//создать операцию
	operation_t* opBuf = (operation_t*)malloc(sizeof(operation_t));
	if (!opBuf)
	{
		printf("ERROR: Memory allocation problem.\n");
		return FAIL;
	}
	opBuf->type = '+';
	opBuf->beginIndex = i;
	opBuf->endIndex = INVALID_INDEX;
	opBuf->data = data;

	//добавить операцию в линейный список буфера
	if (!pushIntoOpList(opBuf) == FAIL)
	{
		printf("ERROR: Unable to push into operation list.\n");
		return FAIL;
	}
	return SUCCESS;
}

bool_t indexIsCorrect(int i)
{
	int textLen = getTextLen();
	if (i > textLen) return FALSE;
	return TRUE;
}

int getTextLen()
{ 
	int result = 0;
	version_t* ver = generalInfo->root;

	if (buf)
	{
		result += getLenDiff(buf->operation);
		ver = buf->parentPtr;
	}
	while (ver)
	{
		result += getLenDiffFromVer(ver->operation); //просуммировать длины операций из этой версии
		ver = ver->parentPtr; //новая текущая версия - родитель текущей
	}
	return result;
}

int getLenDiff(operation_t* list)
{
	int result = 0;
	operation_t* op = list;
	while (op)
	{
		if (op->type == '+') result += strlen(op->data);
		if (op->type == '-') result -= op->endIndex + op->beginIndex;
		op = op->next;
	}
	return result;
}