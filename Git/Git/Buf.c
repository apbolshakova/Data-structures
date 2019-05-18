#include "Common.h"

status_t initBuf(int version)
{
	if (buf) buf = NULL;
	buf = (version_t*)malloc(sizeof(version_t));
	if (!buf)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	buf->parentPtr = NULL;
	if (generalInfo->root) //дерево уже существует
	{
		buf->parentPtr = getVerPtr(generalInfo->root, version);
		if (!(buf->parentPtr))
		{
			deleteBuf();
			printf("ERROR: Attempt to use invalid version as parent.\n");
			return FAIL;
		}
	}
	buf->verNum = generalInfo->lastCreatedVersion + 1;
	buf->childNum = 0;
	buf->child = NULL;
	buf->operation = NULL;
	return SUCCESS;
}

void deleteBuf()
{
	if (buf)
	{
		if (buf->operation) deleteOperationList(&(buf->operation));
		free(buf);
		buf = NULL;
	}
}

status_t createVerFile()
{
	const char* fileName = getNameOfVerFile(buf->verNum);
	FILE* file = fopen(fileName, "w");
	free(fileName);
	if (!file)
	{
		printf("ERROR: unable to create new version file.\n");
		return FAIL;
	}
	if (buf->parentPtr) fprintf(file, "%i\n", buf->parentPtr->verNum);
	else fprintf(file, "-1\n");
	if (printOperations(file) == FAIL)
	{
		printf("ERROR: operations data is corrupted.\n");
		return FAIL;
	};
	fclose(file);
	return SUCCESS;
}