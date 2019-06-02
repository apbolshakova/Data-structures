#include "Common.h"

char* getNameOfVerFile(int version)
{
	char* fileName = (char*)calloc(FNAME_LEN, sizeof(char));
	if (!fileName) return NULL;
	char* fileExt = getFileExt(generalInfo->name);
	strncpy(fileName, generalInfo->name,
		strlen(generalInfo->name) - strlen(fileExt) - 1);
	strcat(fileName, "\\");
	strncat(fileName, generalInfo->name, 
		strlen(generalInfo->name) - strlen(fileExt));
	char* numAsStr = (char*)calloc(VERSION_CHARS_NUM, sizeof(char));
	if (!numAsStr) return NULL;
	_itoa(version, numAsStr, 10);
	strcat(fileName, numAsStr);
	free(numAsStr);
	strcat(fileName, ".txt");
	return fileName;
}

char* getDirName()
{
	char* dirName = (char*)calloc(FNAME_LEN, sizeof(char));
	if (!dirName) return NULL;
	char* fileExt = getFileExt(generalInfo->name);
	strncpy(dirName, generalInfo->name,
		strlen(generalInfo->name) - strlen(fileExt) - 1);
	return dirName;
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

status_t getDataFromFile(char** storage, FILE* file)
{
	char* data = (char*)calloc(TEMP_LEN, sizeof(char));
	char ch = '\0';
	fscanf_s(file, "%c", &ch); //reads space
	do
	{
		fscanf_s(file, "%c", &ch);
		if (ch == EOF)
		{
			free(data);
			printf("ERROR: unexpected end of file.\n");
			return FAIL;
		}
		strncat(data, &ch, 1);
		if (strlen(data) % (TEMP_LEN - 1) == 0)
			data = (char*)realloc(data, (strlen(data) + TEMP_LEN) * sizeof(char));
	} while (ch != NEW_STRING);
	data[strlen(data) - 1] = '\0';
	*storage = data;
	return SUCCESS;
}