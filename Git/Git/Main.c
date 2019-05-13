#include "Header.h"

general_t* generalInfo = NULL;
version_t* buf = NULL;

int main(void)
{
	char action[ACTION_LEN] = { 0 };
	char* fname = (char*)calloc(FNAME_LEN, sizeof(char));
	char temp[TEMP_LEN] = { 0 };
	int version = 0; //¬ерси€, котора€ станет родителем следующей. ≈сли version == 0, то это корень - верси€ с добавлением исходного текста
	
	printf("Type \"open <fname> <version>\" to start:\n");
	scanf("%s %s %s", action, fname, temp);
	version = atoi(temp);
	
	if (strcmp(getFileExt(fname), TEXT_EXT))
	{
		printf("ERROR: Invalid source file. It must be a .%s file.\n", TEXT_EXT);
		goto End;
	}
	if (strcmp(action, OPEN))
	{
		printf("ERROR: Invalid operation. File opening required.\n");
		goto End;
	}
	if (initGeneralInfo(fname) == FAIL)
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
	do {
		system("cls");
		printMainMenu();
		scanf("%s", action);
		if (handleAction(action) == FAIL) printf("\nAction wasn't completed.\n");
		printf("Press any button to continue.\n");
		_getch();
	} while (strcmp(action, CLOSE));
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

func_res_t handleAction(char action[ACTION_LEN])
{
	system("cls");
	if (!strcmp(action, PRINT) && print() != FAIL) return SUCCESS;
	if (!strcmp(action, EDIT))
	{
		//TODO
	}
	if (!strcmp(action, ADD) && handleAdd() != FAIL) return SUCCESS;
	if (!strcmp(action, REMOVE))
	{
		//TODO
	}
	if (!strcmp(action, MERGE))
	{
		//TODO
	}
	if (!strcmp(action, PUSH) && push() != FAIL) return SUCCESS;
	if (!strcmp(action, PULL))
	{
		//TODO
	}
	if (!strcmp(action, DELETE_VERSION))
	{
		//TODO
	}
	if (!strcmp(action, REBASE))
	{
		//TODO
	}
	if (!strcmp(action, CLOSE)) return SUCCESS;
	printf("ERROR: Invalid action.\n");
	return FAIL;
}

void cleanup()
{
	deleteGeneralInfo();
	deleteBuf();
}