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
	char* dir = getDirName();
	if (!CreateDirectory(dir, NULL))
	{
		printf("ERROR: unable to create directory for version files.\n");
		return FAIL;
	}
	if (push() == FAIL) //записать версию в файл (push), последн€€ созданна€ верси€ - 0
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
	if (buf == p) buf = NULL;
	free(p);
}

status_t buildVerTree(int verNum) 
{
	char* fileName = getNameOfVerFile(verNum);
	char* rootFileName = getNameOfVerFile(ROOT_VER);
	if (!exists(rootFileName) && verNum != ROOT_VER) //попытка прочитать несуществующее дерево
	{
		free(fileName);
		free(rootFileName);
		printf("ERROR: attempt to load not existing tree.\n");
		return FAIL;
	}
	if (!exists(fileName))
	{
		free(fileName);
		if (initVerTree() == FAIL) //buffer will be initialized here
		{
			printf("ERROR: unable to create tree.\n");
			return FAIL;
		}
	}
	else
	{
		if (loadVerTree() == FAIL)
		{
			free(fileName);
			printf("ERROR: uable to create init version of file.\n");
			return FAIL;
		}
		free(fileName);
		if (initBuf(verNum) == FAIL)
		{
			printf("ERROR: unable to create new buffer.\n");
			return FAIL;
		}
	}
	return SUCCESS;
}

status_t loadVerTree()
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;
	char* dirName = getDirName();
	
	char filePath[FNAME_LEN] = { 0 };
	sprintf(filePath, FILE_MASK, dirName, dirName);
	if ((hFind = FindFirstFile(filePath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		printf("ERROR: path not found.\n");
		return FAIL;
	}
	do
	{
		if (strcmp(fdFile.cFileName, ".") != 0		//Find first file will always return "."
			&& strcmp(fdFile.cFileName, "..") != 0) //and ".." as the first two directories.
		{
			sprintf(filePath, "%s\\%s", dirName, fdFile.cFileName);
			if (handleVerFile(filePath) == FAIL)
			{
				printf("ERROR: unable to load existing tree.\n");
				return FAIL;
			}
		}
	} while (FindNextFile(hFind, &fdFile));
	FindClose(hFind);
	return SUCCESS;
}

status_t handleVerFile(char filePath[FNAME_LEN])
{
	FILE* file = fopen(filePath, "r");
	if (!file)
	{
		printf("ERROR: unable to open file with information.\n");
		return FAIL;
	}
	int parentVer = INVALID_PARENT;
	fscanf(file, "%i", &parentVer);
	if (parentVer == INVALID_PARENT) return SUCCESS; //version is deleted from tree
	
	version_t* ver = (version_t*)malloc(sizeof(version_t));
	ver->verNum = getVerNum(filePath);
	if (ver->verNum == INVALID_VER)
	{
		printf("ERROR: invalid file name detected.\n");
		free(ver);
		return FAIL;
	}
	ver->parentPtr = getVerPtr(generalInfo->root, parentVer);
	ver->childNum = 0;
	ver->child = NULL;
	ver->operation = NULL;
	if (getOperationList(&(ver->operation), file) == FAIL)
	{
		printf("ERROR: invalid operation in file detected.\n");
		free(ver);
		return FAIL;
	}
	insertIntoTree(ver);
	fclose(file);
	return SUCCESS;
}

void insertIntoTree(version_t* ver)
{
	if (!(ver->parentPtr)) generalInfo->root = ver;
	else
	{
		(ver->parentPtr->childNum)++;
		ver->parentPtr->child = (version_t**)realloc(ver->parentPtr->child, ver->parentPtr->childNum * sizeof(version_t*));
		ver->parentPtr->child[ver->parentPtr->childNum - 1] = ver;
	}
	generalInfo->lastCreatedVersion = ver->verNum; //увеличить номер последней сохранЄнной версии
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
	if (buf->parentPtr) //сохранить буфер как ребЄнка потенциального родител€
	{
		(buf->parentPtr->childNum)++;
		buf->parentPtr->child = (version_t**)realloc(buf->parentPtr->child, buf->parentPtr->childNum * sizeof(version_t*));
		buf->parentPtr->child[buf->parentPtr->childNum - 1] = buf; 
	}
	generalInfo->lastCreatedVersion = buf->verNum; //увеличить номер последней сохранЄнной версии
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

status_t handleVerDeleting()
{
	int i = 0;
	printf("Enter version number or any negative number to cancel (no number -> index = 0): ");
	scanf_s("%i", &i);
	if (i < 0) return SUCCESS;
	version_t* ver = getVerPtr(generalInfo->root, i);
	if (!ver)
	{
		printf("ERROR: attempt to delete version that dooesn't exist.\n");
		return FAIL;
	}
	if (deleteVer(ver) == FAIL)
	{
		printf("ERROR: unable to delete version.\n");
		return FAIL;
	}
	return SUCCESS;
}

status_t deleteVer(version_t* verToDelete)
{
	if (copyVerChildren(verToDelete) == FAIL) //перекинуть детей версии на еЄ родител€ (создать детей родител€ verToDelete, операции которых - сумма операций из verToDelete и их собственные)
	{
		printf("ERROR: unable to attach version's children to it's parent.\n");
		return FAIL;
	}
	if (buf->parentPtr == verToDelete && copyChild(/*TODO*/) == FAIL) //если буфер - сын удал€емой версии, то его тоже перекинуть на родител€ verToDelete
	{
		printf("ERROR: unable to attach buffer to new parent");
		return FAIL;
	}
	char* fileName = getNameOfVerFile(verToDelete->verNum);
	if (!fileName || !DeleteFile(fileName)) printf("WARNNING: version file wasn't deleted.\n");
	if (fileName) free(fileName);
	return SUCCESS;
}