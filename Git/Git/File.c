#include "Common.h"

char* getNameOfVerFile(int version)
{
	char* fileName = (char*)calloc(FNAME_LEN, sizeof(char));
	char* fileExt = getFileExt(generalInfo->name);
	strncpy(fileName, generalInfo->name,
		strlen(generalInfo->name) - strlen(fileExt) - 1);
	strcat(fileName, "\\");
	strncat(fileName, generalInfo->name, 
		strlen(generalInfo->name) - strlen(fileExt));
	strcatInt(fileName, version);
	strcat(fileName, ".txt");
	return fileName;
}

char* getDirName()
{
	char* dirName = (char*)calloc(FNAME_LEN, sizeof(char));
	char* fileExt = getFileExt(generalInfo->name);
	strncpy(dirName, generalInfo->name,
		strlen(generalInfo->name) - strlen(fileExt) - 1);
	return dirName;
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
		return TRUE_;
	}
	return FALSE_;
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

int getVerNum(char filePath[FNAME_LEN])
{
	char* firstDot = strchr(filePath, '.');
	if (!firstDot) return INVALID_VER;
	char* secondDot = strchr(firstDot + 1, '.');
	if (!secondDot) return INVALID_VER;
	
	int verLen = secondDot - firstDot;
	char* ver = (char*)calloc(verLen, sizeof(char));
	strncpy(ver, firstDot + 1, verLen - 1);
	return atoi(ver);
}