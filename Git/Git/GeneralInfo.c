#include "Common.h"

status_t initGeneralInfo(char* fname, int verNum)
{
	generalInfo = (general_t*)malloc(sizeof(general_t));
	if (!generalInfo)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}

	size_t nameLen = strlen(fname);
	generalInfo->name = (char*)malloc((nameLen + 1) * sizeof(char));
	if (!generalInfo->name)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	strncpy(generalInfo->name, fname, nameLen);
	generalInfo->lastCreatedVersion = 0;
	generalInfo->name[nameLen] = '\0';
	generalInfo->root = NULL;

	if (buildVerTree(verNum) == FAIL)
	{
		printf("ERROR: unable to correctly build tree of versions.\n");
		return FAIL;
	}; 
	return SUCCESS;
}

void deleteGeneralInfo()
{
	if (generalInfo)
	{
		if (generalInfo->name)
		{
			free(generalInfo->name);
			generalInfo->name = NULL;
		}
		deleteVerTree(generalInfo->root);
		generalInfo->root = NULL;

		free(generalInfo);
		generalInfo = NULL;
	}
}