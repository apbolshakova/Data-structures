#include "Header.h"

func_res_t initBuf(int version)
{
	if (buf) buf = NULL;
	buf = (version_t*)malloc(sizeof(version_t));
	if (!buf)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	buf->parentPtr = NULL;
	if (generalInfo->root) //äåðåâî óæå ñóùåñòâóåò
	{
		buf->parentPtr = getVerPtr(generalInfo->root, version); //TODO: ÏÐÎÒÅÑÒÈÐÎÂÀÒÜ
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