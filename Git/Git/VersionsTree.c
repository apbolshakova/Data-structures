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
	if (add(0, text, NULL) == FAIL)
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
	if (insertIntoTree(ver) == FAIL)
	{
		printf("ERROR: unable to insert version into version tree.\n");
		return FAIL;
	}
	fclose(file);
	return SUCCESS;
}

status_t insertIntoTree(version_t* ver)
{
	if (!(ver->parentPtr)) generalInfo->root = ver;
	else if (addChild(ver, ver->parentPtr) == FAIL)
	{
		printf("ERROR: unable to add child to version.\n");
		return FAIL;
	}
	generalInfo->lastCreatedVersion = ver->verNum; //увеличить номер последней сохранЄнной версии
	return SUCCESS;
}

version_t* getVerPtr(version_t* p, int verNum)
{
	version_t* result = NULL;
	if (!p) return result;

	if (p->verNum == verNum) return p;
	else
	{
		for (int i = 0; i < p->childNum; i++)
		{
			result = getVerPtr(p->child[i], verNum);
			if (result && result->verNum == verNum) break;
		}
	}
	return result;
}

status_t push()
{
	if (buf->parentPtr && addChild(buf, buf->parentPtr) == FAIL) //сохранить буфер как ребЄнка потенциального родител€
	{
		printf("ERROR: unable to add buffer to version as it's child.\n");
		return FAIL;
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

status_t pull()
{
	int i = 0;
	printf("Enter version number or any negative number to cancel (no number -> index = 0): ");
	fflush(stdin);
	scanf_s("%i", &i);
	if (i < 0) return SUCCESS;
	if (!getVerPtr(generalInfo->root, i))
	{
		printf("ERROR: attempt to pull not existing version.\n");
	}
	if (initBuf(i) == FAIL)
	{
		printf("ERROR: unable to initialize new buffer.\n");
		return FAIL;
	}
	return SUCCESS;
}

status_t handleVerDeleting()
{
	int i = 0;
	printf("Enter version number or any negative number to cancel (no number -> index = 0): ");
	fflush(stdin);
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
	if (deleteFromChildren(verToDelete) == FAIL) //TODO
	{
		printf("ERROR: unable to delete version from it's parent children.\n");
		return FAIL;
	}
	if (copyVerChildren(verToDelete) == FAIL)
	{
		printf("ERROR: unable to attach version's children to it's parent.\n");
		return FAIL;
	}
	if (buf->parentPtr == verToDelete && relocateChild(verToDelete, BUF) == FAIL) //если буфер - сын удал€емой версии, то его тоже перекинуть на родител€ verToDelete
	{
		printf("ERROR: unable to attach buffer to new parent");
		return FAIL;
	}
	char* fileName = getNameOfVerFile(verToDelete->verNum);
	if (!fileName || !DeleteFile(fileName)) printf("WARNNING: version file wasn't deleted.\n");
	if (fileName) free(fileName);
	cleanupVersion(&verToDelete);
	return SUCCESS;
}

status_t deleteFromChildren(version_t* verToDelete)
{
	if (!(verToDelete) || !(verToDelete->parentPtr)) return SUCCESS;
	int verPos = INVALID_INDEX;
	for (int i = 0; i < verToDelete->parentPtr->childNum; i++)
	{
		if (verToDelete->parentPtr->child[i] == verToDelete)
		{
			verPos = i;
			break;
		}
	}
	if (verPos == INVALID_INDEX)
	{
		printf("ERROR: version is not attacted to it's parent as child");
		return FAIL;
	}
	verToDelete->parentPtr->child[verPos] = NULL;
	moveBackChildren(verToDelete->parentPtr, verPos);
	return SUCCESS;
}

void moveBackChildren(version_t* parentPtr, int verPos) //TODO test
{
	for (int i = verPos + 1; i < parentPtr->childNum; i++)
	{
		parentPtr->child[i - 1] = parentPtr->child[i];
		parentPtr->child[i] = NULL;
	}
	if (parentPtr->childNum >= 1) parentPtr->childNum--;
}

status_t copyVerChildren(version_t* prevParent)
{
	if (!(prevParent->parentPtr)) //handle if it's root
	{
		generalInfo->root = prevParent->child[0];
		for (int i = 1; i < prevParent->childNum; i++)
		{
			//формируемый список операций - отмена всех операций нулевого ребЄнка + операции ребЄнка, присвоить созданный список как список операций ребЄнка, addChild TODO
		}
	}
	else
	{
		for (int i = 0; i < prevParent->childNum; i++)
		{
			if (relocateChild(prevParent, i) == FAIL) return FAIL;
		}
	}
	return SUCCESS;
}

status_t relocateChild(version_t* prevParent, int i)
{
	operation_t* opListRoot = NULL; //новый список операций
	if (appendOpList(&opListRoot, prevParent->operation) == FAIL)
	{
		printf("ERROR: unable to copy version's operation list.\n");
		return FAIL;
	}
	if (i != BUF)
	{
		if (appendOpList(&opListRoot, prevParent->child[i]->operation) == FAIL)
		{
			printf("ERROR: unable to copy version's operation list.\n");
			return FAIL;
		}
		deleteOperationList(&(prevParent->child[i]->operation));
		prevParent->child[i]->operation = opListRoot;
		if (addChild(prevParent->child[i], prevParent->parentPtr) == FAIL)
		{
			printf("ERROR: unable to add child to version.\n");
			return FAIL;
		}
		if (rewriteVerFile(prevParent->child[i]) == FAIL)
		{
			printf("ERROR: unable to rewrite child's file.\n");
			return FAIL;
		}
	}
	else
	{
		deleteOperationList(&(buf->operation));
		buf->parentPtr = prevParent->parentPtr;
	}
	return SUCCESS;
}

status_t addChild(version_t* newChild, version_t* parent)
{
	if (!newChild)
	{
		printf("ERROR: attempt to add NULL pointer as child.\n");
		return FAIL;
	}
	if (!parent)
	{
		printf("ERROR: attempt to add child for NULL pointer.\n");
		return FAIL;
	}
    (parent->childNum)++;
	parent->child = (version_t**)realloc(parent->child, parent->childNum * sizeof(version_t*));
	if (!parent->child)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	parent->child[parent->childNum - 1] = newChild;
	newChild->parentPtr = parent;
	return SUCCESS;
}

void cleanupVersion(version_t** ver)
{
	if (!ver) return;
	deleteOperationList(&((*ver)->operation));
	free(*ver);
	*ver = NULL;
}

status_t handleRebasing()
{
	int i = 0;
	printf("Enter version number or any negative number to cancel (no number -> index = 0): ");
	fflush(stdin);
	scanf_s("%i", &i);
	if (i < 0) return SUCCESS;
	if (rebase(i) == FAIL)
	{
		printf("ERROR: unable to delete version.\n");
		return FAIL;
	}
}

status_t rebase(int verNum)
{
	version_t* newRoot = getVerPtr(generalInfo->root, verNum); //получить newRoot и lastEl
	if (!newRoot)
	{
		printf("ERROR: invalid version number for rebasing.\n");
		return FAIL;
	}
	version_t* lastEl = newRoot; //конец пути, по которому можно дойти от корн€ до элемента (newRoot поднимаетc€ по этому пути)
	if (mergeOperaions(lastEl) == FALSE)//заменить операции из newRoot на единственную - add текст в виде из данной версии
	{
		printf("ERROR: unable to merge operations for new root into one add operation.\n");
		return FAIL;
	}
	while (newRoot->parentPtr != NULL)
	{
		if (addChild(newRoot->parentPtr, lastEl) == FAIL) //добавить newRoot->parent в детей lastEl
		{
			printf("ERROR: unable to switch versions positions in tree.\n");
			return FAIL;
		}
		lastEl = newRoot->parentPtr; //lastEl = этот новый ребЄнок
		if (lastEl->parentPtr == NULL) newRoot->parentPtr = NULL; //newRoot встал на позицию корн€
		else
		{
			int i = INVALID_INDEX; 
			//убрать lastEl из детей newRoot->parent->parent (если lastEl - корень, то игнор)
			//добавить newRoot в детей newRoot->parent->parent (если lastEl - корень, то игнор)
			newRoot->parentPtr = newRoot->parentPtr->parentPtr;
		}
		/*if (reverseOpList(lastEl) == FAIL) //–еверс операций lastEl
		{
			printf("ERROR: unable to reverse operations.\n");
			return FAIL;
		}*/
		//обновить файл версии lastEl
	}
	//обновить файл версии newRoot
	return SUCCESS;
}

status_t mergeOperaions(version_t* ver)
{
	int stringLen = getMaxTextLen(ver);
	char* text = (char*)calloc(stringLen + 1, sizeof(char));
	if (getCurText(text, stringLen, ver) == FAIL)
	{
		free(text);
		return FAIL;
	}
	deleteOperationList(&(ver->operation));
	if (add(0, text, ver) == FAIL)
	{
		free(text);
		return FAIL;
	}
	return SUCCESS;
}