#include "Common.h"

status_t getPath(verList_t** path, version_t* ver)
{
	if (!ver)
	{
		ver = generalInfo->root;
		if (buf)
		{
			if (shiftIntoPath(path, buf) == FAIL)
			{
				printf("ERROR: unable to get path of buffer's parent.\n");
				return FAIL;
			}
			ver = buf->parentPtr;
		}
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

status_t shiftIntoPath(verList_t** root, version_t* ver)
{
	verList_t* newEl = (verList_t*)malloc(sizeof(verList_t));
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

void popFromVerList(verList_t** root)
{
	if (!*root) return;
	verList_t* newRoot = (*root)->next;
	free(*root);
	*root = newRoot;
}

void deletePath(verList_t** root)
{
	verList_t* temp = *root;
	while (*root)
	{
		*root = (*root)->next;
		free(temp);
		temp = NULL;
		temp = *root;
	}
}