#include "Header.h"

int main(void)
{
	char operation[OPERATION_LEN] = { 0 };
	char fname[FNAME_LEN] = { 0 };
	size_t version = 0;
	printf("Type \"open <fname> <version>\" to start:\n");
	scanf("%s %s %i", operation, fname, &version);
	
	if (!strcmp(operation, OPEN))
	{
		buf_t* buf = (buf_t*)malloc(sizeof(buf_t));
		if (openFile(fname, version, buf) == FAIL)
		{
			printf("ERROR: Unable to open required file.\n"); //TODO: ������� �� � ��� ���� ������� ��� 1-� ����� ������ �� ���������
			_getch();
			return 0;
		};
	}
	else printf("ERROR: Invalid operation. File opening required.\n");
	
	//������ ��������� �����
	_getch();
	return 0;
}

func_result_t openFile(char fname[FNAME_LEN], size_t version, buf_t* buf)
{
    //������� ���� fname.version
	//���� ���������� - ������������ �����
	//���� �� ���������� - ������
}