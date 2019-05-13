#include "Header.h"

func_res_t getPath(path_t** path)
{
	version_t* ver = generalInfo->root;

	if (buf)
	{
		if (pushIntoPath(path, buf) == FAIL)
		{
			printf("ERROR: unable to get path of buffer's parent.\n");
			return FAIL;
		}
		ver = buf->parentPtr;
	}
	while (ver)
	{
		if (pushIntoPath(path, ver) == FAIL)
		{
			printf("ERROR: unable to get path of buffer's parent.\n");
			return FAIL;
		}
		ver = ver->parentPtr; //новая текущая версия - родитель текущей
	}
	return SUCCESS;
}

func_res_t pushIntoPath(path_t** root, version_t* ver)
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