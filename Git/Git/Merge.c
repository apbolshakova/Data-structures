#include "Common.h"

status_t handleMerging()
{
	int i = 0;
	printf("Enter version number or any negative number to cancel (no number -> index = 0): ");
	fflush(stdin);
	scanf_s("%i", &i);
	if (i < 0) return SUCCESS;
	if (merge(i) == FAIL)
	{
		printf("ERROR: unable to delete version.\n");
		return FAIL;
	}
}

status_t merge(int verNum)
{
	verList_t* pathToBuf = NULL;
	verList_t* pathToVer = NULL;
	int* bufOffset = NULL;
	int* verOffset = NULL;

	if (getPath(&pathToBuf, NULL) == FAIL)
	{
		printf("ERROR: unable to get path to buffer.\n");
		goto Fail;
	}

	version_t* ver = getVerPtr(generalInfo->root, verNum);
	if (!ver)
	{
		printf("ERROR: unable to get version to merge with.\n");
		goto Fail;
	}
	if (getPath(&pathToVer, ver) == FAIL)
	{
		printf("ERROR: unable to get path to version to merge with.\n");
		goto Fail;
	}

	int offsetsArrLen = 0;
	if (generalInfo->root->operation)
	{
		offsetsArrLen = strlen(generalInfo->root->operation->data) + 1;
	}
	bufOffset = getOffsets(pathToBuf, offsetsArrLen);
	if (!bufOffset)
	{
		printf("ERROR: unable to compare versions.\n");
		goto Fail;
	}
	verOffset = getOffsets(pathToVer, offsetsArrLen);
	if (!verOffset)
	{
		printf("ERROR: unable to compare versions.\n");
		goto Fail;
	}

	/*if (addOperationsForMerging(bufOffset, verOffset) == FAIL) //add to buffer required for merge operations
	{
		printf("ERROR: unable to merge.\n");
		goto Fail;
	}*/

	if (pathToBuf) deletePath(&pathToBuf);
	if (pathToVer) deletePath(&pathToVer);
	if (bufOffset) free(bufOffset);
	if (verOffset) free(verOffset);
	return SUCCESS;

Fail:
	if (pathToBuf) deletePath(&pathToBuf);
	if (pathToVer) deletePath(&pathToVer);
	if (bufOffset) free(bufOffset);
	if (verOffset) free(verOffset);
	return FAIL;
}

int* getOffsets(verList_t* path, int size)
{
	int* offset = (int*)malloc(size * sizeof(int));
	if (!offset)
	{
		printf("ERROR: memory allocation problem.\n");
		return NULL;
	}
	for (int i = 0; i < size; i++) offset[i] = i;
	while (path)
	{
		if (path->ver == generalInfo->root) //no need to handle root's first operation
			updateOffsetsForVer(path->ver->operation->next, offset, size);
		else
			updateOffsetsForVer(path->ver->operation, offset, size);
		path = path->next;
	}
	return offset;
}

void updateOffsetsForVer(operation_t* opEl, int* offset, int size)
{
	while (opEl)
	{
		if (opEl->type == '+')
		{
			int i = findClosestIndex(opEl->beginIndex, offset, size);
			int opLen = strlen(opEl->data);
			for (i; i < size; i++) offset[i] += opLen;
		}
		if (opEl->type == '-')
		{
			int i = findClosestIndex(opEl->beginIndex, offset, size);
			while (opEl->beginIndex <= i && i < opEl->endIndex)
			{
				offset[i] = DELETED;
				i++;
			}
			for (; i < size; i++)
			{
				offset[i] -= opEl->endIndex - opEl->beginIndex;
			}
		}
		opEl = opEl->next;
	}
}

int findClosestIndex(int valueToFind, int* array, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (valueToFind <= array[i]) return i;
	}
	return size; //adding to the end
}