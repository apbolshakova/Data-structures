#include "Header.h"

func_result_t initVerTree()
{
	generalInfo->lastCreatedVersion = INIT_VERSION;
	if (initBuf(ROOT_VER) == FAIL) 	//������� ����� � ������� �������
	{
		printf("ERROR: Unable to create init version of file because of invalid buffer initialization.\n");
		return FAIL;
	}
	generalInfo->root = buf;
	if (initTextAsOpearationInRootVer() == FAIL)
	{
		printf("ERROR: Unable to create init version of file because of invalid operation building.\n");
		return FAIL;
	}
	//�������� ������ � ����
	
	//��������� ��������� ������ - 0
	//�����
}

func_result_t initTextAsOpearationInRootVer()
{
	//������� ���� generalInfo->fileName
	char* text = NULL;
	if (getSourceText(&text) == FAIL)
	{
		printf("ERROR: Unable to get source text.\n");
		return FAIL;
	}
	//�������� � �������� ������ ������� add ������ �� �����
}

func_result_t getSourceText(char** text)
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
		printf("ERROR: Memory allocation error.\n");
		return FAIL;
	}
	char* temp = (char*)calloc(fileSize, sizeof(char));
	if (!temp)
	{
		printf("ERROR: Memory allocation error.\n");
		return FAIL;
	}
	while (fgets(temp, fileSize, source) != NULL) strcat(*text, temp);
	return SUCCESS;
}

void deleteVerTree()
{
	//TODO
}

//��������� �� ������������ ������ � ������ ������, ������ � ��������� ����� ������ ���������� � generalInfo
func_result_t buildVerTree() 
{
	if (!exists(getNameOfVerFile(ROOT_VER)))
	{
		initVerTree();
	}
	else
	{
		//load existing tree
	}
}