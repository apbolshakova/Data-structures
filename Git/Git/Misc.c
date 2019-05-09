#include "Header.h"

void cleanup()
{
	if (generalInfo)
	{
		if (generalInfo->name)
		{
			free(generalInfo->name);
			generalInfo->name = NULL;
		}
		if (generalInfo->root)
		{
			deleteVerTree(); //TODO: �����������
			generalInfo->root = NULL;
		}
		free(generalInfo);
		generalInfo = NULL;
	}
	if (buf)
	{
		if (buf->operation) deleteOperationList(&(buf->operation)); //TODO: �����������
		free(buf);
		buf = NULL;
	}
}

const char* getFileExt(const char* filename) 
{
	const char *dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
}