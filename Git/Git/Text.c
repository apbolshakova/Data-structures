#include "Header.h"

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
		result += getLenDiff(ver->operation); //просуммировать длины операций из этой версии
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

func_res_t print()
{
	int textLen = getTextLen();
	char* text = (char*)malloc(sizeof(char) * textLen + 1);
	if (getCurText(text) == FAIL)
	{
		free(text);
		printf("ERROR: unable to print text.\n");
		return FAIL;
	}
	//printText(text); TODO
	free(text);
	return SUCCESS;
}

func_res_t getCurText(char* text)
{
	path_t* pathToBuf = NULL;
	if (getPath(&pathToBuf) == FAIL)
	{
		printf("ERROR: unable to get text as it's look now.\n");
		return FAIL;
	}
	//applyOperationsToText(text); //пройти по пути и собрать операции, применяя их к text TODO

	//TODO: not forget to clean pathToBuf
	return SUCCESS;
}