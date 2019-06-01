#include "Common.h"

status_t initVerTree()
{
	generalInfo->lastCreatedVersion = INIT_VERSION;
	if (initBuf(ROOT_VER) == FAIL) 	//create buffer with version number 0
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
	if (push() == FAIL) //save version as file (push), last created version is 0
	{
		printf("ERROR: unsuccessful attempt to push init version.\n");
		return FAIL;
	} 
	return SUCCESS;
}

status_t initTextAsOpearationInRootVer()
{
	char* text = NULL;
	if (getSourceText(&text) == FAIL)
	{
		printf("ERROR: Unable to get source text.\n");
		return FAIL;
	}
	//create add operation with source text
	if (add(0, text, ALL, NULL) == FAIL)
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
	if (!exists(rootFileName) && verNum != ROOT_VER) //an attempt to read non existable tree
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
	verList_t* lostVersList = NULL; //list of versions which parent wasn't found in tree
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
			if (handleVerFile(filePath, &lostVersList) == FAIL)
			{
				printf("ERROR: unable to load existing tree.\n");
				return FAIL;
			}
			if (insertLostVersIntoTree(&lostVersList) == FAIL) return FAIL;
		}
	} while (FindNextFile(hFind, &fdFile));
	FindClose(hFind);
	if (lostVersList)
	{
		printf("ERROR: found versions with not existing parents.\n");
		return FAIL;
	}
	return SUCCESS;
}

status_t handleVerFile(char filePath[FNAME_LEN], verList_t** lostVersList)
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
	ver->parentVerNum = parentVer;
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
	if (insertIntoTree(ver, parentVer, lostVersList) == FAIL)
	{
		printf("ERROR: unable to insert version into version tree.\n");
		return FAIL;
	}
	fclose(file);
	return SUCCESS;
}

status_t insertIntoTree(version_t* ver, int parentVerNum, verList_t** lostVersList)
{
	if (parentVerNum == INVALID_VER) generalInfo->root = ver;
	else if (!(ver->parentPtr)) shiftIntoPath(lostVersList, ver);
	else if (addChild(ver, ver->parentPtr) == FAIL)
	{
		
		printf("ERROR: unable to add child to version.\n");
		return FAIL;
	}
	generalInfo->lastCreatedVersion = ver->verNum; //inc number of last saved version
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
	if (buf->parentPtr && addChild(buf, buf->parentPtr) == FAIL) //save buffer as it's parent child
	{
		printf("ERROR: unable to add buffer to version as it's child.\n");
		return FAIL;
	}
	generalInfo->lastCreatedVersion = buf->verNum; //inc number of last saved version
	if (createVerFile() == FAIL)
	{
		printf("ERROR: unable to update version files after pushing new version.\n");
		return FAIL;
	}
	if (initBuf(generalInfo->lastCreatedVersion) == FAIL) //recreate buffer
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
	if (!(verToDelete->parentPtr))
	{
		if (verToDelete->childNum == 0)
		{
			printf("ERROR: attempt to delete single node.\n");
			return FAIL;
		}
		if (rebase(verToDelete->child[0]->verNum) == FAIL) 
		{
			printf("ERROR: unable to set root's child as new root.\n");
			return FAIL;
		}
		return deleteVer(verToDelete);
	}
	if (deleteFromChildren(verToDelete, NULL) == FAIL)
	{
		printf("ERROR: unable to delete version from it's parent children.\n");
		return FAIL;
	}
	if (copyVerChildren(verToDelete) == FAIL)
	{
		printf("ERROR: unable to attach version's children to it's parent.\n");
		return FAIL;
	}
	if (buf->parentPtr == verToDelete && relocateChild(verToDelete, BUF) == FAIL) //if buffer is child of version to remove change it's parent to parent of verToDelete
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

status_t deleteFromChildren(version_t* verToDelete, version_t* parent)
{
	if (!(verToDelete) || !(verToDelete->parentPtr)) return SUCCESS;
	int verPos = INVALID_INDEX;
	if (!parent)
	{
		for (int i = 0; i < verToDelete->parentPtr->childNum; i++)
		{
			if (verToDelete->parentPtr->child[i] == verToDelete)
			{
				verPos = i;
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < parent->childNum; i++)
		{
			if (parent->child[i] == verToDelete)
			{
				verPos = i;
				break;
			}
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

void moveBackChildren(version_t* parentPtr, int verPos)
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
	for (int i = 0; i < prevParent->childNum; i++)
	{
		if (relocateChild(prevParent, i) == FAIL) return FAIL;
	}
	return SUCCESS;
}

status_t relocateChild(version_t* prevParent, int i)
{
	operation_t* opListRoot = NULL; //new operation list
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
	if (!newChild || !parent) return SUCCESS;
    (parent->childNum)++;
	parent->child = (version_t**)realloc(parent->child, parent->childNum * sizeof(version_t*));
	if (!parent->child)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	parent->child[parent->childNum - 1] = newChild;
	newChild->parentVerNum = parent->verNum;
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
