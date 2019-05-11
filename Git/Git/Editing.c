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
	//TODO
}