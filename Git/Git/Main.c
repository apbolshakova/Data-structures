#include "Header.h"

general_t* generalInfo = NULL;
version_t* buf = NULL;

int main(void)
{
	char operation[OPERATION_LEN] = { 0 };
	char* fname = (char*)calloc(FNAME_LEN, sizeof(char));
	int version = 0; //Версия, которая станет родителям следующей. Если version == 0, то корень - версия, когда никаких изменений нет (нулевая)
	printf("Type \"open <fname> <version>\" to start:\n");
	scanf("%s %s %i", operation, fname, &version);
	
	if (strcmp(getFileExt(fname), TEXT_EXT))
	{
		printf("ERROR: Invalid source file. It must be a .%s file.\n", TEXT_EXT);
		goto End;
	}
	if (strcmp(operation, OPEN))
	{
		printf("ERROR: Invalid operation. File opening required.\n");
		goto End;
	}
	if (initGeneralInfo(fname) == FAIL)
	{
		printf("ERROR: unable to get general info about tree of versions.\n");
		goto End;
	}
	if (initBuf(version) == FAIL)
	{
		printf("ERROR: memory allocation problem.\n");
		goto End;
	};
	//handleMainCycle(); //запуск основного цикла
End:
	cleanup();
	_getch();
	return 0;
}
