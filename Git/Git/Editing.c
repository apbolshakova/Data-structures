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
	//TODO: finish; учесть, что буфер тут может быть пустым (идти только по дереву)
	/*result += getLenDiffFromVer(buf->operation); //текущая рассматриваемая функция - буфер

	version_t* ver = getVerPtr(buf->parentVerNum);
	while (verNum != NOT_DEFINED_PARENT) //пока элемент валидный
	{
		//просуммировать длины операций из этой версии
 		//новая текущая версия - родитель текущей
	}*/
		
	return result;
}