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
		printf("ERROR: unable to merge.\n");
		return FAIL;
	}
}

status_t merge(int verNum)
{
	verList_t* pathToBuf = NULL;
	verList_t* pathToVer = NULL;
	int* bufOffset = NULL;
	int* verOffset = NULL;
	char* text = NULL;

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
		goto Fail;
	}
	removeSamePartOfPath(&pathToBuf, &pathToVer);

	int stringLen = getMaxTextLen(pathToBuf->ver);
	text = (char*)calloc(stringLen + 1, sizeof(char));
	if (getCurText(text, stringLen, pathToBuf->ver, NULL) == FAIL)
	{
		printf("ERROR: unable to calculate length of merged text.\n");
		goto Fail;
	}
	int offsetsArrLen = strlen(text) + 1;
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
	if (!areAbleToMerge(bufOffset, verOffset, offsetsArrLen))
	{
		goto Fail;
	}
	if (copyMergeOperations() == FAIL)
	{
		printf("ERROR: unable to copy operations.\n");
		goto Fail;
	}

	if (text) free(text);
	if (pathToBuf) deletePath(&pathToBuf);
	if (pathToVer) deletePath(&pathToVer);
	if (bufOffset) free(bufOffset);
	if (verOffset) free(verOffset);
	return SUCCESS;

Fail:
	if (initBuf(buf->parentVerNum) == FAIL)
	{
		printf("ERROR: unable to recreate buffer.\n");
	}
	if (text) free(text);
	if (pathToBuf) deletePath(&pathToBuf);
	if (pathToVer) deletePath(&pathToVer);
	if (bufOffset) free(bufOffset);
	if (verOffset) free(verOffset);
	return FAIL;
}

int* getOffsets(verList_t* path, int size)
{
	int* offset = (int*)malloc(size * sizeof(int)); //value in offset - position in text with additions
	if (!offset)
	{
		printf("ERROR: memory allocation problem.\n");
		return NULL;
	}
	for (int i = 0; i < size; i++) offset[i] = i;
	path = path->next;
	while (path)
	{
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
			while (opEl->beginIndex <= offset[i] && offset[i] < opEl->endIndex)
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

bool_t areAbleToMerge(int* bufOffset, int* verOffset, int size)
{
	int index = 0; //offset for addition into result
	for (int i = 0; i < size; i++)
	{
		if (verOffset[i] == DELETED && bufOffset[i] == DELETED) continue;
	    int nextIndex = index;

		//copy char itself
		if (verOffset[i] != DELETED && bufOffset[i] != DELETED && i != size - 1)
		{
			nextIndex++;
		}
		int toAddFromBuf = 0;
		int toAddFromVer = 0;
		if (i == 0)
		{
			//copy text before first char from buf or ver
			toAddFromBuf = bufOffset[i];
			toAddFromVer = verOffset[i];
			if (toAddFromBuf != 0 && toAddFromVer != 0)
			{
				printf("ERROR: conflict while merging.\n");
				return FALSE_;
			}
			else if (toAddFromBuf > 0) nextIndex += toAddFromBuf;
			else if (toAddFromVer > 0) nextIndex += toAddFromVer;
		}
		else
		{
			getAddOperationInfo(&toAddFromBuf, i, bufOffset);
			getAddOperationInfo(&toAddFromVer, i, verOffset);
			if (toAddFromBuf != 0 && toAddFromVer != 0)
			{
				printf("ERROR: conflict while merging.\n");
				return FALSE_;
			}
			else if (toAddFromBuf > 0) nextIndex += toAddFromBuf;
			else if (toAddFromVer > 0) nextIndex += toAddFromVer;
		}
	    index = nextIndex;
	}
	return TRUE_;
}

bool_t areOnSamePath(verList_t* pathToBuf, verList_t* pathToVer)
{
	while (pathToBuf && pathToVer && pathToBuf->ver == pathToVer->ver)
	{
		pathToBuf = pathToBuf->next;
		pathToVer = pathToVer->next;
	}
	if (!pathToBuf || !pathToVer) return TRUE;
	return FALSE;
}

int getClosestNonDeletedIndex(int i, int* offset)
{
	i = i - 1;
	for (; i >= 0; i--)
	{
		if (offset[i] != DELETED) return i;
	}
	return INVALID_INDEX;
}

void removeSamePartOfPath(verList_t** pathToBuf, verList_t** pathToVer)
{
	verList_t* bufPrev = NULL;
	verList_t* verPrev = NULL;
	do
	{
		bufPrev = *pathToBuf;
		verPrev = *pathToVer;
		*pathToBuf = (*pathToBuf)->next;
		*pathToVer = (*pathToVer)->next;
	} while (*pathToBuf && *pathToVer && (*pathToBuf)->ver == (*pathToVer)->ver);
	*pathToBuf = bufPrev;
	*pathToVer = verPrev;
}

void getAddOperationInfo(int* toAdd, int i, int* offsetsArr)
{
    int prev = getClosestNonDeletedIndex(i, offsetsArr);
	*toAdd = offsetsArr[i] - 1;
	if (prev != INVALID_INDEX) *toAdd -= offsetsArr[prev];
	else (*toAdd)++; //only deleted chars before current
}