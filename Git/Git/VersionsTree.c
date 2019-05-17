#include "Common.h"

status_t initVerTree()
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
	if (push() == FAIL) //записать версию в файл (push), последняя созданная версия - 0
	{
		printf("ERROR: unsuccessful attempt to push init version.\n");
		return FAIL;
	} 
	return SUCCESS;
}

status_t initTextAsOpearationInRootVer()
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

status_t getSourceText(char** text)
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

void deleteVerTree(version_t* p)
{
	if (!p) return;
	for (int i = 0; i < p->childNum; i++) deleteVerTree(p->child[i]);
	free(p);
}

status_t buildVerTree() 
{
	char* fileName = getNameOfVerFile(ROOT_VER);
	if (!exists(fileName))
	{
		free(fileName);
		if (initVerTree() == FAIL)
		{
			printf("ERROR: Unable to create init version of file.\n");
			return FAIL;
		}
	}
	else
	{
		if (loadVerTree(fileName) == FAIL)
		{
			free(fileName);
			printf("ERROR: Unable to create init version of file.\n");
			return FAIL;
		}
		free(fileName);
		//TODO: load existing tree
	}
	return SUCCESS;
}

status_t loadVerTree(char* fileName)
{
	int parentVer = NOT_DEFINED_PARENT;
	if (loadVer(fileName, &parentVer) == FAIL) //получить номер версии родителя, создать узел в дереве - родителя текущего корня и сделать его корнем
	{
		printf("ERROR: unable to parse data of existing tree.\n");
		return FAIL;
	}
	free(fileName);
	fileName = NULL;
	while (parentVer != NOT_DEFINED_PARENT) //пока не был обработан корень
	{
		fileName = getNameOfVerFile(parentVer);
		if (loadVer(fileName, &parentVer) == FAIL)
		{
			printf("ERROR: unable to parse data of existing tree.\n");
			return FAIL;
		}
		free(fileName);
		fileName = NULL;
	}
	getLastCreatedVersion(); //TODO
}

status_t loadVer(char* fileName, int* parentVer)
{
	FILE* file = fopen(fileName, "r");
	if (!file)
	{
		printf("ERROR: unable to open file with information.\n");
		return FAIL;
	}
	//Получить номер версии родителя
	//Создать экземпляр версии
	//Вставить версию в качестве корня дерева
	return SUCCESS;
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

status_t push()
{
	if (buf->parentPtr) //сохранить буфер как ребёнка потенциального родителя
	{
		(buf->parentPtr->childNum)++;
		buf->parentPtr->child = (version_t**)realloc(buf->parentPtr->child, buf->parentPtr->childNum * sizeof(version_t*));
		buf->parentPtr->child[buf->parentPtr->childNum - 1] = buf; 
	}
	generalInfo->lastCreatedVersion = buf->verNum; //увеличить номер последней сохранённой версии
	if (createVerFile() == FAIL)
	{
		printf("ERROR: unable to update version files after pushing new version.\n");
		return FAIL;
	}
	if (initBuf(generalInfo->lastCreatedVersion) == FAIL) //очистить и создать новый буфер
	{
		printf("ERROR: unable to create new buffer.\n");
		return FAIL;
	}
	return SUCCESS;
}