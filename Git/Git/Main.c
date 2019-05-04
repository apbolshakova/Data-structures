#include "Header.h"

int main(void)
{
	char operation[OPERATION_LEN] = { 0 };
	char fname[FNAME_LEN] = { 0 };
	int version = 0;
	printf("Type \"open <fname> <version>\" to start:\n");
	scanf("%s %s %i", operation, fname, &version);
	
	if (!strcmp(operation, OPEN))
	{
		buf_t* verInfo = (buf_t*)malloc(sizeof(buf_t));
		verInfo->verNum = version;
		if (handleFile(fname, version, verInfo) == FAIL)
		{
			free(verInfo);
			printf("ERROR: Unable to open required file.\n");
			_getch();
			return 0;
		};
	}
	else printf("ERROR: Invalid operation. File opening required.\n");
	handleMainCycle(); //запуск основного цикла
	_getch();
	//TODO: cleanup
	return 0;
}
