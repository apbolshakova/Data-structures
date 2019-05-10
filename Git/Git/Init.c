#include "Header.h"

func_result_t initGeneralInfo(char fname[FNAME_LEN])
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
	generalInfo->name[nameLen] = '\0';
	
	if (buildVerTree() == FAIL) //получить generalInfo->root и lastCreatedVersion
	{
		printf("ERROR: unable to correctly build tree of versions.\n");
		return FAIL;
	}; 
	return SUCCESS;
}

func_result_t initBuf(int version)
{
	buf = (version_t*)malloc(sizeof(buf));
	if (!buf)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	buf->verNum = generalInfo->lastCreatedVersion + 1;
	buf->parentVerNum = version;
	buf->operation = NULL;
	return SUCCESS;
}

//TODO: maybe delete
/*func_result_t handleFile(char fname[FNAME_LEN], int version, version_t* verInfo)
{
	char* fileName = (char*)malloc((FNAME_LEN + VERSION_CHARS_NUM) * sizeof(char));
	strncpy(fileName, fname, strlen(fname));
	fileName[strlen(fname)] = '.';
	_itoa(version, fileName + strlen(fname) + 1, DEC_NUMBER_SYSTEM);
	
	FILE* verFile = fopen(fileName, 'r'); //открыть файл fname.version
	if (!verFile || loadVersion(verInfo, verFile) == FAIL) return FAIL;
	return SUCCESS;
}

func_result_t loadVersion(version_t* verInfo, FILE* verFile) 
{
	fscanf(verFile, "%i", &(verInfo->parentVer));
	verInfo->operation = (operation_t*)malloc(sizeof(operation_t));
	if (verInfo->operation == NULL) return FAIL;
	return SUCCESS;
}

char* getTextInCurVer(version_t* verInfo)
{
}*/