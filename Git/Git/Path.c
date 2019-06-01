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
		ver = ver->parentPtr; //new current version is the parent of previous current
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

status_t insertLostVersIntoTree(verList_t** root)
{
	bool_t changed = TRUE_;
	while (changed)
	{
		changed = FALSE_;
		if (!*root) return;
		verList_t* prev = NULL;
		verList_t* el = *root;
		while (el)
		{
			version_t* parent = getVerPtr(generalInfo->root, el->ver->parentVerNum);
			if (parent)
			{
				if (addChild(el->ver, parent) == FAIL)
				{
					printf("ERROR: found missed parent but unable to attach child to it.\n");
					return FAIL;
				}
				verList_t* sav = el->next;
				free(el);
			
				el = sav;
				if (prev) prev->next = el;
				else *root = el;
				changed = TRUE_;
			}
			else
			{
				prev = el;
				el = el->next;
			}
		}
	}
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