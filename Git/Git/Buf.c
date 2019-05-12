#include "Header.h"

func_res_t initBuf(int version)
{
	if (buf) deleteBuf();
	buf = (version_t*)malloc(sizeof(buf));
	if (!buf)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	buf->parentPtr = NULL;
	if (generalInfo->root) //дерево уже существует
	{
		buf->parentPtr = getVerPtr(generalInfo->root, version); //TODO: ПРОТЕСТИРОВАТЬ
		if (!(buf->parentPtr))
		{
			free(buf);
			buf = NULL;
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
	//TODO: очистка буфера
}