#include "Common.h"

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
	if (reverseVerOperations(newRoot) == FAIL)
	{
		printf("ERROR: unable to reverse operations.\n");
		return FAIL;
	}
	if (setVerAsRoot(newRoot) == FAIL)
	{
		printf("ERROR: unable to change version bonds.\n");
		return FAIL;
	}
	return SUCCESS;
}

status_t setVerAsRoot(version_t* newRoot)
{
	version_t* lastEl = newRoot; //конец пути, по которому можно дойти от корня до элемента (newRoot поднимаетcя по этому пути)
	while (newRoot->parentPtr != NULL)
	{
		version_t* par = newRoot->parentPtr;
		version_t* parOfPar = NULL;
		if (par->parentPtr) parOfPar = par->parentPtr;
		if (deleteFromChildren(par, parOfPar) == FAIL ||
			deleteFromChildren(newRoot, par) == FAIL)
		{
			printf("ERROR: unable to move version down.\n");
			return FAIL;
		}
		if (!parOfPar)
		{
			newRoot->parentPtr = NULL;
			newRoot->parentVerNum = INVALID_VER;
		}
		else if (addChild(newRoot, parOfPar) == FAIL)
		{
			printf("ERROR: unable to switch versions positions in tree.\n");
			return FAIL;
		}
		if (addChild(par, lastEl) == FAIL)
		{
			printf("ERROR: unable to switch versions positions in tree.\n");
			return FAIL;
		}
		lastEl = par;
		if (rewriteVerFile(lastEl) == FAIL)
		{
			printf("ERROR: unable to rewrite version file.\n");
			return FAIL;
		}
	}
	generalInfo->root = newRoot;
	if (rewriteVerFile(newRoot) == FAIL)
	{
		printf("ERROR: unable to rewrite version file of new root.\n");
		return FAIL;
	}
}