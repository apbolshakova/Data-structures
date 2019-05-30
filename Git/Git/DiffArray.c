#include "Common.h"

status_t* getDiffArray(int** diffPtr, verList_t* path, int diffArrayLen)
{
	//нечётные эл-ты отвечают за символы (0 - изменений нет, 1 - удалён, 2 - изменён)
	//чётные эл-ты отвечают за расстояние между символами (1 - между ними ничего не добавлено, 0 - элемента с одной из сторон нет, len - между символами добавились len - 1 символов)
	int* diff = (int*)malloc(sizeof(int) * diffArrayLen);
	if (!diff)
	{
		printf("ERROR: memory allocation error.\n");
		return FAIL;
	}	
	initDiffArrayVal(diff, diffArrayLen);

	verList_t* posInPath = path->next;
	operation_t* opEl = posInPath->ver->operation;
	while (posInPath)
	{
		while (opEl)
		{
			if (opEl->type == '+')
			{
				int index = getAdditionBegin(diff, opEl->beginIndex); //получить индекс в basic тексте, после которого добавили текст
				diff[index] += strlen(opEl->data);
			}
			opEl = opEl->next;
		}
		posInPath = posInPath->next;
		if (posInPath) opEl = posInPath->ver->operation;
	}
	*diffPtr = diff;
	return SUCCESS;
}

void initDiffArrayVal(int* diff, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (i % 2) diff[i] = 0;
		else diff[i] = 1;
	}
}

int getAdditionBegin(int* diff, int begin)
{
	int sav = begin;
	for (int i = 0; i <= sav; i += 2)
	{
		begin -= (diff[i] - 1);
	}
	if (begin < 0) begin = 0;
	return begin * 2;
}