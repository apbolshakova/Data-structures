#include "Header.h"

func_result_t handleFile(char fname[FNAME_LEN], int version, buf_t* verInfo) //�������� � verInfo ������ � ������ � ������ �����
{
	char* fileName = (char*)malloc((FNAME_LEN + VERSION_CHARS_NUM) * sizeof(char));
	strncpy(fileName, fname, strlen(fname));
	fileName[strlen(fname)] = '.';
	_itoa(version, fileName + strlen(fname) + 1, DEC_NUMBER_SYSTEM);
	FILE* verFile = fopen(fileName, 'r'); //������� ���� fname.version
	if (!verFile || loadVersion(verInfo, verFile) == FAIL) return FAIL;
	return SUCCESS;
}

func_result_t loadVersion(buf_t* verInfo, FILE* verFile) //�������� � verInfo ��� ��������� ������
{
	fscanf(verFile, "%i", &(verInfo->parentVer));
	verInfo->text = getTextInCurVer(verInfo->parentVer); //������ �� ������ � �������� ������� text
	if (verInfo->text == NULL) return FAIL;
	verInfo->textLen = strlen(verInfo->text); //���������� textLen
	verInfo->unsavedChange = (change_t*)malloc(sizeof(change_t)); //������� ������ ������ unsavedChange
	if (verInfo->unsavedChange == NULL)
	{
		free(verInfo->text);
		verInfo->text = NULL;
		return FAIL;
	}
	return SUCCESS;
}

char* getTextInCurVer(buf_t* verInfo)
{
	//TODO: �����������
}