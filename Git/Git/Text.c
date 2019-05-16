#include "Common.h"

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

int getMaxTextLen()
{
	int result = 0;
	int lenDiff = 0;
	version_t* ver = generalInfo->root;

	if (buf)
	{
		lenDiff = getMaxLenDiff(buf->operation);
		if (lenDiff > 0) result += lenDiff;
		ver = buf->parentPtr;
	}
	while (ver)
	{
		lenDiff = getMaxLenDiff(ver->operation);
		if (lenDiff > 0) result += lenDiff;
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
		if (op->type == '-') result -= op->endIndex - op->beginIndex;
		op = op->next;
	}
	return result;
}

int getMaxLenDiff(operation_t* list)
{
	int result = 0;
	operation_t* op = list;
	while (op)
	{
		if (op->type == '+') result += strlen(op->data);
		op = op->next;
	}
	return result;
}

status_t print()
{
	int stringLen = getMaxTextLen();
	char* text = (char*)calloc(stringLen + 1, sizeof(char));
	if (getCurText(text, stringLen) == FAIL)
	{
		free(text);
		printf("ERROR: unable to print text.\n");
		return FAIL;
	}
	printText(text);
	free(text);
	return SUCCESS;
}

status_t getCurText(char* text, int textLen)
{
	path_t* pathToBuf = NULL;
	if (getPath(&pathToBuf) == FAIL)
	{
		deletePath(&pathToBuf);
		printf("ERROR: unable to get path to buffer.\n");
		return FAIL;
	}
	if (applyChanges(text, textLen, pathToBuf) == FAIL)
	{
		deletePath(&pathToBuf);
		printf("ERROR: unable to apply all operations that were made.\n");
		return FAIL;
	}
	deletePath(&pathToBuf);
	return SUCCESS;
}

status_t applyChanges(char* text, int textLen, path_t* el)
{
	while (el)
	{
		if (applyVerChanges(text, textLen, el->ver->operation) == FAIL) return FAIL;
		el = el->next;
	}
	return SUCCESS;
}

status_t applyVerChanges(char* text, int textLen, operation_t* opEl)
{
	while (opEl)
	{
		if ((opEl->type == '+' && addToText(text, textLen, opEl) == FAIL))
		{
			printf("ERROR: invalid operation found.\n");
			return FALSE;
		}
		if (opEl->type == '-' && removeFromText(text, textLen, opEl) == FAIL)
		{
			printf("ERROR: invalid operation found.\n");
			return FALSE;
		}
		opEl = opEl->next;
	}
	return SUCCESS;
}

status_t addToText(char* text, int textLen, operation_t* opEl)
{
	if (!opEl || opEl->type != '+')
	{
		printf("ERROR: invalid operation.\n");
		return FAIL;
	}
	char* temp = (char*)calloc(textLen + 1, sizeof(char));
	if (!temp)
	{
		printf("ERROR: memory allocation error.\n");
		return FAIL;
	}
	strcpy(temp, text + opEl->beginIndex);
	strcpy(text + opEl->beginIndex, opEl->data);
	strcpy(text + opEl->beginIndex + strlen(opEl->data), temp);
	free(temp);
	return SUCCESS;
}

status_t removeFromText(char* text, int textLen, operation_t* opEl) //TODO: протестировать
{
	if (!opEl || opEl->type != '-')
	{
		printf("ERROR: invalid operation.\n");
		return FAIL;
	}
	char* temp = (char*)calloc(textLen + 1, sizeof(char));
	if (!temp)
	{
		printf("ERROR: memory allocation error.\n");
		return FAIL;
	}
	strcpy(temp, text + opEl->endIndex); 
	strcpy(text + opEl->beginIndex, temp);
	free(temp);
	return SUCCESS;
}

void printText(char* text)
{
    while (*text)
	{
		printf("%c", *text);
		text++;
	}
	printf("\n");
}