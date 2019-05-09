#include "Header.h"

char* getNameOfVerFile(int version)
{
	char fileName[FNAME_LEN] = { 0 };
	strcpy(fileName, generalInfo->name);
	strcat(fileName, ".");
	strcatInt(fileName, version);
	strcat(fileName, VER_FILE_EXT);
	return fileName;
}

void strcatInt(char fileName[FNAME_LEN], int version)
{
	char* digitAsChar = "0";
	if (!version) strcat(fileName, digitAsChar);
	while (version)
	{
		digitAsChar = (version % DEC_NUMBER_SYSTEM) + '0';
		version /= DEC_NUMBER_SYSTEM;
		strcat(fileName, digitAsChar);
	}
}

bool_t exists(const char *fname)
{
	FILE *file;
	if ((file = fopen(fname, "r")))
	{
		fclose(file);
		return TRUE;
	}
	return FALSE;
}

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
			deleteVerTree();
			generalInfo->root = NULL;
		}
		free(generalInfo);
		generalInfo = NULL;
	}
	if (buf)
	{
		if (buf->operation) deleteOperationList(&(buf->operation));
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