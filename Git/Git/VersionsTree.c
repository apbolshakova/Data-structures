#include "Header.h"

func_res_t initVerTree()
{
	generalInfo->lastCreatedVersion = INIT_VERSION;
	if (initBuf(ROOT_VER) == FAIL) 	//создать буфер с нулевой версией
	{
		printf("ERROR: invalid buffer initialization.\n");
		return FAIL;
	}
	generalInfo->root = buf;
	if (initTextAsOpearationInRootVer() == FAIL)
	{
		printf("ERROR: invalid operation building.\n");
		return FAIL;
	}
	if (push() == FAIL) //записать версию в файл (push), последн€€ созданна€ верси€ - 0
	{
		printf("ERROR: unsuccessful attempt to push init version.\n");
		return FAIL;
	} 
	return SUCCESS;
}

func_res_t initTextAsOpearationInRootVer()
{
	//открыть файл generalInfo->fileName и получить из него текст
	char* text = NULL;
	if (getSourceText(&text) == FAIL)
	{
		printf("ERROR: Unable to get source text.\n");
		return FAIL;
	}
	//положить в операции буфера команду add текста из файла
	if (add(0, text) == FAIL)
	{
		printf("ERROR: Unable to create an operation entry.\n");
		return FAIL;
	}
}

func_res_t getSourceText(char** text)
{
	FILE* source = fopen(generalInfo->name, "r");
	if (!source)
	{
		fclose(source);
		printf("ERROR: Unable to open source file.\n");
		return FAIL;
	}
	long int fileSize = getFileSize(source);
	*text = (char*)calloc(fileSize, sizeof(char));
	if (!(*text))
	{
		fclose(source);
		printf("ERROR: Memory allocation error.\n");
		return FAIL;
	}
	char* temp = (char*)calloc(fileSize, sizeof(char));
	if (!temp)
	{
		fclose(source);
		printf("ERROR: Memory allocation error.\n");
		return FAIL;
	}
	while (fgets(temp, fileSize, source) != NULL) strcat(*text, temp);
	fclose(source);
	return SUCCESS;
}

void deleteVerTree()
{
	//TODO
}

func_res_t buildVerTree() 
{
	if (!exists(getNameOfVerFile(ROOT_VER)))
	{
		if (initVerTree() == FAIL)
		{
			printf("ERROR: Unable to create init version of file.\n");
			return FAIL;
		}
	}
	//TODO: load existing tree
}

version_t* getVerPtr(version_t* p, int verNum)
{
	if (!p) return NULL;

	version_t* result = p;
	if (result->verNum == verNum) return p;
	else
	{
		for (int i = 0; i < p->childNum; i++)
		{
			result = getVerPtr(p->child[i], verNum);
			if (result == verNum) break;
		}
	}
	return result;
}

func_res_t push()
{
	if (buf->parentPtr) //сохранить буфер как ребЄнка потенциального родител€
	{
		(buf->parentPtr->childNum)++;
		buf->parentPtr->child[buf->parentPtr->childNum - 1] = buf; 
	}
	generalInfo->lastCreatedVersion = buf->verNum; //увеличить номер последней сохранЄнной версии
	if (updateFilesAttachedToBuf() == FAIL)
	{
		printf("ERROR: unable to update version files after pushing new version.\n");
		return FAIL;
	}
	if (initBuf(generalInfo->lastCreatedVersion) == FAIL) //очистить и создать новый буфер
	{
		printf("ERROR: unable to create new buffer.\n");
		return FAIL;
	}
}

func_res_t updateFilesAttachedToBuf()
{
	return SUCCESS;
}