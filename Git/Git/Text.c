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
	printText(text);
	free(text);
	return SUCCESS;
}

func_res_t getCurText(char* text)
{
	path_t* pathToBuf = NULL;
	if (getPath(&pathToBuf) == FAIL)
	{
		printf("ERROR: unable to get path to buffer.\n");
		return FAIL;
	}
	if (applyChanges(text, pathToBuf) == FAIL)
	{
		printf("ERROR: unable to apply all operations that were made.\n");
		return FAIL;
	}
	//TODO: not forget to clean pathToBuf
	return SUCCESS;
}

func_res_t applyChanges(char* text, path_t* el)
{
	while (el)
	{
		if (applyVerChanges(text, el->ver->operation) == FAIL) return FAIL;
		el = el->next;
	}
	return SUCCESS;
}

func_res_t applyVerChanges(char* text, operation_t* opEl)
{
	while (opEl)
	{
		     if (opEl->type == '+') addToText(text, opEl);
		else if (opEl->type == '-') removeFromText(text, opEl);
		else
		{
			printf("ERROR: invalid operation found.\n");
			return FALSE;
		}
		opEl = opEl->next;
	}
	return SUCCESS;
}

void printText(char* text)
{
    while (*text)
	{
		printf("%c", *text);
		text++;
	}
}