#include "Common.h"

status_t* getDiffArray(int** diffPtr, verList_t* path, int diffArrayLen)
{
	//�������� ��-�� �������� �� ������� (0 - ��������� ���, 1 - �����, 2 - ������)
	//������ ��-�� �������� �� ���������� ����� ��������� (1 - ����� ���� ������ �� ���������, 0 - �������� � ����� �� ������ ���, len - ����� ��������� ���������� len - 1 ��������)
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
				int index = getAdditionBegin(diff, opEl->beginIndex); //�������� ������ � basic ������, ����� �������� �������� �����
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