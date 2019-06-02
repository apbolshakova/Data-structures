#include "Common.h"

status_t handleMerging()
{
	int i = 0;
	printf("Enter version number or any negative number to cancel (no number -> index = 0): ");
	fflush(stdin);
	scanf_s("%i", &i);
	if (i < 0) return SUCCESS;
	if (i == buf->parentVerNum)
	{
		printf("ERROR: attempt to merge version with itself.\n");
		return FAIL;
	}
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

	if (areOnSamePath(pathToBuf, pathToVer))
	{
		printf("ERROR: merged version will contain no changing.\n");
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

	if (saveMergeInBuffer(bufOffset, verOffset, offsetsArrLen, ver) == FAIL) //recreate buffer and add operations to get merged text
	{
		printf("ERROR: unable to merge.\n");
		goto Fail;
	}

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

status_t saveMergeInBuffer(int* bufOffset, int* verOffset, int size, version_t* ver)
{
	int bufStrLen = getMaxTextLen(NULL);
	char* bufText = (char*)calloc(bufStrLen + 1, sizeof(char));
	if (getCurText(bufText, bufStrLen, NULL, NULL) == FAIL)
	{
		free(bufText);
		printf("ERROR: unable to get text to define merge operations.\n");
		return FAIL;
	}

	int verStrLen = getMaxTextLen(ver);
	char* verText = (char*)calloc(verStrLen + 1, sizeof(char));
	if (getCurText(verText, verStrLen, ver, NULL) == FAIL)
	{
		free(bufText);
		free(verText);
		printf("ERROR: unable to get text to define merge operations.\n");
		return FAIL;
	}

	if (initBuf(buf->parentVerNum) == FAIL)
	{
		printf("ERROR: unable to recreate buffer.\n");
		return FAIL;
	}
	if (remove(0, strlen(bufText)) == FAIL)
	{
		free(bufText);
		free(verText);
		return FAIL;
	}

	int addIndex = 0; //offset for addition into result
	for (int i = 0; i < size; i++)
	{
		if (verOffset[i] == DELETED && bufOffset[i] == DELETED) continue;
		int nextAddIndex = addIndex;
		
		//copy first char itself
		if (verOffset[i] != DELETED && bufOffset[i] != DELETED && i != size - 1)
		{
			nextAddIndex++;
			if (add(addIndex, bufText + bufOffset[i], 1, NULL) == FAIL)
			{
				free(bufText);
				free(verText);
				return FAIL;
			}
		}
		
		if (i == 0)
		{
			//copy text before first char from buf
			int len = bufOffset[i] + 1;
			if (len > 0)
			{
				nextAddIndex += len;
				if (add(addIndex, bufText, len, NULL) == FAIL)
				{
					free(bufText);
					free(verText);
					return FAIL;
				}
			}
			//copy text before first char from ver
			len = verOffset[i] + 1;
			if (len > 0)
			{
				nextAddIndex += len;
				if (add(addIndex, verText, len, NULL) == FAIL)
				{
					free(bufText);
					free(verText);
					return FAIL;
				}
			}
		}
		else
		{
			//copy text before char from buf
			int len = bufOffset[i] - bufOffset[i - 1] - 1;
			if (len > 1)
			{
				nextAddIndex += len;
				if (add(addIndex, bufText + bufOffset[i - 1] + 1, len, NULL) == FAIL)
				{
					free(bufText);
					free(verText);
					return FAIL;
				}
			}
			//copy text before char from ver
			len = verOffset[i] - verOffset[i - 1] - 1;
			if (len > 1)
			{
				nextAddIndex += len;
				if (add(addIndex, verText + verOffset[i - 1] + 1, len, NULL) == FAIL)
				{
					free(bufText);
					free(verText);
					return FAIL;
				}
			}
		}
		addIndex = nextAddIndex;
	}
	return SUCCESS;
}

bool_t areOnSamePath(verList_t* pathToBuf, verList_t* pathToVer)
{
	while (pathToBuf && pathToVer && pathToBuf->ver == pathToVer->ver)
	{
		pathToBuf = pathToBuf->next;
		pathToVer = pathToVer->next;
	}
	if (!pathToBuf || !pathToVer || pathToBuf->ver == buf) return TRUE;
	return FALSE;
}