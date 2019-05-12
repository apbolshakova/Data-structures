#include "Header.h"

general_t* generalInfo = NULL;
version_t* buf = NULL;

int main(void)
{
	char action[ACTION_LEN] = { 0 };
	char* fname = (char*)calloc(FNAME_LEN, sizeof(char));
	char temp[TEMP_LEN] = { 0 };
	int version = 0; //Версия, которая станет родителям следующей. Если version == 0, то корень - версия, когда никаких изменений нет (нулевая)
	
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
	handleMainCycle(); //запуск основного цикла
End:
	cleanup();
	_getch();
	return 0;
}

void handleMainCycle()
{
	char action[ACTION_LEN] = { 0 };
	do {
		system("cls");
		printMainMenu();
		scanf("%s", action);
		if (isAction(action)) handleAction(action);
	} while (!strcmp(action, CLOSE));
}

void printMainMenu()
{
	printf("Enter required action:\n\n");
	printf("Basic:\n");
	printf("- print\n");

	printf("Editing:\n");
	printf("- edit\n");
	printf("- add\n");
	printf("- remove\n");
	printf("- merge\n");

	printf("Versions handling:\n");
	printf("- push\n");
	printf("- pull\n");
	printf("- delete_version\n");
	printf("- rebase\n");
}