#include "Common.h"

general_t* generalInfo = NULL;
version_t* buf = NULL;

int main(void)
{
	char action[ACTION_LEN] = { 0 };
	char fname[FNAME_LEN] = { 0 };
	char temp[TEMP_LEN] = { 0 };
	int version = 0; //¬ерси€, котора€ станет родителем следующей. ≈сли version == 0, то это корень - верси€ с добавлением исходного текста
	
	printf("Type \"open <fname> <version>\" to start:\n");
	scanf_s("%s %s %s", action, (unsigned)sizeof(action), fname, (unsigned)sizeof(fname), temp, (unsigned)sizeof(temp));
	version = atoi(temp);
	
	if (strcmp(getFileExt(fname), TEXT_EXT))
	{
		printf("ERROR: Invalid source file. It must be a .%s file.\n", TEXT_EXT);
		goto End;
	}
	if (getID(action) != openID)
	{
		printf("ERROR: Invalid operation. File opening required.\n");
		goto End;
	}
	if (initGeneralInfo(fname, version) == FAIL)
	{
		printf("ERROR: unable to get general info about tree of versions.\n");
		goto End;
	}
	handleMainCycle();
End:
	cleanup();
	return 0;
}

void handleMainCycle()
{
	char action[ACTION_LEN] = { 0 };
	int actionID = INVALID_ACTION;
	do {
		system("cls");
		printMainMenu();
		fflush(stdin);
		scanf_s("%s", action, (unsigned)sizeof(action));
		actionID = getID(action);
		if (handleAction(actionID) == FAIL) printf("\nAction wasn't completed.\n");
		printf("Press any button to continue.\n");
		_getch();
	} while (actionID != closeID);
}

void printMainMenu()
{
	printf("Enter required action:");
	printf("\nBasic:\n");
	printf("- print\n");
	printf("- close\n");

	printf("\nEditing:\n");
	printf("- edit\n");
	printf("- add\n");
	printf("- remove\n");
	printf("- merge\n");

	printf("\nVersions handling:\n");
	printf("- push\n");
	printf("- pull\n");
	printf("- delete_version\n");
	printf("- rebase\n\n");
}

int getID(char action[ACTION_LEN])
{
	const char* actions[] =
	{
		"open",
		"close",
		"print",
		"edit",
		"add",
		"remove",
		"merge",
		"push",
		"pull",
		"delete_version",
		"rebase"
	};
	for (int i = 0; i < countID; i++)
	{
		if (!strcmp(action, actions[i])) return i;
	}
	return INVALID_ACTION;
}

status_t handleAction(int ID)
{
	system("cls");
	switch (ID) 
	{
	case printID: return print(); break;
	case editID: return handleEditing(); break;
	case addID: return handleAdd(); break;
	case removeID: return handleRemoving(); break;
	case mergeID: return SUCCESS; break; //TODO
	case pushID: return push(); break;
	case pullID: return pull(); break; //TODO
	case deleteVerID: return handleVerDeleting(); break;
	case rebaseID: return SUCCESS; break; //TODO
	case closeID: return SUCCESS;
	}
	printf("ERROR: Invalid action.\n");
	return FAIL;
}

void cleanup()
{
	deleteGeneralInfo();
	deleteBuf();
}