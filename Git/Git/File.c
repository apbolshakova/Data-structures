#include "Common.h"

char* getNameOfVerFile(int version)
{
	char* fileName = (char*)calloc(FNAME_LEN, sizeof(char));
	strncpy(fileName, generalInfo->name, 
		strlen(generalInfo->name) - strlen(getFileExt(generalInfo->name)));
	strcatInt(fileName, version);
	strcat(fileName, ".txt");
	return fileName;
}

void strcatInt(char* fileName, int version)
{
	char digitAsChar = '0';
	if (!version) strncat(fileName, &digitAsChar, 1);
	while (version)
	{
		digitAsChar = (version % DEC_NUMBER_SYSTEM) + '0';
		version /= DEC_NUMBER_SYSTEM;
		strncat(fileName, &digitAsChar, 1);
	}
}

bool_t exists(const char *fname)
{
	FILE *file;
	if ((file = fopen(fname, "r")))
	{
		fclose(file);
		return TRUE;
	}
	return FALSE;
}

const char* getFileExt(char* filename) 
{
	const char *dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
}

long int getFileSize(FILE *f)
{
	long int sav = ftell(f);
	fseek(f, 0L, SEEK_END);
	long int fileSize = ftell(f);
	fseek(f, sav, SEEK_SET);
	return(fileSize);
}

void getLastCreatedVersion()
{
	//проверить все файлы в директории и вытащить файл с наибольшим номером версии
	//записать его в generalInfo
}