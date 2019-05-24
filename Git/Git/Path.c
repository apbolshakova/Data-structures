#include "Common.h"

status_t getPath(path_t** path)
{
	version_t* ver = generalInfo->root;

	if (buf)
	{
		if (shiftIntoPath(path, buf) == FAIL)
		{
			printf("ERROR: unable to get path of buffer's parent.\n");
			return FAIL;
		}
		ver = buf->parentPtr;
	}
	while (ver)
	{
		if (shiftIntoPath(path, ver) == FAIL)
		{
			printf("ERROR: unable to get path of buffer's parent.\n");
			return FAIL;
		}
		ver = ver->parentPtr; //новая текущая версия - родитель текущей
	}
	return SUCCESS;
}

status_t shiftIntoPath(path_t** root, version_t* ver)
{
	path_t* newEl = (path_t*)malloc(sizeof(path_t));
	if (!newEl)
	{
		printf("ERROR: memory allocation error.\n");
		return FAIL;
	}
	newEl->ver = ver;
	if (!(*root)) newEl->next = NULL;
	else newEl->next = *root;
	*root = newEl;
	return SUCCESS;
}

void deletePath(path_t** root)
{
	path_t* temp = *root;
	while (*root)
	{
		*root = (*root)->next;
		free(temp);
		temp = NULL;
		temp = *root;
	}
}