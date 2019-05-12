#include "Header.h"

char* getNameOfVerFile(int version)
{
	char* fileName = (char*)calloc(FNAME_LEN, sizeof(char));
	strncpy(fileName, generalInfo->name, 
		strlen(generalInfo->name) - strlen(getFileExt(generalInfo->name)));
	strcatInt(fileName, version);
	strcat(fileName, ".%s", TEXT_EXT);
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
	deleteGeneralInfo();
	deleteBuf();
}

const char* getFileExt(const char* filename) 
{
	const char *dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
}

long int getFileSize(FILE *f)
{
	long int sav = ftell(f);
	fseek(f, 0L, SEEK_END);
	long int fileSize = ftell(f);
	fseek(f, sav, SEEK_SET);
	return(fileSize);
}

int getTextLen()
{
	int result = 0;
	version_t* ver = generalInfo->root;

	if (buf)
	{
		result += getLenDiff(buf->operation);
		ver = buf->parentPtr;
	}
	while (ver)
	{
		result += getLenDiff(ver->operation); //просуммировать длины операций из этой версии
		ver = ver->parentPtr; //новая текущая версия - родитель текущей
	}
	return result;
}

int getTextLenAndVersionDepth(int* versionDepth)
{
	int len = 0;
	*versionDepth = 0;
	version_t* ver = generalInfo->root;

	if (buf)
	{
		len += getLenDiff(buf->operation);
		(*versionDepth)++;
		ver = buf->parentPtr;
	}
	while (ver)
	{
		len += getLenDiff(ver->operation); //просуммировать длины операций из этой версии
		(*versionDepth)++;
		ver = ver->parentPtr; //новая текущая версия - родитель текущей
	}
	return len;
}

int* getPath()
{
	/*int* path = ;
	version_t* ver = generalInfo->root;

	if (buf)
	{
		copyOperationsToBegin(&opList, buf->operation);
		ver = buf->parentPtr;
	}
	while (ver)
	{
		copyOperationsToBegin(&opList, ver->operation);
		ver = ver->parentPtr; //новая текущая версия - родитель текущей
	}
	return result;*/
}

int getLenDiff(operation_t* list)
{
	int result = 0;
	operation_t* op = list;
	while (op)
	{
		if (op->type == '+') result += strlen(op->data);
		if (op->type == '-') result -= op->endIndex + op->beginIndex;
		op = op->next;
	}
	return result;
}

func_res_t print()
{
	int depth = 0;
	int textLen = getTextLenAndVersionDepth(&depth);

	char* text = (char*)malloc(sizeof(char) * textLen + 1);
	int* path = (int*)malloc(sizeof(int) * depth);
	getPath(path); //получить путь по дереву
	//пройти по пути и собрать операции, применяя их к text
}