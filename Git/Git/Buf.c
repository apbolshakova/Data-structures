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
	buf->parentVerNum = INVALID_VER;
	if (generalInfo->root) //дерево уже существует
	{
		buf->parentVerNum = version;
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
	if (buf->parentPtr) fprintf(file, "%i\n", buf->parentVerNum);
	else fprintf(file, "-1\n");
	if (printOperations(file, NULL) == FAIL)
	{
		fclose(file);
		printf("ERROR: operations data is corrupted.\n");
		return FAIL;
	};
	fclose(file);
	return SUCCESS;
}

status_t rewriteVerFile(version_t* ver)
{
	if (!ver) return SUCCESS;
	const char* fileName = getNameOfVerFile(ver->verNum);
	FILE* file = fopen(fileName, "w");
	free(fileName);
	if (!file)
	{
		printf("ERROR: unable to open version file.\n");
		return FAIL;
	}
	if (ver->parentPtr) fprintf(file, "%i\n", ver->parentVerNum);
	else fprintf(file, "-1\n");
	if (printOperations(file, ver->operation) == FAIL)
	{
		fclose(file);
		printf("ERROR: operations data is corrupted.\n");
		return FAIL;
	};
	fclose(file);
	return SUCCESS;
}