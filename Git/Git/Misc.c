#include "Header.h"

char* getNameOfVerFile(int version)
{
	char* fileName = (char*)calloc(FNAME_LEN, sizeof(char));
	strncpy(fileName, generalInfo->name, 
		strlen(generalInfo->name) - strlen(getFileExt(generalInfo->name)));
	strcatInt(fileName, version);
	strcat(fileName, ".%s", TEXT_EXT);
	return fileName;
}

void strcatInt(char fileName[FNAME_LEN], int version)
{
	char* digitAsChar = "0";
	if (!version) strcat(fileName, digitAsChar);
	while (version)
	{
		digitAsChar = (version % DEC_NUMBER_SYSTEM) + '0';
		version /= DEC_NUMBER_SYSTEM;
		strcat(fileName, digitAsChar);
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

void cleanup()
{
	deleteGeneralInfo();
	deleteBuf();
}

const char* getFileExt(const char* filename) 
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