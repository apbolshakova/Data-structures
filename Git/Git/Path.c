#include "Header.h"

func_res_t getPath(path_t** path)
{
	*path = NULL;
	version_t* ver = generalInfo->root;

	if (buf)
	{
		if (pushVerIntoPath(path, BUF) == FAIL)
		{
			printf("ERROR: unable to get path of buffer's parent.\n");
			return FAIL;
		}
		ver = buf->parentPtr;
	}
	while (ver)
	{
		if (pushVerIntoPath(path, ver->verNum) == FAIL)
		{
			printf("ERROR: unable to get path of buffer's parent.\n");
			return FAIL;
		}
		ver = ver->parentPtr; //новая текущая версия - родитель текущей
	}
	return SUCCESS;
}

func_res_t pushVerIntoPath(path_t** root, int verNum)
{
	path_t* newEl = (path_t*)malloc(sizeof(path_t));
	if (!newEl)
	{
		printf("ERROR: memory allocation error.\n");
		return FAIL;
	}
	newEl->verNum = verNum;
	if (!root) newEl->next = NULL;
	else newEl->next = *root;
	root = &newEl;
	return SUCCESS;
}